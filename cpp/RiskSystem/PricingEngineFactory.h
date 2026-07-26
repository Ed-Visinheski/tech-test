#ifndef PRICINGENGINEFACTORY_H
#define PRICINGENGINEFACTORY_H

#include "../Models/IPricingEngine.h"
#include "PricingEngineConfig.h"
#include <map>
#include <memory>
#include <string>

class PricingEngineFactory 
{
public:
    static std::map<std::string, std::shared_ptr<IPricingEngine>> loadPricers(const std::string& configFile);

private:
    static std::shared_ptr<IPricingEngine> createPricingEngine(const std::string& typeName);
};

#endif // PRICINGENGINEFACTORY_H
