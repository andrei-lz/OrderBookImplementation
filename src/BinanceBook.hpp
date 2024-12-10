/*

A book for Binance, or any other similar protocol.
We get updates which are up to the maximum 20 levels every 100ms, in canonical order,
ie bids highest->lowest, asks lowest->highest.
Then we get incremental best bid/offer (BBO) updates, so we need to update top of book and uncross.
Terminology: "bid" is an order to buy at that price, "offer" or "ask" is an order to sell.

API docs for reference - note you can assume this data has been fetched and parsed into some appropriate data structures
like std::vector.
https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#partial-book-depth-streams
https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-book-ticker-streams

Incoming JSON for reference is:
Full book update:
{"lastUpdateId":34698491742,"bids":[["20078.54000000","0.00431000"],["20078.39000000","0.00100000"],
["20078.27000000","0.00070000"],["20078.21000000","0.00066000"],["20077.91000000","0.03781000"],
["20077.90000000","0.00110000"],["20077.86000000","0.00070000"],["20077.80000000","0.00650000"],
["20077.73000000","0.00055000"],["20077.71000000","0.00100000"],["20077.69000000","0.00984000"],
["20077.66000000","0.00066000"],["20077.61000000","0.04000000"],["20077.60000000","0.02484000"],
["20077.56000000","0.05481000"],["20077.52000000","0.28882000"],["20077.51000000","0.00064000"],
["20077.45000000","0.00070000"],["20077.43000000","0.05181000"],["20077.39000000","0.00689000"]],
"asks":[["20078.91000000","0.03437000"],["20078.95000000","0.00100000"],["20078.99000000","0.00498000"],
["20079.01000000","0.04981000"],["20079.09000000","0.00070000"],["20079.15000000","0.24902000"],
["20079.30000000","0.04110000"],["20079.31000000","0.00066000"],["20079.35000000","0.00864000"],
["20079.42000000","0.00100000"],["20079.44000000","0.09402000"],["20079.46000000","0.09402000"],
["20079.49000000","0.00100000"],["20079.50000000","0.00070000"],["20079.51000000","0.17430000"],
["20079.53000000","0.09602000"],["20079.60000000","0.00100000"],["20079.61000000","0.22853000"],
["20079.62000000","0.08741000"],["20079.66000000","0.00400000"]]}
*/

#pragma once

#include <vector>

#include "util.hpp"

using Price = double;
using Quantity = double;

struct PriceQuantity
{
    Price price{};
    Quantity quantity{};

    auto operator<=>(const PriceQuantity&) const = default; // if C++20 available.
    auto operator==(const PriceQuantity& pq) const { return essentiallyEqual(price, pq.price) && essentiallyEqual(quantity, pq.quantity); };
};

template <size_t n>
class BinanceBook final
{
    friend class Tests; // So I can run the tests

public:
    BinanceBook() = default;
    BinanceBook(const BinanceBook&) = delete;
    BinanceBook(BinanceBook&&) = delete;
    BinanceBook& operator=(const BinanceBook&) = delete;
    BinanceBook& operator=(const BinanceBook&&) = delete;

    // Clear the book
    // clear()

    void clear()
    {
        bids.clear();
        asks.clear();
    }

    // Test whether book is empty.
    // bool is_empty()
    bool is_empty() const { return extract().first[0].price == 0.0 && extract().second[0].price == 0.0; }

    // Replace entire contents of book with given bids / asks (assumed to be in canonical order).
    // Assumes the inputs are already sorted and valid
    // Bonus: accept any suitable container of PriceQuantity
    // replace(bids, asks)
    void replace(const std::vector<PriceQuantity>& new_bids, const std::vector<PriceQuantity>& new_asks) {
        bids = new_bids;
        asks = new_asks;
    }

