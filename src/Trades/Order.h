#pragma once
#include <string>

enum class OrderType { Market, Limit };
enum class OrderSide { Buy, Sell };

class Order {
public:
    Order(const std::string& symbol, OrderType type, OrderSide side, double quantity, double price = 0.0);

    std::string getSymbol() const { return m_symbol; }
    OrderType getType() const { return m_type; }
    OrderSide getSide() const { return m_side; }
    double getQuantity() const { return m_quantity; }
    double getPrice() const { return m_price; }

private:
    std::string m_symbol;
    OrderType m_type;
    OrderSide m_side;
    double m_quantity;
    double m_price;
};