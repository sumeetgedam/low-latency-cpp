#pragma once

#include <cstdint>
#include <string>

// Represents one row of market data
struct MarketEvent {
    std::uint64_t timestamp_ns;
    std::string symbol;
    char side;
    int price_ticks;
    int quantity;
}