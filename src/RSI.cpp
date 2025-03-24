#include "CryptoPortfolio.hpp"

double RSI::compute(const std::map<std::string, double> &cryptoRate, unsigned long period) {
    if (cryptoRate.size() < period) return -1;  // Not enough data

    auto it = cryptoRate.begin();
    std::advance(it, period);

    double avgGain = 0, avgLoss = 0;
    auto prevIt = cryptoRate.begin();

    for (auto currentIt = std::next(cryptoRate.begin()); currentIt != it; ++currentIt) {
        double change = currentIt->second - prevIt->second;
        if (change > 0) avgGain += change;
        else avgLoss -= change;
        prevIt = currentIt;
    }

    avgGain /= period;
    avgLoss /= period;

    for (; it != cryptoRate.end(); ++it) {
        double change = it->second - prevIt->second;
        double gain = change > 0 ? change : 0;
        double loss = change < 0 ? -change : 0;

        avgGain = (avgGain * (period - 1) + gain) / period;
        avgLoss = (avgLoss * (period - 1) + loss) / period;

        prevIt = it;
    }

    double rs = avgLoss == 0 ? 100 : avgGain / avgLoss;
    return 100 - (100 / (1 + rs));
}

void RSI::interpret(double rsi, const std::string &cryptoSymbol) {
    int rsiCategory = (rsi < 30) ? 0 : (rsi < 50) ? 1 : (rsi < 70) ? 2 : 3;

    std::cout << "RSI for " << cryptoSymbol << ": " << rsi << " => ";

    switch (rsiCategory)
    {
        case 0:
            std::cout << "Oversold (BUY Signal)." << std::endl;
            break;
        case 1:
            std::cout << "Weak Momentum (Hold / Caution)." << std::endl;
            break;
        case 2:
            std::cout << "Strong Momentum (Hold / Caution)." << std::endl;
            break;
        case 3:
            std::cout << "Overbought (SELL Signal)." << std::endl;
            break;
        default:
            std::cout << "Invalid RSI value." << std::endl;
            break;
    }
}