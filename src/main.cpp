#include "CryptoExchange.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <portfolio_file>" << std::endl;
        return 1;
    }

    std::string portfolioFile = argv[1];
    double portfolioResult = 0;

    // Load crypto pairs from JSON
    std::vector<std::string> cryptoFiles = loadCryptoPairs(portfolioFile, 1);
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
        std::map<std::string, double> btcRate = crypto.second;
        portfolioResult += printPortfolio(btcRate, portfolioStream, 1, crypto.first);
        portfolioStream.clear();
        portfolioStream.seekg(0, std::ios::beg);
        double rsi = computeRSI(btcRate, 14);
        interpretRSI(rsi, crypto.first);
       std::cout << std::endl;

    }
    std::cout << "Portfolio performance: $" << portfolioResult << std::endl;
    portfolioStream.close();
    return 0;
}
