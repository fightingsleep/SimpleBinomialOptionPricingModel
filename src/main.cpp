#include <memory>
#include <iostream>
#include "binomial_model.hpp"

int main(int argc, char *argv[])
{
    auto model = std::make_shared<BinomialModel>();
    model->Initialize(50, 2, 0.174012, 2);
    std::cout << model->PriceOption(52, 0.05, PUT) << std::endl;
}
