#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include "figure.h"
#include "rhombus.h"
#include "pentagon.h"
#include "hexagon.h"
#include "figure_array.h"

TEST(PointTest, EqualityOperator) {
    Point p1(1.0, 2.0);
    Point p2(1.0, 2.0);
    Point p3(1.0, 2.1);
    
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}

TEST(RhombusTest, ConstructorAndCenter) {
    Rhombus rhombus(Point(0, 0), 4, 6);
    Point center = rhombus.center();
    
    EXPECT_NEAR(center.x, 0.0, 1e-9);
    EXPECT_NEAR(center.y, 0.0, 1e-9);
}

TEST(RhombusTest, AreaCalculation) {
    Rhombus rhombus(Point(0, 0), 4, 6);
    double area = rhombus.area();
    
    EXPECT_NEAR(area, 12.0, 1e-9); // (4 * 6) / 2 = 12
}

TEST(RhombusTest, CopyConstructor) {
    Rhombus rhombus1(Point(0, 0), 4, 6);
    Rhombus rhombus2(rhombus1);
    
    EXPECT_TRUE(rhombus1 == rhombus2);
}

TEST(RhombusTest, MoveConstructor) {
    Rhombus rhombus1(Point(0, 0), 4, 6);
    Rhombus rhombus2(std::move(rhombus1));
    
    Point center = rhombus2.center();
    EXPECT_NEAR(center.x, 0.0, 1e-9);
    EXPECT_NEAR(center.y, 0.0, 1e-9);
}

TEST(RhombusTest, AssignmentOperator) {
    Rhombus rhombus1(Point(0, 0), 4, 6);
    Rhombus rhombus2;
    rhombus2 = rhombus1;
    
    EXPECT_TRUE(rhombus1 == rhombus2);
}

TEST(RhombusTest, InputOutput) {
    Rhombus rhombus(Point(0, 0), 4, 6);
    
    std::stringstream ss;
    ss << rhombus;
    
    EXPECT_FALSE(ss.str().empty());
    EXPECT_NE(ss.str().find("Rhombus"), std::string::npos);
}

TEST(RhombusTest, CloneMethod) {
    Rhombus rhombus(Point(0, 0), 4, 6);
    auto cloned = rhombus.clone();
    
    EXPECT_NE(cloned, nullptr);
    EXPECT_TRUE(*cloned == rhombus);
}

TEST(PentagonTest, ConstructorAndCenter) {
    Pentagon pentagon(Point(0, 0), 5);
    Point center = pentagon.center();
    
    EXPECT_NEAR(center.x, 0.0, 1e-9);
    EXPECT_NEAR(center.y, 0.0, 1e-9);
}

TEST(PentagonTest, AreaCalculation) {
    Pentagon pentagon(Point(0, 0), 1.0);
    double area = pentagon.area();
    EXPECT_GT(area, 0);
}

TEST(PentagonTest, CopyConstructor) {
    Pentagon pentagon1(Point(0, 0), 5);
    Pentagon pentagon2(pentagon1);
    
    EXPECT_TRUE(pentagon1 == pentagon2);
}

TEST(PentagonTest, InputOutput) {
    Pentagon pentagon(Point(0, 0), 5);
    
    std::stringstream ss;
    ss << pentagon;
    
    EXPECT_FALSE(ss.str().empty());
    EXPECT_NE(ss.str().find("Pentagon"), std::string::npos);
}

TEST(HexagonTest, ConstructorAndCenter) {
    Hexagon hexagon(Point(0, 0), 5);
    Point center = hexagon.center();
    
    EXPECT_NEAR(center.x, 0.0, 1e-9);
    EXPECT_NEAR(center.y, 0.0, 1e-9);
}

TEST(HexagonTest, AreaCalculation) {
    Hexagon hexagon(Point(0, 0), 1.0);
    double area = hexagon.area();
    EXPECT_GT(area, 0);
}

TEST(HexagonTest, CopyConstructor) {
    Hexagon hexagon1(Point(0, 0), 5);
    Hexagon hexagon2(hexagon1);
    
    EXPECT_TRUE(hexagon1 == hexagon2);
}

TEST(HexagonTest, InputOutput) {
    Hexagon hexagon(Point(0, 0), 5);
    
    std::stringstream ss;
    ss << hexagon;
    
    EXPECT_FALSE(ss.str().empty());
    EXPECT_NE(ss.str().find("Hexagon"), std::string::npos);
}

TEST(FigureArrayTest, AddAndTotalArea) {
    FigureArray array;
    
    auto rhombus = std::make_shared<Rhombus>(Point(0, 0), 4, 6);
    auto pentagon = std::make_shared<Pentagon>(Point(0, 0), 5);
    
    array.add(rhombus);
    array.add(pentagon);
    
    EXPECT_EQ(array.size(), 2);
    
    double total_area = array.total_area();
    EXPECT_GT(total_area, 0);
}

TEST(FigureArrayTest, RemoveFigure) {
    FigureArray array;
    
    auto rhombus = std::make_shared<Rhombus>(Point(0, 0), 4, 6);
    array.add(rhombus);
    
    EXPECT_EQ(array.size(), 1);
    array.remove(0);
    EXPECT_EQ(array.size(), 0);
}

TEST(FigureArrayTest, RemoveInvalidIndex) {
    FigureArray array;
    
    EXPECT_NO_THROW(array.remove(0));
    EXPECT_NO_THROW(array.remove(10));
}

TEST(FigureArrayTest, GetFigure) {
    FigureArray array;
    
    auto rhombus = std::make_shared<Rhombus>(Point(0, 0), 4, 6);
    array.add(rhombus);
    
    auto retrieved = array.get(0);
    EXPECT_NE(retrieved, nullptr);
    
    auto invalid = array.get(10);
    EXPECT_EQ(invalid, nullptr);
}

TEST(PolymorphismTest, FigurePointerOperations) {
    std::shared_ptr<Figure> rhombus = std::make_shared<Rhombus>(Point(0, 0), 4, 6);
    std::shared_ptr<Figure> pentagon = std::make_shared<Pentagon>(Point(0, 0), 5);
    
    EXPECT_GT(rhombus->area(), 0);
    EXPECT_GT(pentagon->area(), 0);
    
    Point center1 = rhombus->center();
    Point center2 = pentagon->center();
    
    EXPECT_NEAR(center1.x, 0.0, 1e-9);
    EXPECT_NEAR(center1.y, 0.0, 1e-9);
    EXPECT_NEAR(center2.x, 0.0, 1e-9);
    EXPECT_NEAR(center2.y, 0.0, 1e-9);
}

TEST(PolymorphismTest, DoubleConversion) {
    std::shared_ptr<Figure> rhombus = std::make_shared<Rhombus>(Point(0, 0), 4, 6);
    double area1 = rhombus->area();
    double area2 = static_cast<double>(*rhombus);
    
    EXPECT_NEAR(area1, area2, 1e-9);
}

TEST(InputTest, RhombusInput) {
    Rhombus rhombus;
    std::stringstream ss("0 3 2 0 0 -3 -2 0");
    
    EXPECT_NO_THROW(ss >> rhombus);
    
    Point center = rhombus.center();
    EXPECT_NEAR(center.x, 0.0, 1e-9);
    EXPECT_NEAR(center.y, 0.0, 1e-9);
}

TEST(InputTest, StreamOperators) {
    Rhombus rhombus(Point(0, 0), 4, 6);
    std::stringstream ss;
    
    EXPECT_NO_THROW(ss << rhombus);
    EXPECT_FALSE(ss.str().empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}