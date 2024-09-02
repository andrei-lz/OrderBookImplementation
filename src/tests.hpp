#pragma once

#include <cassert>
#include <iostream>

#include "BinanceBook.hpp"

// Assuming PriceQuantity, format_double, and BinanceBook classes are defined as per the provided implementation.

class Tests {
public:

    static void test_empty_book() {
        BinanceBook<20> book;
        assert(book.is_empty());
        std::cout << "Test empty book passed.\n";
    }

    static void test_replace_book() {
        BinanceBook<20> book;

        // Define some bids and asks
        std::vector<PriceQuantity> bids = {
            {20078.54000000, 0.00431000},
            {20078.39000000, 0.00100000},
            {20078.27000000, 0.00070000}
        };

        std::vector<PriceQuantity> asks = {
            {20078.91000000, 0.03497000},
            {20078.95000000, 0.00100000},
            {20078.99000000, 0.00498000}
        };

        // Replace book with new bids and asks
        book.replace(bids, asks);
        auto [extracted_bids, extracted_asks] = book.extract();

        // Check if the book state matches expected state
        assert(extracted_bids == bids);
        assert(extracted_asks == asks);

        std::cout << "Test replace book passed.\n";
    }

    static void test_replace_bid_quantity()
    {
        BinanceBook<20> book;

        std::vector<PriceQuantity> bids =
        {
            {5, 0.5},
            {4, 0.5},
            {3, 0.5},
            {2, 0.5},
        };

        book.replace(bids, {});

        book.new_best_bid({ 3, 0.9 });

        std::vector<PriceQuantity> expected_bids =
        {
            //{5, 0.5},// ?
            //{4, 0.5},// ?
            {3, 0.9},
            {2, 0.5},
        };

        auto [extracted_bids, extracted_asks] = book.extract();

        // Check if the book state matches expected state
        assert(extracted_bids == expected_bids);

        std::cout << "Test add bid 1 passed.\n";

    }

    static void test_replace_ask_quantity()
    {
        BinanceBook<20> book;

        std::vector<PriceQuantity> asks =
        {
            {2, 0.5},
            {3, 0.5},
            {4, 0.5},
            {5, 0.5},
        };

        book.replace({}, asks);

        book.new_best_ask({ 4, 0.9 });

        std::vector<PriceQuantity> expected_asks =
        {
            //{2, 0.5},
            //{3, 0.5},
            {4, 0.9},
            {5, 0.5},
        };

        auto [extracted_bids, extracted_asks] = book.extract();

        // Check if the book state matches expected state
        assert(extracted_asks == expected_asks);

        std::cout << "Test add ask 1 passed.\n";
    }

    static void test_add_bid_1()
    {
        BinanceBook<20> book;

        std::vector<PriceQuantity> bids =
        {
            {5, 0.5},
            {4, 0.5},
            {3, 0.5},
            {2, 0.5},
        };

        book.replace(bids, {});
    
        book.new_best_bid({6, 0.5});

        std::vector<PriceQuantity> expected_bids =
        {
            {6, 0.5},
            {5, 0.5},
            {4, 0.5},
            {3, 0.5},
            {2, 0.5},
        };
    
        auto [extracted_bids, extracted_asks] = book.extract();

        // Check if the book state matches expected state
        assert(extracted_bids == expected_bids);

        std::cout << "Test add bid 1 passed.\n";

    }


    static void test_add_bid_2()
    {
        BinanceBook<20> book;

        std::vector<PriceQuantity> bids =
        {
            {5, 0.5},
            {4, 0.5},
            {3, 0.5},
            {2, 0.5},
        };

        book.replace(bids, {});

        book.new_best_bid({ 4.5, 0.5 });

        std::vector<PriceQuantity> expected_bids =
        {
            {4.5, 0.5},
            {4, 0.5},
            {3, 0.5},
            {2, 0.5},
        };

        auto [extracted_bids, extracted_asks] = book.extract();

        // Check if the book state matches expected state
        assert(extracted_bids == expected_bids);

        std::cout << "Test add bid 2 passed.\n";

    }

    static void test_add_bid_3()
    {
        BinanceBook<20> book;

        std::vector<PriceQuantity> bids =
        {
            {5, 0.5},
            {4, 0.5},
            {3, 0.5},
            {2, 0.5},
        };

        book.replace(bids, {});

        book.new_best_bid({ 3.5, 0.5 });

        std::vector<PriceQuantity> expected_bids =
        {
            {3.5, 0.5},
            {3, 0.5},
            {2, 0.5},
        };

        auto [extracted_bids, extracted_asks] = book.extract();

        // Check if the book state matches expected state
        assert(extracted_bids == expected_bids);

        std::cout << "Test add bid 3 passed.\n";

    }

