import requests
import json
import csv
import os
from dotenv import load_dotenv

# Load environment variables from .env file
load_dotenv()

# Get API key and URL from .env
API_KEY = os.getenv("API_KEY")
URL = os.getenv("CMC_URL")

PARAMS = {
    "start": "1",
    "limit": "10",  # Adjust based on needs
    "convert": "EUR",
}

HEADERS = {
    "Accepts": "application/json",
    "X-CMC_PRO_API_KEY": API_KEY,
}

def fetch_prices():
    """Fetch latest cryptocurrency prices from CoinMarketCap API."""
    try:
        response = requests.get(URL, headers=HEADERS, params=PARAMS)
        response.raise_for_status()  # Raise HTTP errors if any
        return response.json()
    except requests.exceptions.RequestException as e:
        print(f"Error fetching data: {e}")
        return None

def save_to_csv(prices, filename="crypto_prices.csv"):
    """Save cryptocurrency prices to a CSV file."""
    if "data" not in prices:
        print("Invalid data format received.")
        return

    with open(filename, mode="w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["Symbol", "Name", "Price (EUR)"])  # CSV Header

        for crypto in prices["data"]:
            symbol = crypto["symbol"]
            name = crypto["name"]
            price = round(crypto["quote"]["EUR"]["price"], 2)  # Rounded for cleaner output
            writer.writerow([symbol, name, price])

    print(f"CSV file generated: {filename}")

if __name__ == "__main__":
    prices = fetch_prices()
    if prices:
        save_to_csv(prices)
        print("Data fetched and saved successfully.")
    else:
        print("Failed to fetch data.")

