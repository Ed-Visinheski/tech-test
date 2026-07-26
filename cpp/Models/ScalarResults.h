#ifndef SCALARRESULTS_H
#define SCALARRESULTS_H

#include "IScalarResultReceiver.h"
#include "ScalarResult.h"
#include <map>
#include <vector>
#include <optional>
#include <string>
#include <iterator>
#include <utility>
using ScalarResultsMap = std::map<std::string, std::pair<std::optional<double>, std::optional<std::string>>>;

class ScalarResults : public IScalarResultReceiver 
{

public:
    virtual ~ScalarResults();
    std::optional<ScalarResult> operator[](const std::string& tradeId) const;

    bool containsTrade(const std::string& tradeId) const;

    virtual void addResult(const std::string& tradeId, double result) override;

    virtual void addError(const std::string& tradeId, const std::string& error) override;

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = ScalarResult;
        using difference_type = std::ptrdiff_t;
        using pointer = ScalarResult*;
        using reference = ScalarResult&;

        Iterator() = default;

        // Iterator must be constructable from ScalarResults parent
        Iterator(ScalarResultsMap::const_iterator scalarResultsIt): scalarResultsIt_(scalarResultsIt) {};

        Iterator& operator++();
        ScalarResult operator*() const;
        bool operator!=(const Iterator& other) const;

        private:
            ScalarResultsMap::const_iterator scalarResultsIt_;
    };

    Iterator begin() const;
    Iterator end() const;

private:
    ScalarResultsMap scalarResults_ {};
};

#endif // SCALARRESULTS_H
