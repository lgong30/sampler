//
// Created by longgong on 3/24/17.
//

#include "../static_tree/static_tree.hpp"
#include <iostream>

using namespace sds;

int main()
{
    int n = 8;
    static_tree<int> st(n);
    std::cout << "Create a tree with size: " << n << "\n";
    std::cout << "Test level traversal: \n";

    for(static_tree<int>::level_iterator it=st.begin();it != st.end();++ it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

}

