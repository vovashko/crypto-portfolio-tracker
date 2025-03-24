#include "CryptoPortfolio.hpp"

void trimString(std::string &str)
{
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);
}

// Function to check if a year is a leap year
bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool verifyYear(const std::string &yearStr)
{
    int year;
    try
    {
        year = std::stoi(yearStr);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Invalid year: " << yearStr << std::endl;
        return false;
    } catch (const std::out_of_range& e) {
        std::cout << "Year out of range: " << yearStr << std::endl;
        return false;
    }
    if (year < 2009)
    {
        std::cout << "Invalid year: " << year << std::endl;
        return false;
    }
    return true;
}

bool verifyMonth(const std::string &monthStr)
{
    int month;
    try
    {
        month = std::stoi(monthStr);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Invalid month: " << monthStr << std::endl;
        return false;
    } catch (const std::out_of_range& e) {
        std::cout << "Month out of range: " << monthStr << std::endl;
        return false;
    }
    if (month < 1 || month > 12)
    {
        std::cout << "Invalid month: " << month << std::endl;
        return false;
    }
    return true;
}

bool verifyDay(const std::string &dayStr, const int month, const int year)
{
    int day;
    try
    {
        day = std::stoi(dayStr);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Invalid day: " << dayStr << std::endl;
        return false;
    } catch (const std::out_of_range& e) {
        std::cout << "Day out of range: " << dayStr << std::endl;
        return false;
    }
    std::map<int, int> daysInMonth = {
        {1, 31}, {2, 28}, {3, 31}, {4, 30}, {5, 31}, {6, 30},
        {7, 31}, {8, 31}, {9, 30}, {10, 31}, {11, 30}, {12, 31}
    };
    if (isLeapYear(year))
        daysInMonth[2] = 29;
    if (day < 1 || day > daysInMonth[month])
    {
        std::cout << "Invalid day: " << day << std::endl;
        return false;
    }
    return true;
}

// Function to validate a date
bool verifyDate(const std::string &date)
{
    std::string year, month, day;
    std::istringstream ss(date);

    if (getline(ss, year, '-') && getline(ss, month, '-') && getline(ss, day))
    {
        if (year.size() != 4 || month.size() != 2 || day.size() != 2)
        {
            std::cout << "Invalid date format: " << date << std::endl;
            return false;
        }
        if (!verifyYear(year))
            return false;
        if (!verifyMonth(month))
            return false;
        if (!verifyDay(day, std::stoi(month), std::stoi(year)))
            return false;
    }
    else
    {
        std::cout << "Invalid date format: " << date << std::endl;
        return false;
    }
    return true;
}

// Function to validate value
bool verifyValue(const std::string &valueStr)
{
    int amount;
    try
    {
        amount = std::stoi(valueStr);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Invalid value: " << valueStr << std::endl;
        return false;
    } catch (const std::out_of_range& e) {
        std::cout << "Value out of range: " << valueStr << std::endl;
        return false;
    }
    if (amount < 0)
    {
        std::cout << "Invalid value: " << valueStr << std::endl;
        return false;
    }
    return true;
}

// Function to process the file
std::map<std::string, double> processFileAndBuildMap(std::ifstream &file, char separator, int containsHeader)
{
    std::map<std::string, double> priceMap;
    std::string line;

    // Skip header if present
    if (containsHeader && getline(file, line))
        containsHeader--;

    while (getline(file, line))
    {   
        std::istringstream ss(line);
        std::string date, open, high, low, close, volume;

        if (getline(ss, date, separator) &&
            getline(ss, open, separator) &&
            getline(ss, high, separator) &&
            getline(ss, low, separator) &&
            getline(ss, close, separator) &&
            getline(ss, volume))
        {
            trimString(date);
            trimString(close);

            if (!verifyDate(date))
                continue;

            try
            {
                double closePrice = std::stod(close);
                priceMap[date] = closePrice;
            }
            catch (const std::exception &e)
            {
                std::cout << "Invalid price data on " << date << ": " << close << std::endl;
            }
        }
    }   
    return priceMap;
}


double printPortfolio(std::map<std::string, double> cryptoRate, std::ifstream &InFile, int containsHeader, std::string targetPair)
{
    std::cout << "Portfolio for " << targetPair << std::endl;
    std::string line;
    double result = 0;
    while (getline(InFile, line))
    {
        if (containsHeader)
            containsHeader--;
        else
        {
            std::istringstream ss(line);
            std::string cryptoPair, valueStr, purchaseDate;
            if (getline(ss, cryptoPair, ',') && getline(ss, valueStr, ',') && getline(ss, purchaseDate))
            {
                trimString(cryptoPair);
                trimString(valueStr);
                trimString(purchaseDate);

                if (!verifyValue(valueStr) || (PATH_TO_DATA + cryptoPair) != targetPair)
                    continue;

                double amount = std::stod(valueStr);

                // Get purchase price
                std::string closestPurchaseDate = findClosestLowerDate(cryptoRate, purchaseDate);
                double purchasePrice = cryptoRate[closestPurchaseDate];

                // Get latest price (last entry in cryptoRate)
                auto latestIt = cryptoRate.rbegin();
                double latestPrice = latestIt->second;

                // Calculate profit/loss
                double profitLoss = (latestPrice - purchasePrice) * amount;

                std::cout << cryptoPair << " => " << amount << " Units = ($" 
                          << amount * latestPrice << ") | P/L: $" << profitLoss << std::endl;
                result += profitLoss;
            }
            else
                std::cout << "Invalid line => " << line << std::endl;
        }
    }
    return result;
}


std::string findClosestLowerDate(std::map<std::string, double> database, std::string date)
{
    std::string closestDate;
    for (auto it = database.begin(); it != database.end(); it++)
    {
        if (it->first < date)
            closestDate = it->first;
    }
    return closestDate;
}

std::vector<std::string> loadCryptoPairs(const std::string &csvFile, int containsHeader)
{
    std::ifstream file(csvFile);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open csv file: " << csvFile << std::endl;
        return {};
    }
    std::string line;
    std::vector<std::string> cryptoPairs;
    while (getline(file, line))
    {
        if (containsHeader)
            containsHeader--;
        else
        {
        std::istringstream ss(line);
        std::string pair, value;
        if (getline(ss, pair, ','))
        {
            trimString(pair);
            cryptoPairs.push_back(PATH_TO_DATA + pair + ".csv");
        }
        }
    }
    return cryptoPairs;
}

std::map<std::string, std::map<std::string, double>> loadCryptoPrices(const std::vector<std::string> &cryptoFiles)
{
    std::map<std::string, std::map<std::string, double>> cryptoPrices;

    for (const std::string &filename : cryptoFiles)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cout << "Failed to open file: " << filename << std::endl;
            continue;
        }

        std::map<std::string, double> prices = processFileAndBuildMap(file, ',', 1);
        if (prices.empty())
        {
            std::cout << "No valid data found in file " << filename << "\n" << std::endl;
            file.close();
            continue;
        }
        std::string cryptoSymbol = filename.substr(0, filename.find('.')); // Extract symbol from filename
        cryptoPrices[cryptoSymbol] = prices;

        file.close();
    }

    return cryptoPrices;
}


