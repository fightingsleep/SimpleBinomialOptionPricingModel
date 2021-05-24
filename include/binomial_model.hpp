#ifndef INCLUDE_BINOMIAL_MODEL_HPP_
#define INCLUDE_BINOMIAL_MODEL_HPP_

#include "binomial_tree.hpp"
#include "option_type.hpp"

class BinomialModel
{
 public:
    // Initializes the binomial option pricing model by constructing a tree based
    // on the given arguments.
    void Initialize(
        double spot_price,
        double years_until_expiry,
        double volatility,
        int time_steps);

    // Price an option using backward induction on the tree
    double PriceOption(
        double strike_price,
        double risk_free_rate,
        double dividend_yield,
        OptionType option_type = OptionType::CALL,
        OptionStyle option_style = OptionStyle::EUROPEAN,
        bool output_debug_info = false);

 protected:
    // The internal binomial tree
    BinomialTree tree_;

    // Returns the value of the option by constructing a risk-free portfolio
    // containing the underlying stock and a short option. The option value is
    // calculated by making a no-arbitrage argument.
    double ValueRiskFreePortfolio(
        double spot_price,
        double strike_price,
        double risk_free_rate,
        double dividend_yield,
        double up_factor,
        double down_factor,
        double option_value_up,
        double option_value_down,
        double term);

    // Returns the value of the option by using Hull's "probability" equation.
    // This is mathematically equivalent to the method above. I just added it
    // for educational purposes and to prove that they are the same.
    double CalculateOptionPrice(
        double spot_price,
        double strike_price,
        double risk_free_rate,
        double dividend_yield,
        double up_factor,
        double down_factor,
        double option_value_up,
        double option_value_down,
        double term);

 private:
    double epsilon = 0.0000001;
};

#endif // INCLUDE_BINOMIAL_MODEL_HPP_

