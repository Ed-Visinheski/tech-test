#ifndef SERIALPRICER_H
#define SERIALPRICER_H

#include "../Models/IPricingEngine.h"
#include "../Models/ITrade.h"
#include "../Models/IScalarResultReceiver.h"
#include "PricingConfigLoader.h"
#include <map>
#include <vector>
#include <memory>
#include <string>

class SerialPricer {
private:
    std::map<std::string, std::shared_ptr<IPricingEngine>> pricers_;

    void loadPricers();

public:
    void price(const std::vector<std::vector<ITrade*>>& tradeContainers,
               IScalarResultReceiver* resultReceiver);
};

#endif // SERIALPRICER_H
