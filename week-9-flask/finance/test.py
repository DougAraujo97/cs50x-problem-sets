from cs50 import SQL

db = SQL("sqlite:///finance.db")

user_id = 1
stock_dicts = db.execute("SELECT stock FROM wallets WHERE user_id = ?;", user_id)
owned_stock = []
for dictionary in stock_dicts:
    owned_stock.append(dictionary["stock"])

print(owned_stock)
