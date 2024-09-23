#include "Order.h"

Order::Order(const std::string& symbol, OrderType type, OrderSide side, double quantity, double price)
    : m_symbol(symbol), m_type(type), m_side(side), m_quantity(quantity), m_price(price) {}