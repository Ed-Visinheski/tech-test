#include "PricingConfigLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <regex>

std::string PricingConfigLoader::getConfigFile() const {
    return configFile_;
}

void PricingConfigLoader::setConfigFile(const std::string& file) {
    configFile_ = file;
}

PricingEngineConfig PricingConfigLoader::loadConfig() {
    if (configFile_.empty()) {
        throw std::invalid_argument("Config file cannot be null");
    }

    std::ifstream stream(configFile_);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + configFile_);
    }

    std::stringstream fileContents;
    fileContents << stream.rdbuf();

    return parseXml(fileContents.str());
}

PricingEngineConfig PricingConfigLoader::parseXml(const std::string& fileContents) {
    static const std::regex engineTagRegex("<Engine\\b([^>]*)/>");
    static const std::regex attributeRegex("(\\w+)\\s*=\\s*\"([^\"]*)\"");

    PricingEngineConfig pricingEngineConfig;

    auto engineTagsBegin = std::sregex_iterator(fileContents.begin(), fileContents.end(), engineTagRegex);
    auto engineTagsEnd = std::sregex_iterator();

    for (auto engineTagIt = engineTagsBegin; engineTagIt != engineTagsEnd; ++engineTagIt) {
        std::string attributesText = (*engineTagIt)[1].str();

        PricingEngineConfigItem item;
        auto attributesBegin = std::sregex_iterator(attributesText.begin(), attributesText.end(), attributeRegex);
        auto attributesEnd = std::sregex_iterator();

        for (auto attributesIt = attributesBegin; attributesIt != attributesEnd; ++attributesIt) 
        {
            std::string name = (*attributesIt)[1].str();
            std::string value = (*attributesIt)[2].str();

            if (name == "tradeType") 
            {
                item.setTradeType(value);
            } 
            else if (name == "assembly") 
            {
                item.setAssembly(value);
            } 
            else if (name == "pricingEngine") 
            {
                item.setTypeName(value);
            }
        }

        pricingEngineConfig.push_back(item);
    }

    if (pricingEngineConfig.empty()) {
        throw std::runtime_error("No pricing engine configuration found in: " + configFile_);
    }

    return pricingEngineConfig;
}
