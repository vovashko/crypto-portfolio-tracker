#include "CryptoPortfolio.hpp"

double MACD::computeEMA(const std::vector<double> &prices, unsigned long period) {
    if (prices.size() < period) return -1;  // Not enough data

    // Compute SMA (initial EMA value)
    double sum = 0.0;
    for (unsigned long i = 0; i < period; ++i) {
        sum += prices[i];
    }
    double ema = sum / period;

    // Apply EMA formula
    double multiplier = 2.0 / (period + 1);
    for (unsigned long i = period; i < prices.size(); ++i) {
        ema = (prices[i] - ema) * multiplier + ema;
    }
    return ema;
}


double MACD::compute(const std::map<std::string, double> &cryptoRate, unsigned long period) {
    if (cryptoRate.size() < 26) return -1;  // Ensure enough data for MACD
    (void)period;
    std::vector<double> prices;
    for (const auto &pair : cryptoRate) {
        prices.push_back(pair.second);
    }

    std::vector<double> macdValues;
    for (unsigned long i = 0; i < prices.size(); ++i) {
        if (i < 26) continue; // Skip until we have enough data for long EMA

        double shortEMA = computeEMA(std::vector<double>(prices.begin(), prices.begin() + i), 12);
        double longEMA = computeEMA(std::vector<double>(prices.begin(), prices.begin() + i), 26);
        macdValues.push_back(shortEMA - longEMA);
    }

    if (macdValues.size() < 9) return -1;  // Not enough MACD values for signal line

    double signalLine = computeEMA(macdValues, 9);
    return macdValues.back() - signalLine;
}

void MACD::interpret(double macd, const std::string &cryptoSymbol)
{
    if (macd > 0)
        std::cout << "MACD for " << cryptoSymbol << ": " << macd << " => BUY Signal." << std::endl;
    else
        std::cout << "MACD for " << cryptoSymbol << ": " << macd << " => SELL Signal." << std::endl;
}