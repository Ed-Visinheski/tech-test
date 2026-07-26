#include "ParallelPricer.h"
#include "PricingEngineFactory.h"
#include "../Models/TradeQueue.h"
#include "../Models/SynchedScalarResults.h"
#include <stdexcept>
#include <thread>

void ParallelPricer::loadPricers()
{
    pricers_ = PricingEngineFactory::loadPricers("./PricingConfig/PricingEngines.xml");
}

void ParallelPricer::price(const std::vector<std::vector<ITrade*>>& tradeContainers, IScalarResultReceiver* resultReceiver)
{
    if (!resultReceiver)
    {
        throw std::invalid_argument("resultReceiver is nullptr");
    }

    loadPricers();

    TradeQueue<ITrade*> queue;
    size_t tradeCount = 0;
    for (const auto& tradeContainer : tradeContainers)
    {
        for (ITrade* trade : tradeContainer)
        {
            queue.push(trade);
            ++tradeCount;
        }
    }

    SynchedScalarResults synchronizedReceiver(*resultReceiver, resultMutex_);
    auto priceOne = [this, &queue, &synchronizedReceiver]()
    {
        ITrade* trade = nullptr;
        if (!queue.pop(trade))
        {
            return;
        }

        auto pricerIt = pricers_.find(trade->getTradeType());
        if (pricerIt == pricers_.end())
        {
            synchronizedReceiver.addError(trade->getTradeId(), "No Pricing Engines available for this trade type");
            return;
        }

        pricerIt->second->price(trade, &synchronizedReceiver);
    };

    std::vector<std::thread> workers;
    workers.reserve(tradeCount);
    for (size_t i = 0; i < tradeCount; ++i)
    {
        workers.emplace_back(priceOne);
    }

    for (auto& worker : workers)
    {
        worker.join();
    }
}
