#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct MarketEvent {
  std::uint64_t timestamp_ns;
  std::string symbol;
  char side;
  int price_ticks;
  int quantity;
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

    int total_quanity = 0;
    for(const MarketEvent& event : events) {
      total_quanity += event.quantity;
    }

    std::cout << "Events loaded : " << events.size() << '\n';
    std::cout << "Total quantity : " << total_quanity << '\n';

    return 0;
}
