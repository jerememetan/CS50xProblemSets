import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
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
    if "alert" in session:
        alert = session["alert"]
    else:
        alert = None
    userid = session["user_id"]
    # show all stocks user currently have
    rows = db.execute(
        "SELECT * FROM userstock JOIN stock ON userstock.stock_id = stock.id JOIN stock_price ON userstock.stock_id = stock_price.stock_id WHERE user_id = ?",
        userid,
    )
    userinfo = db.execute("SELECT * FROM users WHERE id = ?", userid)
    value = userinfo[0]["cash"]
    ownedstocks = []
    for row in rows:
        row["price"] = lookup(row["name"])["price"]
        db.execute(
            "UPDATE stock_price SET price = ? WHERE stock_id = ?",
            row["price"],
            row["stock_id"],
        )
        entry = {
            "stocks": row["name"],
            "amount": row["amount"],
            "price": usd(row["price"]),
            "total": usd(float(lookup(row["name"])["price"]) * int(row["amount"])),
        }
        ownedstocks.append(entry)
        value += float(row["price"]) * int(row["amount"])
    session["alert"] = None
    return render_template(
        "index.html",
        username=userinfo[0]["username"],
        funds=usd(userinfo[0]["cash"]),
        ownedstocks=ownedstocks,
        value=usd(value),
        alert=alert,
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        shares = request.form.get("shares")
        if not request.form.get("symbol"):
            return apology("must provide a valid symbol", 400)
        if not request.form.get("shares") or shares.isdigit() < 1:
            return apology("must provide valid amount", 400)
        if lookup(request.form.get("symbol")) == None:
            return apology("Stock does not exist", 400)
        stock = lookup(request.form.get("symbol"))
        totalprice = float(stock["price"]) * int(request.form.get("shares"))
        userid = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", userid)
        # checks affordability
        if float(totalprice) > cash[0]["cash"]:
            return apology("insufficient funds", 400)
        # check if its in stocks database
        test = db.execute("SELECT * FROM stock WHERE name = ?", stock["symbol"])
        # if not in stocks database, put inside database
        if len(test) == 0:
            db.execute("INSERT INTO stock (name) VALUES(?)", stock["symbol"])
            db.execute(
                "INSERT INTO stock_price (stock_id, price) VALUES((SELECT id FROM stock WHERE name = ?), ?)",
                stock["symbol"],
                stock["price"],
            )
        # else, update the live price that is in database
        else:
            db.execute(
                "UPDATE stock_price SET price = ? WHERE stock_id = (SELECT id FROM stock WHERE name = ?)",
                stock["price"],
                stock["symbol"],
            )
        # updates cash in users table
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?;",
            float(cash[0]["cash"]) - float(totalprice),
            userid,
        )
        # updates transaction table
        db.execute(
            "INSERT INTO transactions (user_id, stock_id, stock_price, amount, totalprice, time, type) VALUES(?, (SELECT id FROM stock WHERE name = ?), ?, ?, ?, datetime('now', 'localtime'), 'BUY')",
            userid,
            stock["symbol"],
            float(stock["price"]),
            int(request.form.get("shares")),
            float(totalprice),
        )
        # updates userstock table
        # check if user has bought the same stock before
        test2 = db.execute(
            "SELECT * FROM userstock WHERE user_id = ? and stock_id = (SELECT id FROM stock WHERE name = ?)",
            userid,
            stock["symbol"],
        )
        if len(test2) == 0:
            db.execute(
                "INSERT INTO userstock (user_id, stock_id, amount) VALUES(?,(SELECT id FROM stock WHERE name = ?),?)",
                userid,
                stock["symbol"],
                int(request.form.get("shares")),
            )
        # if yes, increase the amount
        elif len(test2) == 1:
            db.execute(
                "UPDATE userstock SET amount = amount + ? WHERE user_id = ? and stock_id = (SELECT id FROM stock WHERE name = ?)",
                int(request.form.get("shares")),
                userid,
                stock["symbol"],
            )
        # if not, create a new row and edit the amount
        else:
            return apology("ERROR duplicate in database", 400)
        session["alert"] = "Stock Bought!"
        # create a log of
        return redirect("/")
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    histories = db.execute(
        "SELECT * FROM transactions WHERE user_id = ? ORDER BY time DESC",
        session["user_id"],
    )
    username = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    # convert stock id to stock names
    for history in histories:
        history["stock_id"] = db.execute(
            "SELECT name from stock where id = ?", history["stock_id"]
        )[0]["name"]
        history["stock_price"] = usd(float(history["stock_price"]))
        history["totalprice"] = usd(float(history["totalprice"]))
    """Show history of transactions"""
    return render_template(
        "history.html", username=username[0]["username"], histories=histories
    )


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
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Missing", 400)
        # SELECT price, symbol FROM database WHERE symnbol =  (input)
        quote = lookup(request.form.get("symbol"))
        if quote == None:
            return apology("No stock found", 400)
        price = usd(quote["price"])
        # check if name of symbol is in database
        test = db.execute("SELECT * FROM stock WHERE name = ?", quote["symbol"])
        # if not in database, insert searched stock into database
        if len(test) == 0:
            db.execute("INSERT INTO stock (name) VALUES(?)", quote["symbol"])
            db.execute(
                "INSERT INTO stock_price (stock_id, price) VALUES((SELECT id FROM stock WHERE name = ?), ?)",
                quote["symbol"],
                quote["price"],
            )
            return render_template("quoted.html", quote=quote, price=price)
        # if yes, reupdate stock price
        db.execute(
            "UPDATE stock_price SET price = ? WHERE stock_id = (SELECT id FROM stock WHERE name = ?)",
            quote["price"],
            quote["symbol"],
        )

        return render_template("quoted.html", quote=quote, price=price)
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)
        # Ensure password was submitted and confirmation password matches
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 400)
        elif not (request.form.get("confirmation") == request.form.get("password")):
            return apology("password did not match", 400)
        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )
        if len(rows) > 0:
            return apology("Username has been taken", 400)
        # hashes the password to encrpyt it
        hash = generate_password_hash(request.form.get("password"))
        db.execute(
            "INSERT INTO users (username, hash) VALUES(?,?)",
            request.form.get("username"),
            hash,
        )
        id = db.execute("SELECT id FROM users WHERE hash = ?", hash)
        # Ensure username exists and password is correct
        session["user_id"] = id[0]["id"]
        session["alert"] = "Registered Successfully!"
        return redirect("/")
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    rows = db.execute(
        "SELECT amount, stock.name  FROM userstock JOIN stock on userstock.stock_id = stock.id where user_id = ?",
        session["user_id"],
    )
    stocks = []
    for row in rows:
        value = {"name": row["name"], "amount": row["amount"]}
        stocks.append(value)
    if request.method == "POST":
        # check if options are empty/invalid
        if not request.form.get("shares") or not request.form.get("symbol"):
            return apology("missing info", 400)
        for stock in stocks:
            if request.form.get("symbol") == stock["name"]:
                if int(request.form.get("shares")) > int(stock["amount"]):
                    return apology("missing stock", 400)
                # contiunation of match
                sellprice = float(lookup(request.form.get("symbol"))["price"]) * float(
                    request.form.get("shares")
                )
                db.execute(
                    "UPDATE users SET cash = cash + ? WHERE id = ?",
                    sellprice,
                    session["user_id"],
                )
                db.execute(
                    "UPDATE userstock SET amount = amount - ? WHERE user_id = ? and stock_id = (SELECT id FROM stock WHERE name = ?)",
                    int(request.form.get("shares")),
                    session["user_id"],
                    request.form.get("symbol"),
                )
                db.execute(
                    "INSERT INTO transactions (user_id, stock_id, stock_price, amount, totalprice, time, type) VALUES(?, (SELECT id FROM stock WHERE name = ?), ?, ?, ?, datetime('now', 'localtime'), 'SELL')",
                    session["user_id"],
                    request.form.get("symbol"),
                    (lookup(request.form.get("symbol"))["price"]),
                    request.form.get("shares"),
                    sellprice,
                )
                if (
                    db.execute(
                        "SELECT amount FROM userstock WHERE user_id = ? and stock_id = (SELECT id FROM stock WHERE name = ?)",
                        session["user_id"],
                        request.form.get("symbol"),
                    )[0]["amount"]
                    == 0
                ):
                    db.execute(
                        "DELETE FROM userstock WHERE user_id = ? and stock_id = (SELECT id FROM stock WHERE name = ?)",
                        session["user_id"],
                        request.form.get("symbol"),
                    )
                # minus off from table
                session["alert"] = "Stock Sold!"
                return redirect("/")
        # failure
        return apology("No such stocks", 400)
    """Sell shares of stock"""
    return render_template("sell.html", stocks=stocks)


@app.route("/delete", methods=["GET", "POST"])
@login_required
def delete():
    name = db.execute("SELECT username from users where id = ?", session["user_id"])
    if request.method == "POST":
        if "yes" in request.form:
            db.execute("DELETE FROM transactions WHERE user_id = ?", session["user_id"])
            db.execute("DELETE FROM userstock WHERE user_id = ?", session["user_id"])
            db.execute("DELETE FROM users WHERE id = ?", session["user_id"])
            session.clear()
            return render_template("delete.html")

        if "no" in request.form:
            return redirect("/")

        else:
            return apology("Invalid Input", 400)

    return render_template("delete.html", name=name[0]["username"])


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    currentcash = db.execute("SELECT cash FROM users where id = ?", session["user_id"])
    if request.method == "POST":
        if not request.form.get("number") or float(request.form.get("number")) < 0:
            return apology("invalid input", 400)
        cash = float(request.form.get("number"))
        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?",
            float(cash),
            session["user_id"],
        )
        return redirect("/")
    return render_template(
        "add.html", currentcash="{:,.2f}".format(currentcash[0]["cash"])
    )
