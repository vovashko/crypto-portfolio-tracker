#include "CryptoPortfolio.hpp"

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
        double rsi = computeRSI(cryptoRate, RSI_PERIOD);
        interpretRSI(rsi, crypto.first);
        std::cout << std::endl;

    }
    std::cout << "Portfolio performance: $" << portfolioResult << std::endl;
    portfolioStream.close();
    return 0;
}
