#include <cstdint> // For fixed-width integer types like uint64_t, uint32_t. These types ensure predictable memory layout, essential for performance-critical systems
#include <cstring> // For std::memcpy — used for fast, low-level memory copying. Often used in market data systems to copy raw bytes efficiently without constructors
#include <immintrin.h> // For Intel SIMD intrinsics (e.g., _mm_prefetch). // Enables prefetching and vectorized processing — crucial for reducing latency
#include <iostream>

// Align to 64 bytes to avoid false sharing
struct alignas(64) MDT { // alignas as aligns data with the cache line
    uint64_t timestamp_ns;
    double last_price;
    double bid_price;
    double ask_price;
    uint32_t bid_size;
    uint32_t ask_size;
    char symbol[8]; // for layout predictability

    uint8_t _pad[64 - (8 + (8 * 3) + (4 * 2) +
                       8)]; // padding to align it to 64byte cache line exactly.
    /*
     8 -> uint64_t
     8*3 -> double
     4*2 -> uint32_t
     8 -> char
    */
};

static_assert(sizeof(MDT) == 64, "MDT should be exactly 64 bytes");

void process_tick(const MDT &tick) {
    // prefetch next tick if streaming
    _mm_prefetch(reinterpret_cast<const char *>(&tick) + 64, _MM_HINT_T0);
    std::cout << "Symbol : " << tick.symbol << " Price :" << tick.last_price
              << "\n";
}

int main() {
    // Create and initialize a MarketDataTick with example data
    MDT tick{};

    tick.timestamp_ns = 1624378291000000000ULL; // Example timestamp in nanoseconds
    tick.last_price = 15123.50;
    tick.bid_price = 15123.00;
    tick.ask_price = 15124.00;
    tick.bid_size = 100;
    tick.ask_size = 120;

    // Copy symbol into the fixed-size char array
    std::memcpy(tick.symbol, "BTCUSD",6); // Only 6 chars, null-terminator not required
    // Note: Avoid std::string — using char[8] ensures predictable, packed
    // memory layout

    // Process the tick (which prints it and prefetches the next one)
    process_tick(tick);

    return 0;
}
