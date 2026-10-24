#include "OrderBook.h"

void OrderBook::add_event(const MarketEvent& event) {
    if(event.side == 'B') {
        bids[event.price_ticks] += event.quantity;
    }else  if(event.side == 'S' ){
        asks[event.price_ticks] += event.quantity;
    }
}