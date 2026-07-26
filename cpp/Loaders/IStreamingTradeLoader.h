#ifndef ISTREAMINGTRADELOADER_H
#define ISTREAMINGTRADELOADER_H

#include "../Models/ITrade.h"
#include <functional>

class IStreamingTradeLoader
{
public:
    virtual ~IStreamingTradeLoader() = default;
    virtual void streamTrades(const std::function<void(ITrade*)>& onTrade) = 0;
};

#endif // ISTREAMINGTRADELOADER_H
