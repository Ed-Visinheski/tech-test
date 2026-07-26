#ifndef BONDTRADELOADER_H
#define BONDTRADELOADER_H

#include "ITradeLoader.h"
#include "IStreamingTradeLoader.h"
#include "../Models/BondTrade.h"
#include "../Models/BondTradeList.h"
#include "../Models/ITradeReceiver.h"
#include <functional>
#include <string>
#include <vector>
#include <memory>

class BondTradeLoader : public ITradeLoader, public IStreamingTradeLoader
{
private:
    static constexpr char separator = ',';
    std::string dataFile_;

    BondTrade* createTradeFromLine(std::string line);
    void loadTradesFromFile(std::string filename, ITradeReceiver& tradeReceiver);

public:
    std::vector<ITrade*> loadTrades() override;
    void streamTrades(const std::function<void(ITrade*)>& onTrade) override;
    std::string getDataFile() const override;
    void setDataFile(const std::string& file) override;
};

#endif // BONDTRADELOADER_H
