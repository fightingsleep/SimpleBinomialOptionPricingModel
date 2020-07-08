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
        OptionType option_type = CALL,
        bool print_tree = false);

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
        double up_factor,
        double down_factor,
        double option_value_up,
        double option_value_down,
        double term);
};

#endif // INCLUDE_BINOMIAL_MODEL_HPP_
