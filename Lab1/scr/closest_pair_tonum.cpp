#include "../include/closest_pair_tonum.h"


bool isPerfectSquare(int num) {
    int root = static_cast<int>(sqrt(num));
    return root * root == num;
}

std::vector<int> closest_pair_tonum(int upper_limit)
{
    std::vector<int> ans{};
    for (int m = upper_limit - 1; m > 0; m--) {

        for (int n = m - 1; n > 0; n--) {
            int sum = m + n;
            int diff = m - n;
            
            if (isPerfectSquare(sum) && isPerfectSquare(diff)) {
                ans.push_back(m);
                ans.push_back(n);
                return ans;
            }
        }
    }
    return {};
}