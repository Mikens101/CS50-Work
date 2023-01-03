import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    stocks = db.execute("SELECT symbol, name, SUM(numShares) as total, sharePrice \
                        FROM transactions WHERE userID = ? GROUP BY symbol", session["user_id"])
    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]['username']
    db.execute("DELETE FROM active WHERE userID = ?", session['user_id'])

    bankTotal = cash
    for stock in stocks:
        symbol = stock['symbol']
        price = lookup(symbol)['price']
        num = stock['total']
        name = stock['name']

        db.execute("INSERT INTO active (userID, symbol, name, numShares, sharePrice) \
                    VALUES(?, ?, ?, ?, ?)", session['user_id'], symbol, name, num, price)
        bankTotal += price * num

    active = db.execute("SELECT symbol, name, numShares, sharePrice\
                        FROM active WHERE userID = ? AND numShares <> 0", session['user_id'])

    return render_template("index.html", username=username, stocks=active, cash=cash, usd=usd, bankTotal=bankTotal)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    else:
        symbol = request.form.get("symbol").upper()
        numShares = request.form.get("shares")

        # Check all possible errors
        if (symbol == ""):
            return apology("Missing Symbol")
        quote = lookup(symbol)
        if not quote:
            return apology("Invalid Symbol")
        if (numShares == ""):
            return apology("Missing Shares")
        if not numShares.isdigit():
            return apology("Invalid share quantity")

        # If no erros with input, check purchase amount
        quote = lookup(symbol)
        user = session["user_id"]
        totalBuy = quote["price"] * int(numShares)
        totalStart = db.execute("SELECT cash FROM users WHERE id = ?", user)[0]['cash']

        if (totalBuy > totalStart):
            return apology("Not enough money in account")
        else:
            sharePrice = quote["price"]
            transactionType = "buy"
            bankTotal = totalStart - totalBuy
            name = quote["name"]

            db.execute("INSERT INTO transactions(userID, symbol, name, numShares, sharePrice, transactionType) \
                        VALUES(?, ?, ?, ?, ?, ?)", user, symbol, name, numShares, sharePrice, transactionType)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", bankTotal, user)
            flash("Bought!")

        # Output of data
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT symbol, name, numShares, sharePrice, transactionType, datetime \
                                FROM transactions WHERE userID = ?", session['user_id'])
    return render_template("history.html", transactions=transactions, usd=usd)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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

    if request.method == 'GET':
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")

        if symbol == "":
            return apology("Missing Symbol")

        quote = lookup(symbol)
        if not quote:
            return apology("Invalid Symbol")

        return render_template("quoted.html", symbol=quote["symbol"], name=quote["name"], price=usd(quote["price"]))


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == 'POST':
        # User enters: username, password + confirmation
        username = request.form.get("username")
        nameCheck = db.execute("SELECT username FROM users WHERE username = ?", username)
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        passHash = generate_password_hash(password)

        # Check conditions
        if (len(nameCheck) > 0) or (username == ""):
            return apology("Username already exists. Please try another.")
        if (password == ""):
            return apology("Please enter a password")
        if (password != confirmation):
            return apology("Passwords do not match!")

        # Add user to database
        db.execute("INSERT INTO users(username, hash) VALUES (?,?)", username, passHash)
        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == 'GET':
        symbols = db.execute("SELECT symbol FROM active WHERE userID = ? AND numShares <> 0 GROUP BY symbol", session['user_id'])
        return render_template("sell.html", symbols=symbols)
    else:
        # Check inputs
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Missing Symbol")
        shares = request.form.get("shares")
        name = lookup(symbol)['name']
        if (shares == "") or (int(shares) <= 0):
            return apology("Invalid share quantity")

        # Check if numShares trying to be sold is less than owned
        price = lookup(symbol)['price']
        own = db.execute("SELECT numShares FROM active WHERE userID = ? AND symbol = ? \
                        GROUP BY symbol", session['user_id'], symbol)[0]['numShares']
        if own < int(shares):
            return apology("Too many shares selected")

        # Update users Bank Account
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session['user_id'])[0]['cash']
        totalPrice = int(shares) * price
        bankTotal = cash + totalPrice

        db.execute("UPDATE users SET cash = ? WHERE id = ?", bankTotal, session['user_id'])
        db.execute("INSERT INTO transactions (userID, symbol, name, numShares, sharePrice, transactionType) \
                    VALUES(?, ?, ?, ?, ?, ?)", session['user_id'], symbol, name, -int(shares), price, "sell")
        flash("Sold!")
        return redirect("/")

