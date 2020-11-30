// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <algorithm>

#include "util.h"


void save_dict(std::unordered_map<std::string, size_t> &dict) {
    std::vector<std::pair<std::string, size_t>> res;
    res.reserve(dict.size());

    for (auto &&i: dict)
        res.emplace_back(i.first, i.second);



    std::ofstream res_file1("./results/result_alphabetic.txt");
    std::ofstream res_file2("./results/result_amount.txt");


    std::sort(res.begin(), res.end(), [](const auto &a, const auto &b) { return a.first < b.first; });
    for (auto &&i: res)
        res_file1 << i.first << "\t" << i.second << "\n";

    res_file1.close();


    std::sort(res.begin(), res.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
    for (auto &&i: res)
        res_file2 << i.first << "\t" << i.second << "\n";

    res_file2.close();
}