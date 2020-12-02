// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <unordered_map>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include <thread>

#include "util.h"
#include "util/queue.cpp"

void print_help() {
    std::cout << "Usage: pass path to process" << std::endl;
    exit(EXIT_SUCCESS);
}

void fs_reading_thread(const std::string& dir, concurrent_queue<std::string>& files, size_t empty_lines){

    boost::filesystem::recursive_directory_iterator it(dir), end_itr;

    for (auto &&file_name: boost::make_iterator_range(it, end_itr)){
        if(boost::filesystem::is_directory(file_name))
            continue;
        for (auto &&file: read_file(file_name.path().c_str())) {
            if(!file.empty())
                files.push(file);
        }
    }


    for(int i = 0; i < empty_lines; i++)
        files.push(""); // to stop threads
}

void file_processing_thread(concurrent_queue<std::string>& files, concurrent_queue<Map>& dicts) {
    auto file = files.pop();
    Map dict;

    while (!file.empty()) {
        dict.clear();
        process_file(file, dict);
        if(!dict.empty()) {
            dicts.push(dict);
        }
        file = files.pop();
    }
}

void merge_dicts_thread(concurrent_queue<Map>& dict){
    Map dict1 = dict.pop(), dict2;

    while (!dict1.empty()){
       dict2 = dict.pop();
       if(dict2.empty()){
           if(!dict1.empty())
            dict.push(dict1);
           return;
       }

       merge_dict(dict1, dict2);
       dict2.clear();

       if(!dict1.empty()) {
           dict.push(dict1);
       }
       dict1 = dict.pop();
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


    if (!boost::filesystem::is_directory(dir)) {
        std::cerr << dir << " it is a file" << std::endl;
        return 0;
    }

    concurrent_queue<std::string> files(1000);
    concurrent_queue<Map> dicts(100000);
    Map main_dict;

    size_t to_process = 5, to_merge = 2;
    std::thread reading;
    std::vector<std::thread> processing;
    std::vector<std::thread> merging;

    processing.reserve(to_process);
    merging.reserve(to_merge);

    reading = std::thread{fs_reading_thread, std::ref(dir), std::ref(files), to_process};

    for(size_t i = 0; i < to_process; i++)
        processing[i] = std::thread{file_processing_thread, std::ref(files), std::ref(dicts)};

    for(size_t i = 0; i < to_merge; i++)
        merging[i] = std::thread{merge_dicts_thread, std::ref(dicts)};


    // joining threads (should be one non-empty Map in dicts)
    reading.join();


    for(size_t i = 0; i < to_process; i++)
        processing[i].join();

    for(size_t i = 0; i < to_merge; i++)
        dicts.push({});

    for(size_t i = 0; i < to_merge; i++)
        merging[i].join();

    if(dicts.size() != 2) {
        std::cout << "Something go wrong, size of dicts: " << dicts.size() << std::endl;
        return 1;
    }

    auto a = dicts.pop(), b = dicts.pop();
    merge_dict(a, b);

    std::cout << "End processing" << std::endl;


    save_dict(a);

    return 0;
}
