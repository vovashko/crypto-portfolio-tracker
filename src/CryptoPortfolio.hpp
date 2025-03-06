#ifndef CRYPTOPORTFOLIO_HPP
#define CRYPTOPORTFOLIO_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>

#define PATH_TO_DATA "data_fetch/ohlc_data/"
#define RSI_PERIOD 14
#define HEADER 1

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
double printPortfolio(std::map<std::string, double> cryptoRate, std::ifstream &InFile, int containsHeader, std::string targetPair);
std::string findClosestLowerDate(std::map<std::string, double> database, std::string date);
std::vector<std::string> loadCryptoPairs(const std::string &csvFile, int containsHeader);
std::map<std::string, std::map<std::string, double>> loadCryptoPrices(const std::vector<std::string> &cryptoFiles);

// RSI computation
double computeRSI(const std::map<std::string, double> &cryptoRate, unsigned long period);
void interpretRSI(double rsi, const std::string &cryptoSymbol);

#endif
