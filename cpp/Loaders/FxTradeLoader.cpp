#include "FxTradeLoader.h"
#include "CallbackTradeReceiver.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <string>


FxTrade* FxTradeLoader::createTradeFromLine(std::string line)
{
    size_t currentPos;
    size_t startPos = 0;
    std::vector<std::string> items;

    while ((currentPos = line.find(separator, startPos)) != std::string::npos)
    {
        items.push_back(line.substr(startPos, currentPos - startPos));
        startPos = currentPos + separator.size();
    }

    items.push_back(line.substr(startPos));

    if (items.size() < 9)
    {
        throw std::runtime_error("Invalid line format");
    }
    
    FxTrade* trade = new FxTrade(items[8], items[0]);

    std::tm tm = {};
    std::istringstream dateStream(items[1]);
    dateStream >> std::get_time(&tm, "%Y-%m-%d");
    auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    trade->setTradeDate(timePoint);

    std::tm valueTm = {};
    std::istringstream valueDateStream(items[6]);
    valueDateStream >> std::get_time(&valueTm, "%Y-%m-%d");
    auto valueTimePoint = std::chrono::system_clock::from_time_t(std::mktime(&valueTm));
    trade->setValueDate(valueTimePoint);

    trade->setInstrument((items[2] + items[3]));
    trade->setCounterparty(items[7]);
    
    trade->setNotional(std::stod(items[4]));
    trade->setRate(std::stod(items[5]));

    return trade;
}

void FxTradeLoader::loadTradesFromFile(std::string filename, ITradeReceiver& tradeReceiver) 
{
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be null");
    }

    std::ifstream stream(filename);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    int lineCount = 0;
    std::string line;
    while (std::getline(stream, line))
    {
        if(lineCount < 2) //Skip 2 line header
        {
            ++lineCount;
            continue;
        }

        if(stream.peek() == std::stringstream::traits_type::eof()) //Skip "END"
        {
            break;
        }

        if(!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        tradeReceiver.add(createTradeFromLine(line));
    }
}

std::vector<ITrade*> FxTradeLoader::loadTrades() {
    FxTradeList tradeList;
    loadTradesFromFile(dataFile_, tradeList);

    std::vector<ITrade*> result;
    for (size_t i = 0; i < tradeList.size(); ++i)
    {
        result.push_back(tradeList[i]);
    }
    return result;
}

void FxTradeLoader::streamTrades(const std::function<void(ITrade*)>& onTrade) 
{
    CallbackTradeReceiver receiver(onTrade);
    loadTradesFromFile(dataFile_, receiver);
}

std::string FxTradeLoader::getDataFile() const {
    return dataFile_;
}

void FxTradeLoader::setDataFile(const std::string& file) {
    dataFile_ = file;
}
