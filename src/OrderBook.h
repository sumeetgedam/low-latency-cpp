#pragma once

#include "MarketEvent.h"

#include <functional>
#include <map>

// Aggregates quantity by price for one symbol
struct OrderBook
{
    /* data */
    std::map<int, int , std::greater<int>> bids; // Highest price first
    std::map<int, int> asks;

    void add_event(const MarketEvent& event);
};
