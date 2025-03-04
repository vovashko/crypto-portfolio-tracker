import requests
import csv
import os
import json
from datetime import datetime

# Constants
STEP = 86400  # 1-day candles
LIMIT = 30  # Last 30 days
DATA_FOLDER = "ohlc_data"  # Folder to store CSV files
CURRENCY_PAIRS_FILE = "currency_pairs.json"  # File to store currency pairs list

def load_currency_pairs():
    """Load currency pairs from a JSON file."""
    if not os.path.exists(CURRENCY_PAIRS_FILE):
        return ["btcusd", "ethusd", "xrpusd"]  # Default pairs
    with open(CURRENCY_PAIRS_FILE, "r") as file:
        return json.load(file)

def fetch_bitstamp_ohlc_data(currency_pair):
    """Fetch OHLC data for a given currency pair."""
    url = f'https://www.bitstamp.net/api/v2/ohlc/{currency_pair}/'
    params = {'step': STEP, 'limit': LIMIT}
    response = requests.get(url, params=params)
    data = response.json()
    return data['data']['ohlc']

def save_to_csv(data, currency_pair):
    """Save OHLC data to a CSV file inside the 'ohlc_data' folder."""
    if not os.path.exists(DATA_FOLDER):
        os.makedirs(DATA_FOLDER)  # Create folder if it doesn't exist

    filename = os.path.join(DATA_FOLDER, f'{currency_pair}_ohlc_data.csv')
    file_exists = os.path.isfile(filename)

    with open(filename, mode='a', newline='') as file:
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
        print(f"Saved {pair} data to {DATA_FOLDER}/{pair}_ohlc_data.csv")
