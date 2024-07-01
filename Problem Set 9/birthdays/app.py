import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")




@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response




@app.route("/", methods=["GET", "POST",])
def index():
    if request.method == "POST":
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        month29 =[2]
        month30 =[4,6,9,11]
        month31 =[1,3,5,7,8,10,12]

        if not name:
            database = db.execute("SELECT * FROM birthdays")
            message = "Missing Name"
            return render_template("index.html",database = database, message = message)
        if not month:
            database = db.execute("SELECT * FROM birthdays")
            message = "Missing Month"
            return render_template("index.html",database = database, message = message)
        elif int(month) < 0 or int(month) > 12:
            database = db.execute("SELECT * FROM birthdays")
            message = "Invalid Month"
            return render_template("index.html",database = database, message = message)
        if not day:
            database = db.execute("SELECT * FROM birthdays")
            message = "Missing Day"
            return render_template("index.html",database = database, message = message)
        elif int(day) < 0 or (int(month) in month31 and int(day) > 31) or (int(month) in month30 and int(day) > 30) or (int(month) in month29 and int(day) > 29) :
            database = db.execute("SELECT * FROM birthdays")
            message = "Invalid Day"
            return render_template("index.html",database = database, message = message)

        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?,?,?)", name, month, day)

        return redirect("/")

    else:
        database = db.execute("SELECT * FROM birthdays")
        message = ""
        return render_template("index.html",database = database, message = message)



@app.route("/remove", methods=["POST"])
def remove():
    if request.method == "POST":
        id = request.form.get('id')
        db.execute("DELETE FROM birthdays WHERE id = ?", id)
        return redirect("/")


