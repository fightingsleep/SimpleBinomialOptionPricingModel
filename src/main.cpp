#include <memory>
#include <iostream>
#include "binomial_model.hpp"

int main(int argc, char *argv[])
{
    double spot_price, strike_price, years_until_expiry, volatility, risk_free_rate;
    int time_steps, option_type, option_style;
    bool print_tree = false;
    std::cout << "Option style (0 = European, 1 = American): ";
    std::cin >> option_style;
    std::cout << "Stock spot price (ex: 20): ";
    std::cin >> spot_price;
    std::cout << "Option strike price (ex: 21): ";
    std::cin >> strike_price;
    std::cout << "Option term in years (ex: 0.5): ";
    std::cin >> years_until_expiry;
    std::cout << "Option volatility (ex: 0.1): ";
    std::cin >> volatility;
    std::cout << "Risk free interest rate (ex: 0.05): ";
    std::cin >> risk_free_rate;
    std::cout << "Number of time steps (ex: 2): ";
    std::cin >> time_steps;
    std::cout << "Option type (0 = CALL, 1 = PUT): ";
    std::cin >> option_type;
    std::cout << "Print tree? (0 = NO, 1 = YES): ";
    std::cin >> print_tree;

    auto model = std::make_shared<BinomialModel>();
    model->Initialize(spot_price, years_until_expiry, volatility, time_steps);
    double option_value = model->PriceOption(
        strike_price,
        risk_free_rate,
        static_cast<OptionType>(option_type), 
        static_cast<OptionStyle>(option_style),
        print_tree);
    std::cout << "Option value: " << option_value << std::endl;
}
