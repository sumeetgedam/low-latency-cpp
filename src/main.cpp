#include<cstdint>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<unordered_map>
#include<functional>
#include<map>

#include<iostream>
#include<exception>
struct MarketEvent {
  std::uint64_t timestamp_ns;
  std::string symbol;
  char side;
  int price_ticks;
  int quantity;
};

struct BestPrices {
  bool has_bid = false;
  bool has_ask = false;
  int best_bid = 0;
  int best_ask = 0;
};


struct OrderBook {
  // Highest bid first
  std::map<int, int, std::greater<int>> bids;
  
  // Lowest ask first
  std::map<int, int> asks;
};

int main() {

    std::cout << "Market data replay starting\n";
    std::ifstream file("data/sample_ticks.csv");

    if(!file.is_open()){
      std::cerr << "Could not open data/simple_ticks.csv\n";
      return 1;
    }
    
    std::string line;
    std::getline(file, line); // skip the header row

    std::vector<MarketEvent> events;
    while(std::getline(file, line)) {
      std::stringstream row(line);

      std::string timestamp, symbol, side, price, quantity;

      if(
        !std::getline(row, timestamp, ',') || 
        !std::getline(row, symbol, ',') ||
        !std::getline(row, side, ',') ||
        !std::getline(row, price, ',') ||
        !std::getline(row, quantity, ',')
        ) {
        std::cerr << "Skipping malformed row : " << line << '\n';
        continue;
      }

      try{
        MarketEvent event {
          std::stoull(timestamp),
          symbol,
          side.at(0),
          std::stoi(price),
          std::stoi(quantity)
        };
        events.push_back(event);
        std::cout << event.symbol << ' ' << event.side << " price = " << event.price_ticks << " quantity = " << event.quantity << '\n';
      }catch(const std::exception& error) {
        std::cerr << "Skipping invalid row : " << line << " (" << error.what() << ")\n";
      }

      std::cout << symbol << " " << side << " price = " << price << " quantity = " << quantity << '\n';
    }

    std::unordered_map<std::string, int> quantity_by_symbol;

    int total_quanity = 0;
    for(const MarketEvent& event : events) {
      total_quanity += event.quantity;
      quantity_by_symbol[event.symbol] += event.quantity;
    }

    for(const auto& entry : quantity_by_symbol) {
      std::cout << entry.first << " : " << entry.second << '\n';
    }

    std::cout << "Events loaded : " << events.size() << '\n';
    std::cout << "Total quantity : " << total_quanity << '\n';

    // std::unordered_map<std::string, BestPrices> book;

    // for(const MarketEvent& event : events) {
    //   BestPrices& prices = book[event.symbol];

    //   if(event.side == 'B' &&
    //     (!prices.has_bid || event.price_ticks > prices.best_bid)) {
    //       prices.best_bid = event.price_ticks;
    //       prices.has_bid = true;
    //     }else if(
    //       event.side == 'S' &&
    //       (!prices.has_ask || event.price_ticks < prices.best_ask)
    //     ) {
    //       prices.best_ask = event.price_ticks;
    //       prices.has_ask = true;
    //     }
    // }


    // for(const auto& entry : book) {
    //   std::cout << entry.first;

    //   if(entry.second.has_bid)
    //     std::cout << " best bid = " << entry.second.best_bid;

    //   if(entry.second.has_ask) 
    //     std::cout << " best ask = " << entry.second.best_ask;

    //   std::cout << '\n';
    // }
    
    std::unordered_map<std::string, OrderBook> books;

    for(const MarketEvent& event : events) {
      OrderBook& orderBook = books[event.symbol];

      if(event.side == 'B') {
        orderBook.bids[event.price_ticks] += event.quantity;

      }else if(event.side == 'S') {
        orderBook.asks[event.price_ticks] += event.quantity;
      }
    }

    for(const auto& [symbol, book] : books) {
      std::cout << symbol << '\n';

      for(const auto& [price, quantity] : book.bids) {
        std::cout << " bid " << price << " x " << quantity << '\n';
      }

      for(const auto& [price, quantity] : book.asks) {
        std::cout << " ask " << price << " x " << quantity << '\n';
      }
    }

    for(const auto& [symbol, book] : books) {
      std::cout << symbol;

      if(!book.bids.empty() && !book.asks.empty()) {
        const int best_bid = book.bids.begin()->first;
        const int best_ask = book.asks.begin()->first;
        const int spread = best_ask - best_bid;

        std::cout << " best bid = " << best_bid
                  << " best ask = " << best_ask
                  << " spread = " << spread;
      }else {
        std::cout << " missing bid or ask";
      }
      std::cout << '\n';
    }

    return 0;
}
