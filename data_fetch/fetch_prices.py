import requests
import csv
import os
import json
from datetime import datetime

# Constants
STEP = 86400  # 1-day candles
LIMIT = 1000  # Last 1000 days
DATA_FOLDER = "ohlc_data"  # Folder to store CSV files
CURRENCY_PAIRS_FILE = "../portfolio.csv"  # File to store currency pairs list

def load_currency_pairs():
    """Load currency pairs from a csv file."""
    with open(CURRENCY_PAIRS_FILE, newline='') as csvfile:
        reader = csv.reader(csvfile)
        currency_pairs = set()
        count = 0
        for row in reader:
            if count == 0:
                count += 1
                continue
            currency_pairs.add(row[0])
        return currency_pairs
        

def fetch_bitstamp_ohlc_data(currency_pair):
    """Fetch OHLC data for a given currency pair."""
    url = f'https://www.bitstamp.net/api/v2/ohlc/{currency_pair}/'
    params = {'step': STEP, 'limit': LIMIT}
    try:
        response = requests.get(url, params=params)
        response.raise_for_status()
    except requests.exceptions.HTTPError as err:
        print(f"Error fetching data for {currency_pair}: {err}")
        return []
    data = response.json()
    return data['data']['ohlc']

def save_to_csv(data, currency_pair):
    """Save OHLC data to a CSV file inside the 'ohlc_data' folder."""
    if not os.path.exists(DATA_FOLDER):
        os.makedirs(DATA_FOLDER)  # Create folder if it doesn't exist

    filename = os.path.join(DATA_FOLDER, f'{currency_pair}.csv')
    file_exists = os.path.isfile(filename)
    """If file exists, open it in write mode to overwrite the existing data. Otherwise it will create a new file."""
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        if not file_exists:
            writer.writerow(['Date', 'Open', 'High', 'Low', 'Close', 'Volume'])  # Header row
        for entry in data:
            formatted_date = datetime.fromtimestamp(int(entry['timestamp'])).strftime('%Y-%m-%d')
            writer.writerow([formatted_date, entry['open'], entry['high'], entry['low'], entry['close'], entry['volume']])

if __name__ == "__main__":
    currency_pairs = load_currency_pairs()
    for pair in currency_pairs:
        print(f"Fetching data for {pair}...")
        ohlc_data = fetch_bitstamp_ohlc_data(pair)
        save_to_csv(ohlc_data, pair)
        print(f"Saved {pair} data to {DATA_FOLDER}/{pair}.csv")
