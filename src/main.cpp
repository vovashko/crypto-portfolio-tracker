#include "CryptoPortfolio.hpp"

void technicalAnalysis(const std::map<std::string, double> &cryptoRate, const std::string &cryptoSymbol)
{
    RSI rsi;
    double rsiValue = rsi.compute(cryptoRate, RSI_PERIOD);
    rsi.interpret(rsiValue, cryptoSymbol);
    MACD macd;
    double macdValue = macd.compute(cryptoRate, RSI_PERIOD);
    macd.interpret(macdValue, cryptoSymbol);
}


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <portfolio_file.csv>" << std::endl;
        return 1;
    }

    std::string portfolioFile = argv[1];
    double portfolioResult = 0;

    // Load crypto pairs from JSON
    std::vector<std::string> cryptoFiles = loadCryptoPairs(portfolioFile, HEADER);
    if (cryptoFiles.empty())
    {
        std::cerr << "No crypto pairs loaded. Exiting." << std::endl;
        return 1;
    }
    // Load all crypto price data
    std::map<std::string, std::map<std::string, double>> cryptoPrices = loadCryptoPrices(cryptoFiles);
    std::ifstream portfolioStream(portfolioFile);
    // Process portfolio for each crypto
    for (const auto &crypto : cryptoPrices)
    {
        std::map<std::string, double> cryptoRate = crypto.second;
        portfolioResult += printPortfolio(cryptoRate, portfolioStream, HEADER, crypto.first);
        portfolioStream.clear();
        portfolioStream.seekg(0, std::ios::beg);
        technicalAnalysis(cryptoRate, crypto.first);
        std::cout << std::endl;

    }
    std::cout << "Portfolio performance: $" << portfolioResult << std::endl;
    portfolioStream.close();
    return 0;
}
