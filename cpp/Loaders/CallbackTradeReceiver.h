#ifndef CALLBACKTRADERECEIVER_H
#define CALLBACKTRADERECEIVER_H

#include "../Models/ITradeReceiver.h"
#include <functional>

class CallbackTradeReceiver : public ITradeReceiver
{
public:
    explicit CallbackTradeReceiver(const std::function<void(ITrade*)>& onTrade) : onTrade_(onTrade) {}

    void add(ITrade* trade) override { onTrade_(trade); }

private:
    const std::function<void(ITrade*)>& onTrade_;
};

#endif // CALLBACKTRADERECEIVER_H
