#include <gtest/gtest.h>
#include "../src/lab5.cpp" 

TEST(ArrayTest, Constructor) {
    Allocator mem(1024);
    Array<int> arr(&mem);

    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.currency_capacity(), 5); 


TEST(ArrayTest, PushBackBasic) {
    Allocator mem(1024);
    Array<int> arr(&mem);

    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.currency_capacity(), 5); 
}


TEST(ArrayTest, CapacityGrow) {
    Allocator mem(1024);
    Array<int> arr(&mem);

    for (int i = 0; i < 6; ++i)
        arr.push_back(i);

    EXPECT_EQ(arr.size(), 6);
    EXPECT_GT(arr.currency_capacity(), 5); 
}


TEST(ArrayTest, PopNormal) {
    Allocator mem(1024);
    Array<int> arr(&mem);

    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);

    arr.pop(1);  

    EXPECT_EQ(arr.size(), 2);

    auto it = arr.begin();
    EXPECT_EQ(*it, 10);
    ++it;
    EXPECT_EQ(*it, 30);
}

TEST(ArrayTest, PopOutOfRange) {
    Allocator mem(1024);
    Array<int> arr(&mem);

    arr.push_back(10);
    EXPECT_THROW(arr.pop(1), std::out_of_range);
}


TEST(ArrayTest, IteratorWorks) {
    Allocator mem(1024);
    Array<int> arr(&mem);

    arr.push_back(5);
    arr.push_back(10);
    arr.push_back(15);

    int sum = 0;
    for (auto x : {5, 10, 15}) {} 

    auto it = arr.begin();
    EXPECT_EQ(*it, 5);  ++it;
    EXPECT_EQ(*it, 10); ++it;
    EXPECT_EQ(*it, 15);
}


TEST(ArrayTest, OutputStream) {
    Allocator mem(1024);
    Array<int> arr(&mem);

    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);

    std::stringstream ss;
    ss << arr;

    std::string expected = "[1, 2, 3] - динамический массив, используем аллокатор - Allocator";
    EXPECT_EQ(ss.str(), expected);
}


struct MyStruct {
    int a;
    double b;
};

TEST(ArrayTest, ComplexType) {
    Allocator mem(1024);
    Array<MyStruct> arr(&mem);

    arr.push_back({1, 3.14});
    arr.push_back({2, 2.71});

    EXPECT_EQ(arr.size(), 2);
    auto it = arr.begin();
    EXPECT_EQ(it->a, 1);
    EXPECT_NEAR(it->b, 3.14, 1e-9);

    ++it;
    EXPECT_EQ(it->a, 2);
    EXPECT_NEAR(it->b, 2.71, 1e-9);
}
