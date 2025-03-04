#ifndef CRYPTOEXCHANGE_HPP
#define CRYPTOEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#define DATABASE "crypto_prices.json"
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <json/json.h> // Requires jsoncpp library

// input validation
void trimString(std::string &str);
bool isLeapYear(int year);
bool verifyDate(const std::string &date);
bool verifyYear(const std::string &yearStr);
bool verifyMonth(const std::string &monthStr);
bool verifyDay(const std::string &dayStr, const int month, const int year);
bool verifyValue(const std::string &valueStr);

// file processing
std::map<std::string, double> processFileAndBuildMap(std::ifstream &file, char separator, int containsHeader);
void printPortfolio(std::map<std::string, double> btcRate, std::ifstream &InFile, int containsHeader);
std::string findClosestLowerDate(std::map<std::string, double> database, std::string date);
std::vector<std::string> loadCryptoPairs(const std::string &jsonFile);
std::map<std::string, std::map<std::string, double>> loadCryptoPrices(const std::vector<std::string> &cryptoFiles);

#endif
