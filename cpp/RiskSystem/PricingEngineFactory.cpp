#include "PricingEngineFactory.h"
#include "PricingConfigLoader.h"
#include "../Pricers/GovBondPricingEngine.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include <stdexcept>

std::shared_ptr<IPricingEngine> PricingEngineFactory::createPricingEngine(const std::string& typeName) 
{
    if (typeName == "HmxLabs.TechTest.Pricers.GovBondPricingEngine")
    {
        return std::make_shared<GovBondPricingEngine>();
    }

    else if (typeName == "HmxLabs.TechTest.Pricers.CorpBondPricingEngine")
    {
        return std::make_shared<CorpBondPricingEngine>();
    }

    else if (typeName == "HmxLabs.TechTest.Pricers.FxPricingEngine")
    {
        return std::make_shared<FxPricingEngine>();
    }

    throw std::runtime_error("Unknown pricing engine type: " + typeName);
}

std::map<std::string, std::shared_ptr<IPricingEngine>> PricingEngineFactory::loadPricers(const std::string& configFile) 
{
    PricingConfigLoader pricingConfigLoader;
    pricingConfigLoader.setConfigFile(configFile);
    PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();

    std::map<std::string, std::shared_ptr<IPricingEngine>> enginesByTypeName;
    std::map<std::string, std::shared_ptr<IPricingEngine>> pricersByTradeType;

    for (const auto& configItem : pricerConfig)
    {
        const std::string& typeName = configItem.getTypeName();

        auto existingEngine = enginesByTypeName.find(typeName);
        std::shared_ptr<IPricingEngine> engine;

        if (existingEngine != enginesByTypeName.end())
        {
            engine = existingEngine->second;
        }
        else
        {
            engine = createPricingEngine(typeName);
            enginesByTypeName[typeName] = engine;
        }

        pricersByTradeType[configItem.getTradeType()] = engine;
    }

    return pricersByTradeType;
}
