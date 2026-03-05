//
// Created by Aboba on 05.03.2026.
//

#ifndef OURPROJECT_CSV_LOADER_H
#define OURPROJECT_CSV_LOADER_H


#pragma once

#include <string>
#include <vector>
#include <optional>

struct MarketTick {
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

class DataException : public std::exception {
private:

    std::string message;
public:

    explicit DataException(std::string msg);
    const char* what() const noexcept override;
};

class MarketDataLoader {
public:
    static std::vector<MarketTick> loadFromFile(const std::string& path);
};


#endif //OURPROJECT_CSV_LOADER_H