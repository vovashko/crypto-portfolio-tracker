import requests
import csv
import os
import json
from datetime import datetime

# Constants
STEP = 86400  # 1-day candles
LIMIT = 1  # Only fetch the latest day
DATA_FOLDER = "ohlc_data"
CURRENCY_PAIRS_FILE = "currency_pairs.json"

def load_currency_pairs():
    """Load currency pairs from a JSON file."""
    if not os.path.exists(CURRENCY_PAIRS_FILE):
        return ["btcusd", "ethusd", "xrpusd"]  # Default pairs
    with open(CURRENCY_PAIRS_FILE, "r") as file:
        return json.load(file)

def fetch_bitstamp_ohlc_data(currency_pair):
    """Fetch latest OHLC data for a given currency pair."""
    url = f'https://www.bitstamp.net/api/v2/ohlc/{currency_pair}/'
    params = {'step': STEP, 'limit': LIMIT}
    response = requests.get(url, params=params)
    data = response.json()
    return data['data']['ohlc']

def get_last_date_from_csv(filename):
    """Check the last recorded date in the CSV file."""
    if not os.path.exists(filename):
        return None
    with open(filename, mode='r') as file:
        reader = list(csv.reader(file))
        if len(reader) > 1:
            return reader[-1][0]  # Last row, first column (Date)
    return None

def append_to_csv(data, currency_pair):
    """Append only new data to the CSV file."""
    if not os.path.exists(DATA_FOLDER):
        os.makedirs(DATA_FOLDER)

    filename = os.path.join(DATA_FOLDER, f'{currency_pair}_ohlc_data.csv')
    last_date = get_last_date_from_csv(filename)

    with open(filename, mode='a', newline='') as file:
        writer = csv.writer(file)
        if last_date is None:  # If file is empty, write headers
            writer.writerow(['Date', 'Open', 'High', 'Low', 'Close', 'Volume'])
        
        for entry in data:
            formatted_date = datetime.utcfromtimestamp(int(entry['timestamp'])).strftime('%Y-%m-%d')
            if formatted_date != last_date:  # Append only if new
                writer.writerow([formatted_date, entry['open'], entry['high'], entry['low'], entry['close'], entry['volume']])
                print(f"Added new data for {currency_pair}: {formatted_date}")

if __name__ == "__main__":
    currency_pairs = load_currency_pairs()
    for pair in currency_pairs:
        print(f"Fetching latest data for {pair}...")
        ohlc_data = fetch_bitstamp_ohlc_data(pair)
        append_to_csv(ohlc_data, pair)
