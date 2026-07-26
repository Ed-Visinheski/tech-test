#ifndef STREAMINGTRADELOADER_H
#define STREAMINGTRADELOADER_H

#include "../Loaders/IStreamingTradeLoader.h"
#include "../Models/ITrade.h"
#include "../Models/IScalarResultReceiver.h"
#include "../Models/IPricingEngine.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class StreamingTradeLoader
{
private:
    std::map<std::string, std::shared_ptr<IPricingEngine>> pricers_;

    std::vector<std::unique_ptr<IStreamingTradeLoader>> getTradeLoaders();
    void loadPricers();

public:
    void loadAndPrice(IScalarResultReceiver* resultReceiver);
};

#endif // STREAMINGTRADELOADER_H
