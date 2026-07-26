#ifndef SYNCHEDSCALARRESULTS_H
#define SYNCHEDSCALARRESULTS_H

#include "IScalarResultReceiver.h"
#include <mutex>

class SynchedScalarResults : public IScalarResultReceiver
    {
    public:
        SynchedScalarResults(IScalarResultReceiver& results, std::mutex& mutex) : results_(results), mutex_(mutex) {}

        void addResult(const std::string& tradeId, double result) override
        {
            std::lock_guard<std::mutex> lock(mutex_);
            results_.addResult(tradeId, result);
        }

        void addError(const std::string& tradeId, const std::string& error) override
        {
            std::lock_guard<std::mutex> lock(mutex_);
            results_.addError(tradeId, error);
        }

    private:
        IScalarResultReceiver& results_;
        std::mutex& mutex_;
    };

    #endif //SYNCHEDSCALARRESULTS_H