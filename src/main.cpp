#include <memory>
#include <iostream>
#include "binomial_model.hpp"

int main(int argc, char *argv[])
{
    double spot_price = 0.0, strike_price = 0.0, years_until_expiry = 0.0,
        volatility = 0.0, risk_free_rate = 0.0, dividend_yield = 0.0;
    int time_steps = 0, option_type = 0, option_style = 0;
    bool output_debug_info = false;

    // Read all the user input
    std::cout << "Option style (0 = European, 1 = American): ";
    std::cin >> option_style;
    std::cout << "Stock spot price (ex: 20): ";
    std::cin >> spot_price;
    std::cout << "Stock dividend yield (ex: 0.02): ";
    std::cin >> dividend_yield;
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
    std::cout << "Print debugging information? (0 = NO, 1 = YES): ";
    std::cin >> output_debug_info;
    std::cout << std::endl;

    // Initialize the model
    auto model = std::make_shared<BinomialModel>();
    model->Initialize(spot_price, years_until_expiry, volatility, time_steps);

    // Price the option
    double option_value = model->PriceOption(
        strike_price,
        risk_free_rate,
        dividend_yield,
        static_cast<OptionType>(option_type), 
        static_cast<OptionStyle>(option_style),
        output_debug_info);
    std::cout << "Option value: " << option_value << std::endl << std::endl;
}
