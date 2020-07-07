#ifndef INCLUDE_BINOMIAL_TREE_HPP_
#define INCLUDE_BINOMIAL_TREE_HPP_

#include <vector>

struct Node
{
    double stock_price;
    double option_value;
};

struct TimeStep
{
    std::vector<Node> nodes;
};

// The binomial tree in this implementation is just a vector of vectors.
// The first vector represents each time step in the tree, and the inner
// vectors represent the nodes at each time step.
class BinomialTree : public std::vector<TimeStep>
{
public:
    double delta_t;
    double up_factor;
    double down_factor;
    double spot_price;
    void Build(int time_steps);
    void Print();
};

#endif // INCLUDE_BINOMIAL_TREE_HPP_