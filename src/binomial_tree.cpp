#include "binomial_tree.hpp"
#include <iostream>

void BinomialTree::Build(int time_steps)
{
    resize(time_steps + 1);
    int nodesPerStep = 1;
    for (auto it = this->begin(); it != this->end(); it++)
    {
        it->nodes.resize(nodesPerStep++);
    }
}

void BinomialTree::Print()
{
    int time_step = 0;
    for (auto it = this->begin(); it != this->end(); it++)
    {
        std::cout << "Time step " << time_step++ << ": ";
        for (auto const &node : it->nodes)
        {
            std::cout << " || Stock Price: " << node.stock_price << ", Option Value: " << node.option_value << " || ";
        }
        std::cout << std::endl;
    }
}