// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <algorithm>
#include <thread>

#include "../util.h"
template <typename Comp>
void save(std::vector<std::pair<std::string, size_t>>& res, const std::string& path, Comp comp) {
    std::ofstream res_file(path);

    std::sort(res.begin(), res.end(), comp);
    for (auto &&i: res)
        res_file << i.first << "\t" << i.second << "\n";

    res_file.close();
}

void save_dict(std::unordered_map<std::string, size_t>& dict) {
    std::vector<std::pair<std::string, size_t>> res1;
    std::vector<std::pair<std::string, size_t>> res2; // two vectors in order to sort and save in different threads

    res1.reserve(dict.size());
    res2.reserve(dict.size());

    for (auto &&i: dict) {
        res1.emplace_back(i);
        res2.emplace_back(i);
    }


    auto comp1 = [](const auto &a, const auto &b) { return a.first < b.first; };
    auto comp2 = [](const auto &a, const auto &b) { return a.second > b.second; };

    std::thread tr{save<typeof(comp1)>, std::ref(res1), "./results/result_alphabetic.txt", comp1};
    save(res2, "./results/result_amount.txt", comp2);

    tr.join();
}