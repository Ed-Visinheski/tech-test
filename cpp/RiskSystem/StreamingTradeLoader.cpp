#include "StreamingTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"
#include "PricingEngineFactory.h"
#include <stdexcept>

std::vector<std::unique_ptr<IStreamingTradeLoader>> StreamingTradeLoader::getTradeLoaders()
{
    std::vector<std::unique_ptr<IStreamingTradeLoader>> loaders;

    auto bondLoader = std::make_unique<BondTradeLoader>();
    bondLoader->setDataFile("TradeData/BondTrades.dat");
    loaders.push_back(std::move(bondLoader));

    auto fxLoader = std::make_unique<FxTradeLoader>();
    fxLoader->setDataFile("TradeData/FxTrades.dat");
    loaders.push_back(std::move(fxLoader));

    return loaders;
}

void StreamingTradeLoader::loadPricers()
{
    pricers_ = PricingEngineFactory::loadPricers("./PricingConfig/PricingEngines.xml");
}

void StreamingTradeLoader::loadAndPrice(IScalarResultReceiver* resultReceiver)
{
    if (!resultReceiver)
    {
        throw std::invalid_argument("resultReceiver is nullptr");
    }

    loadPricers();

    auto priceOne = [this, resultReceiver](ITrade* rawTrade)
    {
        std::unique_ptr<ITrade> trade(rawTrade);

        auto pricerIt = pricers_.find(trade->getTradeType());
        if (pricerIt == pricers_.end())
        {
            resultReceiver->addError(trade->getTradeId(), "No Pricing Engines available for this trade type");
            return;
        }

        pricerIt->second->price(trade.get(), resultReceiver);
    };

    for (const auto& loader : getTradeLoaders())
    {
        loader->streamTrades(priceOne);
    }
}
