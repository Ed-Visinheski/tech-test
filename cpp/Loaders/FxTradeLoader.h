#ifndef FXTRADELOADER_H
#define FXTRADELOADER_H

#include "ITradeLoader.h"
#include "../Models/FxTrade.h"
#include "../Models/FxTradeList.h"
#include <string>
#include <string_view>
#include <vector>
#include <memory>

class FxTradeLoader : public ITradeLoader {
private:
    static constexpr std::string_view separator {"¬",2};

    std::string dataFile_;

    FxTrade* createTradeFromLine(std::string line);
    void loadTradesFromFile(std::string filename, FxTradeList& tradeList);
  
public:
    // NOTE: These methods are only here to allow the solution to compile prior to the test being completed.
    std::vector<ITrade*> loadTrades() override;
    std::string getDataFile() const override;
    void setDataFile(const std::string& file) override;
};

#endif // FXTRADELOADER_H
