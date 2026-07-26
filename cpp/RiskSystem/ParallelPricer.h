#ifndef PARALLELPRICER_H
#define PARALLELPRICER_H

#include "../Models/IPricingEngine.h"
#include "../Models/ITrade.h"
#include "../Models/IScalarResultReceiver.h"
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

class ParallelPricer
{
private:
    std::map<std::string, std::shared_ptr<IPricingEngine>> pricers_;
    std::mutex resultMutex_;

    void loadPricers();

public:
    void price(const std::vector<std::vector<ITrade*>>& tradeContainers,
               IScalarResultReceiver* resultReceiver);
};

#endif // PARALLELPRICER_H
