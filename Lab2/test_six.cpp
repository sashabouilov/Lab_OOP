#include <gtest/gtest.h>
#include "Six.h"

TEST(SixTest, DefaultConstructor) {
    Six num;
    EXPECT_EQ(num.toString(), "0");
    EXPECT_EQ(num.size(), 1);
}

TEST(SixTest, StringConstructor) {
    Six num("12345");
    EXPECT_EQ(num.toString(), "12345");
    EXPECT_EQ(num.size(), 5);
}

TEST(SixTest, StringConstructorRemovesLeadingZeros) {
    Six num("000123");
    EXPECT_EQ(num.toString(), "123");
    EXPECT_EQ(num.size(), 3);
}

TEST(SixTest, SizeValueConstructor) {
    Six num(4, 3);
    EXPECT_EQ(num.toString(), "3333");
    EXPECT_EQ(num.size(), 4);
}

TEST(SixTest, CopyConstructor) {
    Six original("12345");
    Six copy(original);
    EXPECT_EQ(copy.toString(), "12345");
    EXPECT_EQ(copy.size(), 5);
}

TEST(SixTest, InvalidDigitThrows) {
    EXPECT_THROW(Six("123456"), std::invalid_argument);
    EXPECT_THROW(Six("12a45"), std::invalid_argument);
    EXPECT_THROW(Six(""), std::invalid_argument);
}

TEST(SixTest, InvalidSizeThrows) {
    EXPECT_THROW(Six(0, 1), std::invalid_argument);
}

TEST(SixTest, AdditionBasic) {
    Six a("123");
    Six b("321");
    Six result = a.add(b);
    EXPECT_EQ(result.toString(), "444");
}

TEST(SixTest, AdditionWithCarry) {
    Six a("5");
    Six b("1");
    Six result = a.add(b);
    EXPECT_EQ(result.toString(), "10");
}

TEST(SixTest, AdditionDifferentSizes) {
    Six a("555");
    Six b("1");
    Six result = a.add(b);
    EXPECT_EQ(result.toString(), "1000");
}

TEST(SixTest, AdditionWithZero) {
    Six a("12345");
    Six zero;
    Six result = a.add(zero);
    EXPECT_EQ(result.toString(), "12345");
}

TEST(SixTest, SubtractionBasic) {
    Six a("321");
    Six b("123");
    Six result = a.subtract(b);
    EXPECT_EQ(result.toString(), "154");
}

TEST(SixTest, SubtractionWithBorrow) {
    Six a("10");
    Six b("1");
    Six result = a.subtract(b);
    EXPECT_EQ(result.toString(), "5");
}

TEST(SixTest, SubtractionEqualNumbers) {
    Six a("12345");
    Six b("12345");
    Six result = a.subtract(b);
    EXPECT_EQ(result.toString(), "0");
}

TEST(SixTest, SubtractionThrowsOnNegative) {
    Six a("10");
    Six b("20");
    EXPECT_THROW(a.subtract(b), std::underflow_error);
}

TEST(SixTest, Equality) {
    Six a("12345");
    Six b("12345");
    Six c("54321");
    
    EXPECT_TRUE(a.equals(b));
    EXPECT_FALSE(a.equals(c));
}

TEST(SixTest, GreaterThan) {
    Six a("54321");
    Six b("12345");
    Six c("12345");
    
    EXPECT_TRUE(a.greaterThan(b));
    EXPECT_FALSE(b.greaterThan(a));
    EXPECT_FALSE(b.greaterThan(c));
}

TEST(SixTest, LessThan) {
    Six a("12345");
    Six b("54321");
    Six c("12345");
    
    EXPECT_TRUE(a.lessThan(b));
    EXPECT_FALSE(b.lessThan(a));
    EXPECT_FALSE(a.lessThan(c));
}

TEST(SixTest, ComparisonDifferentSizes) {
    Six a("1000");
    Six b("555");
    
    EXPECT_TRUE(a.greaterThan(b));
    EXPECT_TRUE(b.lessThan(a));
}

TEST(SixTest, CopyMethod) {
    Six original("12345");
    Six copy = original.copy();
    
    EXPECT_EQ(original.toString(), copy.toString());
    EXPECT_EQ(original.size(), copy.size());

    Six modified = original.add(Six("1"));
    EXPECT_EQ(original.toString(), "12345");
    EXPECT_EQ(copy.toString(), "12345");
}

TEST(SixTest, Immutability) {
    Six original("12345");
    Six result1 = original.add(Six("1"));
    Six result2 = original.subtract(Six("1"));
    
    EXPECT_EQ(original.toString(), "12345");
    EXPECT_EQ(result1.toString(), "12350");
    EXPECT_EQ(result2.toString(), "12344");
}

TEST(SixTest, ComplexOperations) {
    Six a("12345");
    Six b("11111");
    Six c("22222");

    Six temp = a.add(b);
    Six result = temp.subtract(c);
    
    EXPECT_EQ(result.toString(), "1234");
}

TEST(SixTest, LargeNumbers) {
    Six a("55555");
    Six b("1");
    Six result = a.add(b);
    EXPECT_EQ(result.toString(), "100000");
}

TEST(SixTest, MaximumDigits) {
    Six max1("5");
    Six max2("5");
    Six result = max1.add(max2);
    EXPECT_EQ(result.toString(), "14");
}

TEST(SixTest, SingleDigitOperations) {
    Six a("3");
    Six b("2");
    
    EXPECT_EQ(a.add(b).toString(), "5");
    EXPECT_EQ(a.subtract(b).toString(), "1");
    EXPECT_TRUE(a.greaterThan(b));
}

TEST(SixTest, MoveSemantics) {
    Six original("12345");
    Six moved(std::move(original));
    
    EXPECT_EQ(moved.toString(), "12345");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}