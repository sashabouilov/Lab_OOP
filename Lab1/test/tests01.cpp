#include <gtest/gtest.h>
#include "../include/closest_pair_tonum.h"

TEST(test_01, basic_test_set){
    std::vector<int> ret_vec, vec{5, 4};
    ret_vec = closest_pair_tonum(10);
    ASSERT_EQ(ret_vec, vec);
}

TEST(test_02, basic_test_set){
    std::vector<int> ret_vec, vec{29, 20};
    ret_vec = closest_pair_tonum(30);
    ASSERT_EQ(ret_vec, vec);
}

TEST(test_03, basic_test_set){
    std::vector<int> ret_vec, vec{45, 36};
    ret_vec = closest_pair_tonum(50);
    ASSERT_EQ(ret_vec, vec);
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}





