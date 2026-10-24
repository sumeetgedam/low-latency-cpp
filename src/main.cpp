#include "CsvReader.h"
#include "OrderBook.h"

#include<iostream>
#include<exception>
#include <unordered_map>

int main() {
  
  try {
    const auto events = read_market_events("data/sample_ticks.csv");

    std::unordered_map<std::string, OrderBook> books;

    for(const MarketEvent& event : events) {
      books[event.symbol].add_event(event);
    }

    for(const auto& [symbol, book] : books ) {
      std::cout << symbol;

      if(!book.bids.empty() && !book.asks.empty()) {
        const int best_bid = book.bids.begin()->first;
        const int best_ask = book.asks.begin()->first;

        std::cout << " best bid = " << best_bid
                  << " best ask = " << best_ask
                  << " spread = " << best_ask - best_bid;
      }else {
        std::cout << " missing bid or ask";
      }

      std::cout << '\n';
    }
  }catch (const std::exception& error) {
    std::cerr << error.what() << '\n';
    return 1;
  }

  return 0;
}
