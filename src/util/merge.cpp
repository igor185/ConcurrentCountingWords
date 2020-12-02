// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../util.h"


void merge_dict(Map& dict1, Map& dict2){
    for (auto& it: dict2) {
        dict1[it.first]+=it.second;
    }
}