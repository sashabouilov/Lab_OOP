#include <iostream>
#include "include/closest_pair_tonum.h"

int main(){
    int limit;
    std::vector<int> vector;
    std::cout << "Enter the upper bound ";
    std::cin >> limit;

    std::cout << "Result: ";
    vector = closest_pair_tonum(limit);
    std::cout << vector[0] << " " << vector[1] << std::endl;

    return 0;
}