    static void test_add_ask_1()
    {
        BinanceBook<20> book;

        std::vector<PriceQuantity> asks =
        {
            {2, 0.5},
            {3, 0.5},
            {4, 0.5},
            {5, 0.5},
        };

        book.replace({}, asks);

        book.new_best_ask({ 1, 0.5 });

        std::vector<PriceQuantity> expected_asks =
        {
            {1, 0.5},
            {2, 0.5},
            {3, 0.5},
            {4, 0.5},
            {5, 0.5},
        };

        auto [extracted_bids, extracted_asks] = book.extract();

        // Check if the book state matches expected state
        assert(extracted_asks == expected_asks);

        std::cout << "Test add ask 1 passed.\n";
    }


    static void test_add_ask_2()
    {
        BinanceBook<20> book;

        std::vector<PriceQuantity> asks =
        {
            {2, 0.5},
            {3, 0.5},
            {4, 0.5},
            {5, 0.5},
        };

        book.replace({}, asks);

        book.new_best_ask({ 2.5, 0.5 });

        std::vector<PriceQuantity> expected_asks =
        {
            {2.5, 0.5},
            {3, 0.5},
            {4, 0.5},
            {5, 0.5},
        };

        auto [extracted_bids, extracted_asks] = book.extract();

        // Check if the book state matches expected state
        assert(extracted_asks == expected_asks);

        std::cout << "Test add ask 2 passed.\n";

    }

    static void test_add_ask_3()
    {
        BinanceBook<20> book;

        std::vector<PriceQuantity> asks =
        {
            {2, 0.5},
            {3, 0.5},
            {4, 0.5},
            {5, 0.5},
        };

        book.replace({}, asks);

        book.new_best_ask({ 4.5, 0.5 });

        std::vector<PriceQuantity> expected_asks =
        {
            {4.5, 0.5},
            {5, 0.5},
        };

        auto [extracted_bids, extracted_asks] = book.extract();

        // Check if the book state matches expected state
        assert(extracted_asks == expected_asks);

        std::cout << "Test add ask 3 passed.\n";
    }

    static void test_add_bid_with_cross()
    {
        BinanceBook<20> book;

        // Initial book setup
        std::vector<PriceQuantity> bids =
        {
            {4, 0.5},
            {3, 0.5},
            {2, 0.5},
            {1, 0.5}
        };

        std::vector<PriceQuantity> asks =
        {
            {6, 0.5},
            {7, 0.5},
            {8, 0.5},
            {9, 0.5}
        };

        // Replace initial book state
        book.replace(bids, asks);

        // Update with new BBO that changes the top of the book
        book.new_best_bid({ 6.5, 0.5 });

        // Expected state after update
        std::vector<PriceQuantity> expected_bids = 
        {
            {6.5, 0.5},
            {4, 0.5},
            {3, 0.5},
            {2, 0.5},
            {1, 0.5}
        };

        std::vector<PriceQuantity> expected_asks =
        {
            {7, 0.5},
            {8, 0.5},
            {9, 0.5}
        };

        auto [extracted_bids, extracted_asks] = book.extract();
        assert(extracted_bids == expected_bids);
        assert(extracted_asks == expected_asks);

        std::cout << "Test add best bid with cross passed.\n";
    }

    static void test_add_bidask_cross_both()
    {
        BinanceBook<20> book;

        // Initial book setup
        std::vector<PriceQuantity> bids =
        {
            {4, 0.5},
            {3, 0.5},
            {2, 0.5},
            {1, 0.5}
        };

        std::vector<PriceQuantity> asks =
        {
            {6, 0.5},
            {7, 0.5},
            {8, 0.5},
            {9, 0.5}
        };

        // Replace initial book state
        book.replace(bids, asks);

        // Update with new BBO that changes the top of the book
        book.update_bbo({ 6.5, 0.5 }, { 3.5, 0.5 });

        // Expected state after update
        std::vector<PriceQuantity> expected_bids =
        {
            {3, 0.5},
            {2, 0.5},
            {1, 0.5}
        };

        std::vector<PriceQuantity> expected_asks =
        {
            {3.5, 0.5},
            {7, 0.5},
            {8, 0.5},
            {9, 0.5}
        };

        auto [extracted_bids, extracted_asks] = book.extract();
        assert(extracted_bids == expected_bids);
        assert(extracted_asks == expected_asks);

        std::cout << "Test add best ask with cross passed.\n";
    }


