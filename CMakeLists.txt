cmake_minimum_required(VERSION 3.16.3)
project(word_count)

find_package(LibArchive REQUIRED)
find_package(Boost REQUIRED COMPONENTS locale filesystem system)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(word_count src/main.cpp src/util.h src/util/process_file.cpp src/util/save_results.cpp src/util/read_file.cpp src/util/queue.cpp src/util/merge.cpp)
target_link_libraries(word_count LINK_PUBLIC ${Boost_LIBRARIES} ${LibArchive_LIBRARIES}  )