    // Apply a new best bid / ask.
    // update_bbo(new_best_bid, new_best_ask)
    void update_bbo(const PriceQuantity& newbbid, const PriceQuantity& newbask)
    {
        // Update the top of the book and infer any changes needed.
        new_best_bid(newbbid);
        new_best_ask(newbask);
    }

    // Retrieve the book (in canonical order).
    // This should output something similar to the input for `replace()`.
    // extract()
    // Retrieve the current state of the book in canonical order
    std::pair<std::vector<PriceQuantity>, std::vector<PriceQuantity>> extract() const {
        return { bids, asks };
    }

    // to_string() - convert to string for output.
    // This should be efficient but isn't performance critical.
    std::string to_string() const
    {
        /*
        std::ostringstream is relatively slow because it manages a dynamic buffer and performs
        frequent reallocations. Instead, we can use std::string with reserve() to pre-allocate sufficient
        memory, reducing reallocations and improving performance.
        */

        //Pre-calculate the expected size to reduce allocations
        size_t estimated_size = std::max(bids.size(), asks.size()) * 80; // Rough estimation per line
        std::string result;
        result.reserve(estimated_size);

        size_t max_size = std::max(bids.size(), asks.size());

        for (size_t i = 0; i < max_size; ++i) {
            if (i < bids.size()) {
                result += '[' + std::to_string(i + 1) + "] [";
                result += format_double(bids[i].quantity, 8) + "] ";
                result += format_double(bids[i].price, 8) + " | ";
            }
            else {
                result += "                      | ";
            }

            if (i < asks.size()) {
                result += format_double(asks[i].price, 8) + " [";
                result += format_double(asks[i].quantity, 8) + "]";
            }

            result += '\n';
        }

        return result;
    }

private:
    /*
    side A and side B distinction so we can deal with "crossing" values
    side A and side B will swap depending on which vector we're reducing
    is_bid is used to decide which way the comparators go as canonicity is reversed for bids vs asked
    */
    inline void update_side(std::vector<PriceQuantity>& sideA, std::vector<PriceQuantity>& sideB, const PriceQuantity& new_top, bool is_bid)
    {
        // Timer t; // For measuring performance
        if (new_top.price <= 0) [[unlikely]] // For Testing
            return;

        // Find the appropriate position to insert or update the new top price level
        auto it = std::lower_bound(sideA.begin(), sideA.end(), new_top,
            [is_bid](const PriceQuantity& a, const PriceQuantity& b) {
                return is_bid ? a.price > b.price : a.price < b.price;
            });

        // If the price exists, update the quantity
        if (it != sideA.end() && essentiallyEqual(it->price, new_top.price)) {
            it->quantity = new_top.quantity;
        }
        else {
            // Otherwise insert the new price level at the correct position
            it = sideA.insert(it, new_top);
        }

        // If erase everything before our BEST bid/ask because if the new one is now the best, the previously "better" ones
        // Must not be valid anymore or have already been fulfilled
        if (sideA.begin() < it)
            sideA.erase(sideA.begin(), it);

        // Trim the vector size to maintain 20 depth
        while (sideA.size() > 20)
            sideA.pop_back(); // Vector pop_back implementation reduces size, not capacity therefore allocating n + 1 saves time

        // Remove all sideB that are less than or equal to the new bid price this is fixing the "crossover" issue
        sideB.erase(std::remove_if(sideB.begin(), sideB.end(), [new_top, is_bid](const PriceQuantity& ask) {
            return is_bid ? ask.price < new_top.price : ask.price > new_top.price;
            }), sideB.end());
    }

    inline void new_best_bid(const PriceQuantity& new_top) { update_side(bids, asks, new_top, /*is_bid=*/true); }
    inline void new_best_ask(const PriceQuantity& new_top) { update_side(asks, bids, new_top, /*is_bid=*/false); };

    std::vector<PriceQuantity> bids{ n + 1 }, asks{ n + 1 }; // Allocating n + 1 to minimise speed impact of overflow

};