    static void test_add_ask_with_cross()
    {
        BinanceBook<20> book;

        // Initial book setup
        std::vector<PriceQuantity> bids =
        {
            {4, 0.5},
            {3, 0.5},
            {2, 0.5},
            {1, 0.5}
        };

        std::vector<PriceQuantity> asks =
        {
            {6, 0.5},
            {7, 0.5},
            {8, 0.5},
            {9, 0.5}
        };

        // Replace initial book state
        book.replace(bids, asks);

        // Update with new BBO that changes the top of the book
        book.new_best_ask({ 2.5, 0.5 });

        // Expected state after update
        std::vector<PriceQuantity> expected_bids =
        {
            {2, 0.5},
            {1, 0.5}
        };

        std::vector<PriceQuantity> expected_asks =
        {
            {2.5, 0.5},
            {6, 0.5},
            {7, 0.5},
            {8, 0.5},
            {9, 0.5}
        };

        auto [extracted_bids, extracted_asks] = book.extract();
        assert(extracted_bids == expected_bids);
        assert(extracted_asks == expected_asks);

        std::cout << "Test add best bid with cross, then best ask with cross passed.\n";
    }

    static void test_book_overflow_handling() {
        BinanceBook<20> book;

        // Populate the book with 20 levels of bids and asks.
        std::vector<PriceQuantity> initial_bids;
        std::vector<PriceQuantity> initial_asks;

        // Fill bids from 100.0 downwards and asks from 101.0 upwards.
        for (int i = 0; i < 20; ++i) {
            initial_bids.push_back({ 100.0 - i * 0.1, 0.5 + i * 0.01 });  // Bid prices: 100.0, 99.9, ..., 98.1
            initial_asks.push_back({ 101.0 + i * 0.1, 0.4 + i * 0.01 });  // Ask prices: 101.0, 101.1, ..., 102.9
        }

        // Replace the current book state with these initial values.
        book.replace(initial_bids, initial_asks);

        // Add a new best bid and best ask to trigger the overflow condition.
        PriceQuantity new_best_bid{ 100.1, 0.6 }; // A bid higher than the current best.
        PriceQuantity new_best_ask{ 100.9, 0.35 }; // An ask lower than the current best.

        // Update the BBO; this should push the book size to exceed 20 levels, triggering the overflow handling.
        book.update_bbo(new_best_bid, new_best_ask);

        // Expected state:
        // - The new best bid/ask should be inserted.
        // - The lowest bid (price 98.1) and the highest ask (price 102.9) should be removed to maintain the max size of 20.

        // Prepare expected bids and asks after overflow handling.
        std::vector<PriceQuantity> expected_bids = {
            {100.1, 0.6},  // New top bid
            {100.0, 0.5},
            {99.9, 0.51},
            {99.8, 0.52},
            {99.7, 0.53},
            {99.6, 0.54},
            {99.5, 0.55},
            {99.4, 0.56},
            {99.3, 0.57},
            {99.2, 0.58},
            {99.1, 0.59},
            {99.0, 0.6},
            {98.9, 0.61},
            {98.8, 0.62},
            {98.7, 0.63},
            {98.6, 0.64},
            {98.5, 0.65},
            {98.4, 0.66},
            {98.3, 0.67},
            {98.2, 0.68}   // The lowest bid (98.1) has been removed.
        };

        std::vector<PriceQuantity> expected_asks = {
            {100.9, 0.35}, // New top ask
            {101.0, 0.4},
            {101.1, 0.41},
            {101.2, 0.42},
            {101.3, 0.43},
            {101.4, 0.44},
            {101.5, 0.45},
            {101.6, 0.46},
            {101.7, 0.47},
            {101.8, 0.48},
            {101.9, 0.49},
            {102.0, 0.5},
            {102.1, 0.51},
            {102.2, 0.52},
            {102.3, 0.53},
            {102.4, 0.54},
            {102.5, 0.55},
            {102.6, 0.56},
            {102.7, 0.57},
            {102.8, 0.58}   // The highest ask (102.9) has been removed.
        };

        // Extract the current state of bids and asks after the overflow.
        auto [extracted_bids, extracted_asks] = book.extract();

        // Verify that the book state matches the expected state after overflow handling.
        assert(extracted_bids == expected_bids);
        assert(extracted_asks == expected_asks);

        std::cout << "Test book overflow handling passed.\n";
    }

};

static void runTests()
{
    Tests::test_empty_book();    
    Tests::test_replace_book();

    Tests::test_replace_bid_quantity();
    Tests::test_replace_ask_quantity();

    Tests::test_add_bid_1();
    Tests::test_add_bid_2();
    Tests::test_add_bid_3();

    Tests::test_add_ask_1();
    Tests::test_add_ask_2();
    Tests::test_add_ask_3();

    Tests::test_add_bid_with_cross();
    Tests::test_add_ask_with_cross();

    Tests::test_add_bidask_cross_both();

    Tests::test_book_overflow_handling();

    std::cout << "All Tests Passed Successfully";
}