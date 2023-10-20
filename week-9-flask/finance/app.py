import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]
    stocks = db.execute(
        "SELECT stock, quantity FROM wallets WHERE user_id = ?;", user_id
    )
    cash_lookup = db.execute("SELECT cash FROM users WHERE id = ?;", user_id)
    cash = cash_lookup[0]["cash"]
    finaltotal = cash
    lookups = {}

    for dictionary in stocks:
        pricecheck = lookup(dictionary["stock"])
        price = pricecheck["price"]
        finaltotal += price * dictionary["quantity"]
        lookups[dictionary["stock"]] = price

    return render_template(
        "index.html", stocks=stocks, lookups=lookups, cash=cash, finaltotal=finaltotal
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")

    else:
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Fields cannot be blank")

        elif not request.form.get("shares").isdecimal():
            return apology("'Shares' field must include a whole number")

        else:
            try:
                lookup_dict = lookup(request.form.get("symbol"))
                stock_symbol = lookup_dict["symbol"]
                share_price = lookup_dict["price"]
                shares = int(request.form.get("shares"))
                final_price = share_price * float(shares)
                user_id = session["user_id"]
                rows = db.execute("SELECT cash FROM users WHERE id = ?;", user_id)
                cash = rows[0]["cash"]

            except TypeError:
                return apology("Please input a valid symbol")

            if cash < final_price:
                return apology("Not enough cash to purchase shares")

            new_cash = cash - final_price

            rows = db.execute(
                "SELECT stock, quantity FROM wallets WHERE user_id = ?;", user_id
            )

            for dictionary in rows:
                stock = dictionary["stock"]
                quantity = dictionary["quantity"]
                new_quantity = quantity + shares

                if stock == stock_symbol:
                    db.execute(
                        "UPDATE wallets SET quantity = ? WHERE stock = ? AND user_id = ?;",
                        new_quantity,
                        stock,
                        user_id,
                    )
                    db.execute(
                        "INSERT INTO transactions (user_id, type, quantity, stock, price, datetime) VALUES (?, 'BOUGHT', ?, ?, ?, ?);",
                        user_id,
                        shares,
                        stock_symbol,
                        final_price,
                        datetime.datetime.now(),
                    )
                    db.execute(
                        "UPDATE users SET cash = ? WHERE id = ?;", new_cash, user_id
                    )

                    flash("Transaction Completed Successfully")
                    return redirect("/")

            db.execute(
                "INSERT INTO wallets (user_id, stock, quantity) VALUES (?, ?, ?);",
                user_id,
                stock_symbol,
                shares,
            )
            db.execute(
                "INSERT INTO transactions (user_id, type, quantity, stock, price, datetime) VALUES (?, 'BOUGHT', ?, ?, ?, ?);",
                user_id,
                shares,
                stock_symbol,
                final_price,
                datetime.datetime.now(),
            )
            db.execute("UPDATE users SET cash = ? WHERE id = ?;", new_cash, user_id)

            flash("Transaction Completed Successfully")
            return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user_id = session["user_id"]
    transactions_table = db.execute(
        "SELECT * FROM transactions WHERE user_id = ?;", user_id
    )

    return render_template("history.html", transactions_table=transactions_table)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "GET":
        return render_template("quote.html")

    else:
        try:
            symbol = request.form.get("symbol")
            stock_quote = lookup(symbol)
            name = stock_quote["name"]
            price = stock_quote["price"]
            return render_template("quoted.html", name=name, price=price)

        except TypeError:
            return apology("Input a valid symbol")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        password_confirm = request.form.get("confirmation")
        password_hash = generate_password_hash(password)

        if not request.form.get("username"):
            return apology("Username cannot be blank")

        elif not request.form.get("password"):
            return apology("Password cannot be blank")

        elif password != password_confirm:
            return apology("Passwords do not match")

        else:
            try:
                db.execute(
                    "INSERT INTO users (username, hash) VALUES (?, ?);",
                    username,
                    password_hash,
                )

            except ValueError:
                return apology("Username already in use")

            return login()

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Symbol field cannot be blank")

        symbol = request.form.get("symbol").upper()

        try:
            stock_quote = lookup(symbol)

        except TypeError:
            return apology("Input a valid stock")

        user_id = session["user_id"]
        owned_stock = []
        stock_dicts = db.execute(
            "SELECT stock FROM wallets WHERE user_id = ?;", user_id
        )

        for dictionary in stock_dicts:
            owned_stock.append(dictionary["stock"])

        if not symbol in owned_stock:
            return apology("Input a stock you own")

        stock_dicts = db.execute(
            "SELECT quantity FROM wallets WHERE stock = ? AND user_id = ?;",
            symbol,
            user_id,
        )
        owned_shares = stock_dicts[0]["quantity"]
        shares = int(request.form.get("shares"))

        if shares > owned_shares:
            return apology("Cannot sell more stock than you own")

        cash_dict = db.execute("SELECT cash FROM users WHERE id = ?;", user_id)
        cash = cash_dict[0]["cash"]
        price = stock_quote["price"]
        total_price = price * float(shares)
        updated_shares = owned_shares - shares
        new_cash = cash + total_price

        db.execute(
            "UPDATE wallets SET quantity = ? WHERE stock = ? AND user_id = ?;",
            updated_shares,
            symbol,
            user_id,
        )
        db.execute(
            "INSERT INTO transactions (user_id, type, quantity, stock, price, datetime) VALUES (?, 'SOLD', ?, ?, ?, ?);",
            user_id,
            shares,
            symbol,
            total_price,
            datetime.datetime.now(),
        )
        db.execute("UPDATE users SET cash = ? WHERE id = ?;", new_cash, user_id)

        flash("Transaction Completed Successfully")
        return redirect("/")

    else:
        user_id = session["user_id"]
        owned_stock = []
        stock_dicts = db.execute(
            "SELECT stock FROM wallets WHERE user_id = ?;", user_id
        )

        for dictionary in stock_dicts:
            owned_stock.append(dictionary["stock"])

        return render_template("sell.html", owned_stock=owned_stock)


@app.route("/changepass", methods=["GET", "POST"])
@login_required
def changepass():
    if request.method == "GET":
        return render_template("changepass.html")

    else:
        if (
            not request.form.get("password")
            or not request.form.get("new_password")
            or not request.form.get("confirmation")
        ):
            return apology("none of the fields may be blank")

        user_id = session["user_id"]
        old_password = request.form.get("password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")
        rows = db.execute("SELECT hash FROM users WHERE id = ?;", user_id)
        password_hash = rows[0]["hash"]

        if not check_password_hash(password_hash, old_password):
            return apology("Password incorrect")

        elif old_password == new_password:
            return apology("New password cannot be the same as your current password")

        elif new_password != confirmation:
            return apology("New password and confirmation do not match")

        else:
            new_hash = generate_password_hash(new_password)

            db.execute("UPDATE users SET hash = ? WHERE id = ?;", new_hash, user_id)

            flash("Password Updated")
            return redirect("/")
