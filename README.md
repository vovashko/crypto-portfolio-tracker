# **Crypto-Portfolio-Tracker**  

This tool allows you to check the performance of your cryptocurrency portfolio and provides buy/sell advice based on the [Relative Strength Index (RSI)](https://www.investopedia.com/terms/r/rsi.asp).  

---

## **How It Works**  

### **0. Prepare Your Portfolio**  
Each module of the program expects a portfolio file. This project is designed with the **BitStamp API** in mind, so cryptocurrency pairs should be formatted accordingly (e.g., the valid pair for **Bitcoin** would be `btcusd`).  

A sample portfolio file is included in the repository with the expected formatting.  

### **1. Fetch Price Data**  
If you're running the program for the first time:  
- Navigate to the `data_fetch` folder and run:  
  ```bash
  python fetch_prices.py
  ```  

If you have previously fetched prices and want to update them:  
- Run the second script to append new data:  
  ```bash
  python update_ohlc.py
  ```  
- To automate this process, you can set up a **cron job** on your computer to update prices at regular intervals. Example:  
  ```bash
  0 0 * * * python /path/to/update_ohlc.py  # Runs daily at midnight
  ```  

### **2. Run the Crypto Portfolio Analyzer**  
In the root folder of the repository, run:  
```bash
make
```  
This will compile the program and create an executable:  
```bash
./crypto-portfolio-analyzer
```  
To run the program:  
```bash
./crypto-portfolio-analyzer path/to/your/portfolio.csv
```  
Replace `path/to/your/portfolio.csv` with the actual location of your portfolio file.  

---

## **Customization Options**  

The program provides various parameters that can be customized to fit your needs.  

### **Python Script (data_fetch) Customization**  
The following constants can be modified in `fetch_prices.py` and `update_ohlc.py`:  
```python
# Constants
STEP = 86400  # 1-day candles
LIMIT = 1000  # Last 1000 days
DATA_FOLDER = "ohlc_data"  # Folder to store CSV files
CURRENCY_PAIRS_FILE = "../portfolio.csv"  # File to store currency pairs list
```
- **STEP**: Timeframe of each price candle (in seconds). Defaults to **daily (86400 sec)**.  
- **LIMIT**: Number of candles to fetch. Defaults to **last 1000 days**.  
- **DATA_FOLDER**: Folder where CSV files will be stored.  
- **CURRENCY_PAIRS_FILE**: Path to the portfolio file containing currency pairs.  

### **C++ Program Customization**  
These macros can be adjusted in `CryptoPortfolio.hpp`:  
```cpp
#define PATH_TO_DATA "data_fetch/ohlc_data/"
#define RSI_PERIOD 14
#define HEADER 1
```
- **PATH_TO_DATA**: Location where the OHLC data is stored.  
- **RSI_PERIOD**: Number of periods used to compute the RSI indicator. Defaults to **14**.  
- **HEADER**: Set to **1** if the CSV file includes a header row, **0** otherwise.  

---

## **Program Limitations**  
- **Only long (buy) positions are considered.**  
  - The portfolio file should contain only positive numbers for cryptocurrency amounts.  
  - **Sell transactions are ignored** in calculations.  

If you need short (sell) positions to be considered, you will need to modify the program logic accordingly.  

---

## **Bonus: Using Your Own Data**  
If you already have historical price data:  
- Update the file path in `CryptoPortfolio.hpp` to point to your data.  
- The program expects the following **CSV format**:  
  ```
  Date,Open,High,Low,Close,Volume
  ```  
- If your data is structured differently, you can either **reformat it** or modify the code to handle your format.  

---

### **Next Steps**  
- Improve analysis by integrating additional indicators.  
- Expand support for more exchanges.  
