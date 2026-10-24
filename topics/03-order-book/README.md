# Order Book: Price-level aggregation

## Concepts
- An order book stores bids and asks by price
- `std::map` keeps price levels sorted
- `std::greater<int>` sorts bids from highest to lowest
- `books[symbol]` creates a default `OrderBook` if that symbol is new.

## Interview questions
1. Why do bids and asks use different sort orders ?
2. What does `map[key] += value` do when the key is new ?
3. What market events would be needed to support cancellation ?

