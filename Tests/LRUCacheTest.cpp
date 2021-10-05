//
// Created by nik-carlson on 10/2/21.
//
#include <gtest/gtest.h>
#include "../MyLRUCache/MyLRUCache.h"

template<typename T, typename V>
struct TestMyLRUCache : public ::testing::Test {
    MyLRUCache<T, V> cache;

    explicit TestMyLRUCache(size_t cache_capacity = 5) : cache(cache_capacity) {}
};

struct FirstIntTest : TestMyLRUCache<int32_t, int32_t> {
public:
    FirstIntTest() : TestMyLRUCache(5) {
    }
};

TEST_F(FirstIntTest, simple_has_elements_test) {
    cache.set(1, 1);
    cache.set(2, 2);
    cache.set(3, 3);
    cache.set(4, 4);
    cache.set(5, 5);
    for (int32_t i = 1; i <= 5; ++i) {
        auto res = cache.get(i);
        ASSERT_EQ(i, res.value_or(-1));
    }
}

TEST_F(FirstIntTest, order_test) {
    cache.set(1, 1);
    cache.set(2, 2);
    cache.set(4, 4);
    cache.set(2, 2);
    cache.set(3, 5);
    std::vector<std::pair<int32_t, int32_t>> order = {{3, 5},
                                                      {2, 2},
                                                      {4, 4},
                                                      {1, 1}};
    for (auto const &it: order) {
        auto res = cache.top();
        cache.erase(it.first);
        ASSERT_EQ(it.second, res.value_or(-1));
    }
}


struct SecondIntTest : TestMyLRUCache<int32_t, int32_t> {
    SecondIntTest() : TestMyLRUCache(7) {
    }
};

TEST_F(SecondIntTest, simple_has_elements_test) {
    cache.set(1, 1);
    cache.set(2, 2);
    cache.set(3, 3);
    cache.set(4, 4);
    cache.set(5, 5);
    cache.set(6, 6);
    cache.set(7, 7);
    for (int32_t i = 1; i <= 7; ++i) {
        auto res = cache.get(i);
        ASSERT_EQ(i, res.value_or(-1));
    }
}

TEST_F(SecondIntTest, order_test) {
    cache.set(1, 1);
    cache.set(2, 2);
    cache.set(3, 3);
    cache.set(1, 1);

    auto res = cache.top();
    ASSERT_EQ(3, res.value_or(-1));
    cache.set(4, 4);
    res = cache.top();
    ASSERT_EQ(4, res.value_or(-1));

    cache.set(3, 3);
    res = cache.top();
    ASSERT_EQ(3, res.value_or(-1));

    cache.set(1, 1);
    res = cache.top();
    ASSERT_EQ(3, res.value_or(-1));

    cache.set(2, 2);
    cache.set(6, 6);
    cache.set(7, 7);
    cache.set(8, 8);
    cache.set(9, 9);

    res = cache.top();
    ASSERT_EQ(9, res.value_or(-1));

    cache.set(1, 1);
    cache.set(2, 2);
    std::vector<std::pair<int32_t, int32_t>> expected = {{9, 9},
                                                         {8, 8},
                                                         {7, 7},
                                                         {6, 6},
                                                         {1, 1},
                                                         {2, 2},
                                                         {3, 3}};
    for (auto it: expected) {
        res = cache.top();
        cache.erase(it.first);
        ASSERT_EQ(it.second, res.value_or(-1));
    }
}


struct IntToStringTest : TestMyLRUCache<int32_t, std::string> {
    IntToStringTest() : TestMyLRUCache<int32_t, std::string>(10) {}
};

TEST_F(IntToStringTest, simple_has_elements_test) {
    std::vector<std::pair<int32_t, std::string>> elements = {
            {1, "aba"},
            {2, "caba"},
            {3, "daba"},
            {4, "qaba"},
            {5, "eaba"},
            {6, "zaba"},
            {7, "paba"},
            {8, "laba"}
    };
    for (auto const &item: elements) {
        cache.set(item.first, item.second);
    }
    for (auto it = elements.rbegin(); it != elements.rend() - 1; ++it) {
        auto res = cache.top();
        cache.erase(it->first);
        ASSERT_EQ(it->second, res.value_or(""));
    }
}
