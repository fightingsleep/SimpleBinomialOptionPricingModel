#include "binomial_model.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

void BinomialModel::Initialize(
    double spot_price,
    double years_until_expiry,
    double volatility,
    int time_steps)
{
    double delta_t = years_until_expiry / time_steps;
    // The up and down factors are calculated using the Cox, Ross, & Rubinstein (CRR)
    // method to guarantee that the tree is recombinant.
    double up_factor = std::exp(volatility * std::sqrt(delta_t));
    double down_factor = std::exp(-volatility * std::sqrt(delta_t));

    // Store this information in the tree
    tree_.spot_price = spot_price;
    tree_.delta_t = delta_t;
    tree_.up_factor = up_factor;
    tree_.down_factor = down_factor;

    // Build the tree structure and set the stock prices at each node
    tree_.Build(spot_price, up_factor, down_factor, time_steps);
}

double BinomialModel::PriceOption(
    double strike_price,
    double risk_free_rate,
    double dividend_yield,
    OptionType option_type,
    OptionStyle option_style,
    bool output_debug_info)
{
    // Fill in the instrinsic option value at expiry
    int last_index = tree_.size() - 1;
    for (size_t i = 0; i < tree_[last_index].nodes.size(); i++)
    {
        if (option_type == OptionType::CALL)
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
                dividend_yield,
                tree_.up_factor,
                tree_.down_factor,
                tree_[i + 1].nodes[j + 1].option_value,
                tree_[i + 1].nodes[j].option_value,
                tree_.delta_t);

            if (output_debug_info)
            {
                // Compare our two option pricing equations to make sure they are equal
                auto altOptionVal = CalculateOptionPrice(
                    tree_.spot_price,
                    strike_price,
                    risk_free_rate,
                    dividend_yield,
                    tree_.up_factor,
                    tree_.down_factor,
                    tree_[i + 1].nodes[j + 1].option_value,
                    tree_[i + 1].nodes[j].option_value,
                    tree_.delta_t);
                if (std::abs(tree_[i].nodes[j].option_value - altOptionVal) > epsilon)
                {
                    std::cout << "** Option price mismatch detected **" << std::endl;
                }
            }

            // In the case of american style options, sometimes the value of exercising the option
            // early is greater than the option value calculated above.
            if (option_style == OptionStyle::AMERICAN)
            {
                double current_stock_price = tree_[i].nodes[j].stock_price;
                double early_exercise_value = option_type == OptionType::CALL ?
                    current_stock_price - strike_price : strike_price - current_stock_price;

                if (early_exercise_value > tree_[i].nodes[j].option_value)
                {
                    tree_[i].nodes[j].option_value = early_exercise_value;
                }
            }
        }
    }

    if (output_debug_info)
    {
        tree_.Print();
    }

    return tree_[0].nodes[0].option_value;
}

double BinomialModel::ValueRiskFreePortfolio(
    double spot_price,
    double strike_price,
    double risk_free_rate,
    double dividend_yield,
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
    if (dividend_yield == 0.0)
    {
        // From formula: S0 * Delta - C = risk-free portfolio discounted value
        return spot_price * delta - discounted_value;
    }
    else
    {
        // Hull never states the dividend adjustment in this format, but it is equivalent to his example
        return spot_price * delta * std::exp(-(dividend_yield * term)) - discounted_value;
    }
}

double BinomialModel::CalculateOptionPrice(
    double spot_price,
    double strike_price,
    double risk_free_rate,
    double dividend_yield,
    double up_factor,
    double down_factor,
    double option_value_up,
    double option_value_down,
    double term)
{
    // From formula: p = (e^(rT) - d) / (u - d)
    double p = (std::exp((risk_free_rate - dividend_yield) * term) - down_factor) / (up_factor - down_factor);

    // From formula: f = e^(-rT) * [p * fu + (1 - p) * fd]
    return std::exp(-risk_free_rate * term) * ((p * option_value_up) + ((1 - p) * option_value_down));
}
