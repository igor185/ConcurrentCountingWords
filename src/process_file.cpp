// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <algorithm>
#include <boost/locale/generator.hpp>
#include <boost/locale.hpp>
#include "boost/locale/boundary.hpp"

namespace ba = boost::locale::boundary;

#include "util.h"

bool is_alpha(char c) {
    return isalpha(c) != 0;
}

std::string_view process_word(std::string&& str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    std::string_view view{str};

    size_t index = -1;
    while (index + 1 != view.size() && !is_alpha(view[index + 1])) {
        index++;
    }
    if (index != -1) {
        view = view.substr(index + 1);
    }

    index = view.size();
    while (index != 0 && !is_alpha(view[index - 1])) {
        index--;
    }

    if (index != view.size()) {
        view = view.substr(0, index);
    }
    return view;
}

void process_file(std::string &file, std::unordered_map<std::string, size_t> &dict) {
    boost::locale::generator gen;
    std::locale::global(gen("en_US.UTF-8"));

    std::vector<std::string> words_vec;
    ba::ssegment_index words(ba::word, file.begin(), file.end());
    std::string str;


    for (auto &&x: words) {
        str = process_word(boost::locale::normalize(x.str()));
        if(!str.empty()) {
            dict[std::move(str)]++;
        }
    }
}