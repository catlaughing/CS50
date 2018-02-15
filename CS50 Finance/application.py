from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
    user = rows[0]["username"]
    stocks = db.execute(
        "SELECT symbol, shares FROM portofolio WHERE userid = :id", id=session["user_id"])

    if not stocks:
        return apology("you still not buy anything", 200)

    prices = {}
    total_price = {}
    names = {}
    total = rows[0]["cash"]
    grand_total = 0

    for stock in stocks:
        temp = lookup(stock["symbol"])
        prices[stock["symbol"]] = temp["price"]
        names[stock["symbol"]] = temp["name"]
        total_price[stock["symbol"]] = usd(prices[stock["symbol"]] * stock["shares"])
        grand_total += (prices[stock["symbol"]] * stock["shares"])

    grand_total += total

    return render_template("index.html", user=user, stocks=stocks, grand_total=usd(grand_total), total=usd(total), total_price=total_price, prices=prices, names=names)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        stock = lookup(symbol)

        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure password was submitted
        elif not request.form.get("shares"):
            return apology("must provide shares", 403)

        if stock == None:
            return apology("Symbol not found")

        try:
            int(shares)
        except:
            return apology("Shares must be a positive integers")

        if int(shares) < 0:
            return apology("Shares must be a positive integers")

        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])

        total = stock["price"] * int(shares)

        if total > int(rows[0]["cash"]):
            return apology('Cash not enough')

        db.execute("INSERT INTO transactions (userid, symbol, shares, price, total, buy_or_sell) VALUES(:userid, :symbol, :shares, :price, :total, :buy_or_sell)", userid=session["user_id"], price=stock["price"],
            shares=shares, total=total, symbol=symbol, buy_or_sell="buy")

        port = db.execute("INSERT INTO portofolio (userid, symbol, shares) VALUES(:userid, :symbol, :shares)",
                    userid=session["user_id"], symbol=symbol, shares=shares)

        if port == None:
            db.execute("UPDATE portofolio SET shares = shares + :shares WHERE userid = :id",
                    shares=shares, id=session["user_id"])

        db.execute("UPDATE users SET cash = cash - :price WHERE id = :id",
                price=total, id=session["user_id"])

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    results = db.execute("SELECT * FROM transactions WHERE userid = :id", id=session["user_id"])
    for result in results:
        result["price"] = usd(result["price"])

    return render_template("history.html", results=results)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("Please insert symbol")

        result = lookup(symbol)

        if result == None:
            return apology("Symbol not found")

        price = usd(result["price"])

        return render_template("quoted.html",name=result["name"],price=price,symbol=result["symbol"])

    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirm = request.form.get("confirmation")

        if not username or not password or not confirm:
            return apology("Missing Username!",400)

        if (password != confirm):
            return apology("Password and Confirmatin Password are not the same")

        hash_pass = generate_password_hash(password)
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=username, hash=hash_pass)

        if not result:
            return apology("Name is already taken",400)

        rows = db.execute("SELECT * FROM users WHERE username = :username",
                    username=request.form.get("username"))

        session["user_id"] = rows[0]["id"]

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        stocks = db.execute("SELECT symbol, shares FROM portofolio WHERE userid = :id",id=session["user_id"])

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("Wrong Symbol")

        if (int(shares) < 0):
            return apology("Shaares must be more than 0")

        temp = lookup(symbol)
        total_price = temp["price"] * int(shares)

        for stock in stocks:
            if symbol == stock["symbol"]:
                if int(shares) > int(stock["shares"]):
                    return apology("Not enough shares")

                elif int(shares) == int(stock["shares"]):
                    db.execute("DELETE FROM portofolio WHERE userid = :id AND symbol = :symbol",
                            symbol=symbol, id=session["user_id"])
                    db.execute("INSERT INTO transactions (userid, symbol, shares, price, total, buy_or_sell) VALUES(:userid, :symbol, :shares, :price, :total, :buy_or_sell)",
                         userid=session["user_id"], price=temp["price"],shares=shares, total=total_price, symbol=symbol, buy_or_sell="sell")

                else:
                    db.execute("UPDATE portofolio SET shares = shares - :shares WHERE userid = :id AND symbol = :symbol",
                            shares=shares, id=session["user_id"], symbol=symbol)
                    db.execute("UPDATE users SET cash = cash + :price WHERE id = :id",
                            price=total_price, id=session["user_id"])
                    db.execute("INSERT INTO transactions (userid, symbol, shares, price, total, buy_or_sell) VALUES(:userid, :symbol, :shares, :price, :total, :buy_or_sell)", userid=session["user_id"], price=total_price,
                            shares=shares, total=total_price, symbol=symbol, buy_or_sell="sell")

        return redirect("/")
    else:
        stocks = db.execute("SELECT symbol, shares FROM portofolio WHERE userid = :id",id=session["user_id"])
        return render_template("sell.html",stocks=stocks)

@app.route("/chpass", methods=["GET", "POST"])
@login_required
def chpass():
    """Register user"""
    if request.method == "POST":
        user = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        old = request.form.get("old-password")
        new = request.form.get("new-password")
        confirm = request.form.get("confirmation")

        if not old or not new or not confirm:
            return apology("Please fill every form")

        if not check_password_hash(user[0]["hash"], old):
            return apology("Wrong password")

        if new != confirm:
            return apology("Confirmation and password is not same")

        hash = generate_password_hash(new)

        db.execute("UPDATE users SET hash = :hash WHERE id = :id", hash=hash, id=session["user_id"])

        return redirect("/")
    else:
        return render_template("chpass.html")

def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
