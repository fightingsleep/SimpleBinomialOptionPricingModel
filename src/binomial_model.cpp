#include "binomial_model.hpp"
#include <cmath>
#include <algorithm>

void BinomialModel::Initialize(
    double spot_price,
    double years_until_expiry,
    double volatility,
    int time_steps)
{
    double delta_t = years_until_expiry / time_steps;
    double up_factor = std::exp(volatility * std::sqrt(delta_t));
    double down_factor = std::exp(-volatility * std::sqrt(delta_t));

    // Store this information in the tree
    tree_.spot_price = spot_price;
    tree_.delta_t = delta_t;
    tree_.up_factor = up_factor;
    tree_.down_factor = down_factor;

    // Build the tree structure
    tree_.Build(spot_price, up_factor, down_factor, time_steps);
}

double BinomialModel::PriceOption(
    double strike_price,
    double risk_free_rate,
    OptionType option_type)
{
    // Fill in the instrinsic option value at expiry
    int last_index = tree_.size() - 1;
    for (size_t i = 0; i < tree_[last_index].nodes.size(); i++)
    {
        if (option_type == CALL)
        {
            tree_[last_index].nodes[i].option_value = std::max(tree_[last_index].nodes[i].stock_price - strike_price, 0.0);
        }
        else
        {
            tree_[last_index].nodes[i].option_value = std::max(strike_price - tree_[last_index].nodes[i].stock_price, 0.0);
        }
    }

    // Traverse the tree in reverse and calculate the option_value at the remaining nodes
    // by making a series of no-arbitrage arguments
    for (int i = last_index - 1; i >= 0; i--)
    {
        for (size_t j = 0; j < tree_[i].nodes.size(); j++)
        {
            tree_[i].nodes[j].option_value = ValueRiskFreePortfolio(
                tree_.spot_price,
                strike_price,
                risk_free_rate,
                tree_.up_factor,
                tree_.down_factor,
                tree_[i + 1].nodes[j + 1].option_value,
                tree_[i + 1].nodes[j].option_value,
                tree_.delta_t);
        }
    }

    tree_.Print();

    return tree_[0].nodes[0].option_value;
}

double BinomialModel::ValueRiskFreePortfolio(
    double spot_price,
    double strike_price,
    double risk_free_rate,
    double up_factor,
    double down_factor,
    double option_value_up,
    double option_value_down,
    double term)
{
    // Calculate the amount of stock required to construct a riskless portfolio
    double delta = (option_value_up - option_value_down) / (spot_price * (up_factor - down_factor));

    // Discount the value of the riskless portfolio back to today
    double discounted_value = (spot_price * up_factor * delta - option_value_up) * std::exp(-risk_free_rate * term);

    // Return the arbitrage-free option value
    // From formula: S0 * Delta - C = risk-free portfolio value
    return spot_price * delta - discounted_value;
}
