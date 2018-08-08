#include <flow/factory.h>

#include <fstream>
#include <iostream>
#include <list>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace
{

class Tick : public Slot
{
public:
    Tick(const std::map<std::string, std::string> & paras)
        : interval_(1)
        , stopped_(true)
    {
        auto iter = paras.find("interval");
		if(iter != paras.end())
        {
            size_t sec = 0;
            sec = std::stoi(iter->second);
            if(sec != 0)
                interval_ = sec;
		}
    }

    std::string type() override
    {
        return "Tick";
    }

    void addSink(SlotPtr s) override
    {
        slots_.push_back(s);
    }

    bool start() override
    {
        if(!stopped_)
            return true;
        thread_ = std::thread([this]()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            while (!stopped_)
            {
                cond_.wait_for(lock, std::chrono::seconds(interval_));
				std::string str("tick...\n");
                for(auto & s : slots_)
				{
                    s->flow((const uint8_t *)str.data(), str.size());
				}
            }
        });
        stopped_ = false;
        return true;
    }

    void flow(const uint8_t * data, size_t size) override
    {
    }

    void stop() override
    {
        if(stopped_)
            return;

        std::unique_lock<std::mutex> lock(mutex_);
        stopped_ = true;
        cond_.notify_one();
        lock.unlock();

        thread_.join();
    }

    ~Tick() override
    {
        stop();
    }

private:
    std::atomic<bool> stopped_;

    size_t interval_;

    std::list<SlotPtr> slots_;

    std::thread thread_;

    std::mutex mutex_;

    std::condition_variable cond_;
};

}

SlotPtr createSlot(const std::map<std::string, std::string> & paras)
{
    return std::make_shared<Tick>(paras);
}
