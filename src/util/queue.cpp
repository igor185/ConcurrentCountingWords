// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <queue>
#include <condition_variable>
#include <iostream>

template<typename T>
class concurrent_queue {
public:
    concurrent_queue() = delete;

    explicit concurrent_queue(size_t limit) : amount_limit(limit) {}

    void push(const T &element) {
//        if(!element.size()) {
//            std::cout << std::to_string(element.size()) + typeid(element).name() + '\n';
//        }
        std::unique_lock<std::mutex> lock(mutex_push);
        if (data.size() == amount_limit)
            cv_pop.wait(lock, [&] { return data.size() != amount_limit; });
        data.push_back(element);
        lock.unlock();
        cv_push.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_pop);
        if (data.empty())
            cv_push.wait(lock, [&] { return !data.empty(); });
        auto elem = data.front();
        data.pop_front();
        lock.unlock();
        cv_pop.notify_one();
        return elem;
    }

    size_t size() const {
        return data.size();
    }

private:
    size_t amount_limit = 10000;
    std::deque<T> data;
    std::condition_variable cv_push;
    std::condition_variable cv_pop;
    std::mutex mutex_push;
    std::mutex mutex_pop;
};