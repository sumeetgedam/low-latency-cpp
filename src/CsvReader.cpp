#include "CsvReader.h"

#include<fstream>
#include<sstream>
#include<stdexcept>

std::vector<MarketEvent> read_market_events(const std::string& filename) {
    std::ifstream file(filename);

    if(!file.is_open()) {
        throw std::runtime_error("Could not open file : " + filename);
    }

    std::vector<MarketEvent> events;
    std::string line;

    std::getline(file, line); // skip th CSV header

    while(std::getline(file, line)) {
        std::stringstream row(line);
        std::string timestamp, symbol, side, price, quantity;

        if(!std::getline(row, timestamp ,',')
            || !std::getline(row, symbol, ',')
            || !std::getline(row, side, ',')
            || !std::getline(row, price, ',')
            || !std::getline(row, quantity, ',')
        ) {
            throw std::runtime_error("Malformed CSV row : " + line);
        }

        if(side.empty()) {
            throw std::runtime_error("Missing side in CSV row : " + line);
        }

        events.push_back({
            std::stoull(timestamp),
            symbol,
            side[0],
            std::stoi(price),
            std::stoi(quantity)
        });
    }

    return events;

}