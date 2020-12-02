// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef WORD_COUNT_UTIL_H
#define WORD_COUNT_UTIL_H

#include <vector>
#include <fstream>
#include <unordered_map>
#include <string>
#include <chrono>
#include <atomic>

typedef std::unordered_map<std::string, size_t> Map;

std::vector<std::string> read_file(const std::string& file_name);

void process_file(std::string& file, std::unordered_map<std::string, size_t>& dict);

void save_dict(std::unordered_map<std::string, size_t>& dict);

void merge_dict(Map& dict1, Map& dict2);

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D &d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}


#endif //WORD_COUNT_UTIL_H
