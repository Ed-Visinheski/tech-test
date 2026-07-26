#ifndef TRADEQUEUE_H
#define TRADEQUEUE_H
#include <condition_variable>
#include <queue>
#include <mutex>


template <typename T>
class TradeQueue
{
public:
    void push(T trade)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(trade));

        conditionVar_.notify_one();
    }

    bool pop(T& trade)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        conditionVar_.wait(lock,
            [this] 
            { 
                return !queue_.empty() || completed_; 
            });
        
        if (queue_.empty() && completed_) 
        {
            return false;   
        }
        
        trade = std::move(queue_.front());
        queue_.pop();
        return true;

    }

    void setCompleted() 
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            completed_ = true;
        }
        conditionVar_.notify_all();
    }

    private:
    
    std::mutex mutex_{};
    bool completed_ {false};

    std::queue<T> queue_{};
    std::condition_variable conditionVar_{};
};


#endif //TRADEQUEUE_H