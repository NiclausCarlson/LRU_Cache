//
// Created by nik-carlson on 9/26/21.
//

#pragma once

#include <cstddef>
#include <list>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <cassert>

template<typename T, typename V>
class MyLRUCache {
private:
    struct key_value_pair {
        T key;
        V value;
    };
    std::list<key_value_pair> list;
    std::unordered_map<T, typename std::list<key_value_pair>::iterator> hash_map;
    size_t _capacity;

public:
    explicit MyLRUCache(size_t capacity) : _capacity(capacity) {
    }

    void set(T const &key, V const &value) {
        auto map_iter = hash_map.find(key);
        if (map_iter != hash_map.end()) {// map.contains since C++20 ;(
            auto iter = map_iter->second;
            assert(iter->value == value && "value in cache doesn't equals new value");
            auto next_iter = iter;
            if (++next_iter != list.end()) {
                std::iter_swap(iter, next_iter);
                auto key_1 = iter->key, key_2 = next_iter->key;
                hash_map.erase(map_iter);
                hash_map.erase(key_2);
                hash_map[key_1] = iter;
                hash_map[key_2] = next_iter;
            }
        } else {
            if (list.size() == _capacity) list.pop_front();

            list.push_back(key_value_pair{key, value});
            assert(list.back().value == value && "list.back() != value");
            auto last = list.end();
            hash_map[key] = --last;
        }
    }

    void erase(T const &key) {
        auto iter = hash_map.find(key);
        if (iter != hash_map.end()) {
            list.erase(iter->second);
            hash_map.erase(iter);
        }
    }

    std::optional<T> get(T const &key) {
        if (hash_map.count(key) == 0) return std::nullopt;
        return {hash_map[key]->value};
    }

    std::optional<V> top() {
        if (list.empty()) return std::nullopt;
        return {list.back().value};
    }

    size_t capacity() {
        return _capacity;
    }

    size_t size() {
        return hash_map.size();
    }
};
