// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include <archive.h>
#include <archive_entry.h>
#include <sstream>

#include "../util.h"

std::string get_file_ext(const std::string &file_name) {
    std::vector<std::string> file_and_ext;
    std::string extension;
    boost::split(file_and_ext, file_name, boost::is_any_of("."));
    return file_and_ext[file_and_ext.size() - 1];
}


std::vector<std::string> read_file(const std::string &file_name) {
    std::vector<std::string> res;

    std::string extension = get_file_ext(file_name);

    if (extension == "txt") {
        std::fstream f(file_name);
        if(f.is_open()) {
            res.emplace_back((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        }
    } else if (extension == "zip") {
        struct archive *a;
        struct archive_entry *entry;
        int r;

        a = archive_read_new();
        archive_read_support_filter_all(a);
        archive_read_support_format_all(a);
        r = archive_read_open_filename(a, file_name.c_str(), 10240);
        if (r != ARCHIVE_OK)
            return res;
        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            if (archive_entry_filetype(entry) == AE_IFREG) {
                auto *buff = new char[archive_entry_size(entry)];
                archive_read_data(a, buff, archive_entry_size(entry));
                res.emplace_back(buff);
                delete[] buff;
            }
        }
        r = archive_read_free(a);
        if (r != ARCHIVE_OK)
            return res;
    } else {
        return res;
    }
    return res;
}