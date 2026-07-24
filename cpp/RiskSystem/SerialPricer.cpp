#include "SerialPricer.h"
#include "../Pricers/GovBondPricingEngine.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include <stdexcept>

std::shared_ptr<IPricingEngine> SerialPricer::createPricingEngine(const std::string& typeName)
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

void SerialPricer::loadPricers()
{
    pricers_.clear();

    PricingConfigLoader pricingConfigLoader;
    pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");
    PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();

    std::map<std::string, std::shared_ptr<IPricingEngine>> enginesByTypeName;
    for (const auto& configItem : pricerConfig)
    {
        std::string typeName = configItem.getTypeName();

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

        pricers_[configItem.getTradeType()] = engine;
    }
}

void SerialPricer::price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
                         IScalarResultReceiver* resultReceiver) {
    loadPricers();
    
    for (const auto& tradeContainer : tradeContainers) {
        for (ITrade* trade : tradeContainer) {
            std::string tradeType = trade->getTradeType();
            if (pricers_.find(tradeType) == pricers_.end()) {
                resultReceiver->addError(trade->getTradeId(), "No Pricing Engines available for this trade type");
                continue;
            }
                  
            std::shared_ptr<IPricingEngine> pricer = pricers_[tradeType];
            pricer->price(trade, resultReceiver);
        }
    }
}
