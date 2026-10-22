#pragma once

#include "MarketEvent.h"
#include<string>
#include<vector>

// Reads events from a CSV and returns them as a list.
std::vector<MarketEvent> read_market_events(const std::string& file);