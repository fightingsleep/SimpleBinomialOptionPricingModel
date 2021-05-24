#include "binomial_tree.hpp"
#include <iostream>
#include <cmath>

void BinomialTree::Build(
    double spot_price,
    double up_factor,
    double down_factor,
    int time_steps)
{
    // Create the general tree structure
    tree_.resize(time_steps + 1);
    int nodesPerStep = 1;
    for (auto &step : tree_)
    {
        step.nodes.resize(nodesPerStep++);
    }

    // Navigate the tree and fill in the stock prices.
    for (size_t i = 0; i < tree_.size(); i++)
    {
        for (size_t j = 0; j < tree_[i].nodes.size(); j++)
        {
            tree_[i].nodes[j].stock_price = spot_price * std::pow(up_factor, j) * std::pow(down_factor, i - j);
        }
    }
}

void BinomialTree::Print()
{
    int time_step = 0;
    for (auto const &step : tree_)
    {
        std::cout << "Time step " << time_step++ << ": ";
        for (auto const &node : step.nodes)
        {
            std::cout << " || Stock Price: " << node.stock_price << ", Option Value: " << node.option_value << " || ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

TimeStep& BinomialTree::operator[](int index)
{
    return tree_[index];
}

size_t BinomialTree::size()
{
    return tree_.size();
}
