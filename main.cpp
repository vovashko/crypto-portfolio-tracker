#include "CryptoExchange.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <portfolio_file> <crypto_pairs.json>" << std::endl;
        return 1;
    }

    std::string portfolioFile = argv[1];
    std::string jsonFile = argv[2];

    // Load crypto pairs from JSON
    std::vector<std::string> cryptoFiles = loadCryptoPairs(jsonFile);
    if (cryptoFiles.empty())
    {
        std::cerr << "No crypto pairs loaded. Exiting." << std::endl;
        return 1;
    }

    // Load all crypto price data
    std::map<std::string, std::map<std::string, double>> cryptoPrices = loadCryptoPrices(cryptoFiles);

    std::ifstream portfolioStream(portfolioFile);
    if (!portfolioStream.is_open())
    {
        std::cerr << "Error: Unable to open portfolio file: " << portfolioFile << std::endl;
        return 1;
    }

    // Process portfolio for each crypto
    for (const auto &crypto : cryptoPrices)
    {
        std::cout << "Processing portfolio for: " << crypto.first << std::endl;
        printPortfolio(crypto.second, portfolioStream, 1);
    }

    portfolioStream.close();
    return 0;
}
