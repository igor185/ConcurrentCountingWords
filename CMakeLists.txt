cmake_minimum_required(VERSION 3.16.3)
project(word_count)

find_package(LibArchive REQUIRED)
find_package(Boost REQUIRED COMPONENTS locale filesystem system)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(word_count src/main.cpp src/util.h src/process_file.cpp src/save_results.cpp src/read_file.cpp)
target_link_libraries(word_count LINK_PUBLIC ${Boost_LIBRARIES} ${LibArchive_LIBRARIES}  )