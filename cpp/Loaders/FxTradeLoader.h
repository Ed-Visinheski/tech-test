#ifndef FXTRADELOADER_H
#define FXTRADELOADER_H

#include "ITradeLoader.h"
#include "IStreamingTradeLoader.h"
#include "../Models/FxTrade.h"
#include "../Models/FxTradeList.h"
#include "../Models/ITradeReceiver.h"
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <memory>

class FxTradeLoader : public ITradeLoader, public IStreamingTradeLoader
{
private:
    static constexpr std::string_view separator {"¬",2};

    std::string dataFile_;

    FxTrade* createTradeFromLine(std::string line);
    void loadTradesFromFile(std::string filename, ITradeReceiver& tradeReceiver);

public:
    std::vector<ITrade*> loadTrades() override;
    void streamTrades(const std::function<void(ITrade*)>& onTrade) override;
    std::string getDataFile() const override;
    void setDataFile(const std::string& file) override;
};

#endif // FXTRADELOADER_H
