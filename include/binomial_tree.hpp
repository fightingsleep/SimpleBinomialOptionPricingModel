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
class BinomialTree
{
 public:
    double delta_t = 0;
    double up_factor = 0;
    double down_factor = 0;
    double spot_price = 0;

    // Build the tree structure and set stock prices at each node
    void Build(
        double spot_price,
        double up_factor,
        double down_factor,
        int time_steps);

    // Print the tree for debugging
    void Print();

    TimeStep& operator[](int);
    size_t size();
 private:
    std::vector<TimeStep> tree_;
};

#endif // INCLUDE_BINOMIAL_TREE_HPP_
