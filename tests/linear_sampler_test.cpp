//
// Created by Long Gong on 3/28/17.
//

#include <iostream>
#include "../sampler/samplers.hpp"


void all_zero_test()
{
    std::cout << "Create an all-zero linear sampler" << std::endl;
    int n = 8;
    LinearSampler<int> ls(n);

    for (std::size_t i = 0;i < ls.size(); ++ i) {
        std::cout << ls[i]
                  << " ";
    }
    std::cout << std::endl;



}

void update_test()
{
    std::cout << "Create a linear sampler from a vector" << std::endl;
    int n = 8;
    LinearSampler<int> ls(std::vector<int>(n, 2));

    std::cout << "Initial elements: " << std::endl;
    for (std::size_t i = 0;i < ls.size(); ++ i) {
        std::cout << ls[i]
                  << " ";

    }
    std::cout << "\n";

    int w = 5;
    std::cout << "Sample one with value: "
              << w
              << "\nSample result: ";
    std::cout << ls.sample(w)
              << "\n";

    // wrapper test
    std::cout << "wrapper test ...\n";
    int x = ls[0];

    int y = ls[0] + 10;
    std::cout << x
              << " "
              << y
              << std::endl;

    ls[0] = 10;
    std::cout << ls[0]
              << "\n";
    std::cout << "Sample one with value: "
              << w
              << "\nSample result: ";
    std::cout << ls.sample(w)
              << "\n";


}


int main()
{

    update_test();
    return 0;
}