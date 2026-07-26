#include "ScalarResults.h"
#include "ScalarResult.h"
#include <stdexcept>

ScalarResults::~ScalarResults() = default;

std::optional<ScalarResult> ScalarResults::operator[](const std::string& tradeId) const 
{
    auto tradeIt = scalarResults_.find(tradeId);
    if(tradeIt == scalarResults_.cend())
    {
        return std::nullopt;
    }

    const auto& [priceResult, error] = tradeIt->second;

    return ScalarResult(tradeId, priceResult, error);
}

bool ScalarResults::containsTrade(const std::string& tradeId) const 
{
    return scalarResults_.contains(tradeId);
}

void ScalarResults::addResult(const std::string& tradeId, double result) 
{
    scalarResults_[tradeId].first = result;
}

void ScalarResults::addError(const std::string& tradeId, const std::string& error) 
{
    scalarResults_[tradeId].second = error;
}

ScalarResults::Iterator& ScalarResults::Iterator::operator++() 
{
    ++scalarResultsIt_;
    return *this;
}

ScalarResult ScalarResults::Iterator::operator*() const 
{
    return ScalarResult(scalarResultsIt_->first, scalarResultsIt_->second.first, scalarResultsIt_->second.second);
}

bool ScalarResults::Iterator::operator!=(const Iterator& other) const 
{
    return scalarResultsIt_ != other.scalarResultsIt_;
}

ScalarResults::Iterator ScalarResults::begin() const 
{
    return Iterator(scalarResults_.cbegin());
}

ScalarResults::Iterator ScalarResults::end() const 
{
    return Iterator(scalarResults_.cend());
}
