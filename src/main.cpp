// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <unordered_map>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include <archive_entry.h>

#include "util.h"

void print_help() {
    std::cout << "Usage: pass filename or path" << std::endl;
    exit(EXIT_SUCCESS);
}

void process_file(const std::string &file_name, long &time_read, long &time_counting,
                  std::unordered_map<std::string, size_t> &dict) {
    for (auto &&file: read_file(file_name, time_read)) {

        auto start_counting = get_current_time_fenced();

        process_file(file, dict);

        auto end_counting = get_current_time_fenced();

        time_counting += std::chrono::duration_cast<std::chrono::nanoseconds>(
                end_counting - start_counting).count();
    }
}

int main(int argc, char **argv) {
    std::string dir;
    if (argc != 2) {
        print_help();
    } else {
        dir = argv[1];
    }

    if (!boost::filesystem::exists(dir)) {
        std::cout << "Cannot find: " << dir << std::endl;
        exit(EXIT_FAILURE);
    }

    std::unordered_map<std::string, size_t> dict;
    long time_read = 0, time_counting = 0;

    if (!boost::filesystem::is_directory(dir)) {
        process_file(dir, time_read, time_counting, dict);
    } else {
        boost::filesystem::recursive_directory_iterator it(dir), end_itr;

        for (auto &&file_name: boost::make_iterator_range(it, end_itr))
            process_file(file_name.path().c_str(), time_read, time_counting, dict);
    }

    auto start = get_current_time_fenced();
    save_dict(dict);
    auto finish = get_current_time_fenced();

    std::cout << "Reading total time: " << time_read / 1000 << std::endl;
    std::cout << "Processing files total time: " << time_counting / 1000 << std::endl;
    std::cout << "Saving time: " << to_us(finish - start) << std::endl;
    return 0;
}