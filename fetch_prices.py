import requests
import json
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

if __name__ == "__main__":
    prices = fetch_prices()
    if prices:
        with open("crypto_prices.json", "w") as file:
            json.dump(prices, file, indent=4)
        print("Prices saved to crypto_prices.json")
