# OrderBookImplementation

For building (Tested on Windows 11 and godbolt.org):

```
mkdir build
cd build
cmake .. -GNinja
ninja
```

Performance is measured to be ~6000 nanoseconds per update_bbo() call.
