import os
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime, date
from helpers import apology, login_required, usd, time, formatbooking

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd
treatments =[]
#today's date
tdy = date.today()
# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///data.db")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/tags", methods=["GET","POST"])
@login_required
def tags():
    # updated tag
    tags = db.execute("SELECT * FROM usertags WHERE users_id = ?", session["user_id"])
    if request.method == "POST":
        # if empty, return an error
        if not request.form.get("tag_name"):
            error = "Tag needs a name!"
            return render_template("tags.html", error = error, tags = tags)

        current_tag = db.execute("SELECT * FROM usertags WHERE users_id = ? and tag_name = ?",
                                 session["user_id"], request.form.get("tag_name"))
        # if not duplicate
        if len(current_tag) == 0:
           # insert tag into user_tag
           db.execute("INSERT into usertags (users_id, tag_name) VALUES(?,?)",
                      session["user_id"], request.form.get("tag_name"))
           tags = db.execute("SELECT * FROM usertags WHERE users_id = ?", session["user_id"])
           return render_template("tags.html", tags=tags)
        # if duplicate
        error = "Tag already exist!"
        return render_template("tags.html", error = error, tags = tags)

    return render_template("tags.html", tags=tags)

@app.route("/")
@login_required
def index():
    # check today's date
    today = datetime.today()
    # list any bookings today
    today_bookings = db.execute("SELECT * FROM bookings WHERE users_id =? AND date =?", session["user_id"], today.strftime("%Y-%m-%d"))
    if len(today_bookings) == 0:
        today_bookings = None
    else:
        for bookings in today_bookings:
            possibletags = db.execute("SELECT bookings_tags.usertags_id FROM bookings JOIN bookings_tags on bookings.id = bookings_tags.bookings_id WHERE bookings.id = ?",
                            bookings["id"])
            # if there are user tags in the bookings,
            if len(possibletags) > 0:
                for tags in possibletags:
                    tag_name = db.execute("SELECT tag_name FROM usertags WHERE id = ?", tags["usertags_id"])
                    if bookings["type"]:
                        bookings["type"] += " , " + tag_name[0]["tag_name"]
                    else:
                        bookings["type"] = tag_name[0]["tag_name"]
            formatbooking(bookings)
    # list upcoming bookings
    upcoming_bookings = db.execute("SELECT * FROM bookings WHERE users_id =? AND date >?", session["user_id"], today.strftime("%Y-%m-%d"))
    if len(upcoming_bookings) == 0:
        upcoming_bookings = None
    else:
        for bookings in upcoming_bookings:
            possibletags = db.execute("SELECT bookings_tags.usertags_id FROM bookings JOIN bookings_tags on bookings.id = bookings_tags.bookings_id WHERE bookings.id = ?",
                            bookings["id"])
            # if there are user tags in the bookings,
            if len(possibletags) > 0:
                for tags in possibletags:
                    tag_name = db.execute("SELECT tag_name FROM usertags WHERE id = ?", tags["usertags_id"])
                    if bookings["type"]:
                        bookings["type"] += " , " + tag_name[0]["tag_name"]
                    else:
                        bookings["type"] = tag_name[0]["tag_name"]
            formatbooking(bookings)
    # list previous bookings
    previous_bookings = db.execute("SELECT * FROM bookings WHERE users_id =? AND date <?", session["user_id"], today.strftime("%Y-%m-%d"))
    if len(previous_bookings) == 0:
        previous_bookings = None
    else:
        for bookings in previous_bookings:
            possibletags = db.execute("SELECT bookings_tags.usertags_id FROM bookings JOIN bookings_tags on bookings.id = bookings_tags.bookings_id WHERE bookings.id = ?",
                            bookings["id"])
            # if there are user tags in the bookings,
            if len(possibletags) > 0:
                for tags in possibletags:
                    tag_name = db.execute("SELECT tag_name FROM usertags WHERE id = ?", tags["usertags_id"])
                    if bookings["type"]:
                        bookings["type"] += " , " + tag_name[0]["tag_name"]
                    else:
                        bookings["type"] = tag_name[0]["tag_name"]
            formatbooking(bookings)

    username = db.execute("SELECT username FROM users WHERE id =?", session["user_id"])

    """Show bookings"""
    if "feedback" in session:
        feedback = session["feedback"]
    else:
        feedback = None
    options = []
    for hour in range(24):
        for minute in range(4):
            h = hour
            m = minute * 15
            options.append("{:02d}:{:02d}".format(h, m))
    userbookings = db.execute("SELECT * FROM bookings where users_id = ? ORDER BY date", session["user_id"])
    if len(userbookings) == 0:
        userbookings = None
    else:
        for bookings in userbookings:
            possibletags = db.execute("SELECT bookings_tags.usertags_id FROM bookings JOIN bookings_tags on bookings.id = bookings_tags.bookings_id WHERE bookings.id = ?",
                            bookings["id"])
            if len(possibletags) > 0:
                for tags in possibletags:
                    tag_name = db.execute("SELECT tag_name FROM usertags WHERE id = ?", tags["usertags_id"])
                    if bookings["type"]:
                        bookings["type"] += " , " + tag_name[0]["tag_name"]
                    else:
                        bookings["type"] = tag_name[0]["tag_name"]

            formatbooking(bookings)


    session["feedback"] = None

    tags = db.execute("SELECT * FROM usertags WHERE users_id = ?", session["user_id"])
    return render_template("index.html", userbookings = userbookings, options = options, feedback = feedback, today_bookings = today_bookings,
                           upcoming_bookings = upcoming_bookings, previous_bookings = previous_bookings,
                           username = username[0]["username"], tags=tags)


@app.route("/check", methods=["GET", "POST"])
@login_required
def check():
    """check"""
    clash = []
    error = None
    success = None
    options = []
    for hour in range(24):
        for minute in range(4):
            h = hour
            m = minute * 15
            options.append("{:02d}:{:02d}".format(h, m))
    if request.method == "POST":
        if not request.form.get("date") or not request.form.get("start_time") or not request.form.get("end_time"):
            error = "form not filled"
            return render_template("check.html", error = error, options = options)
        if time(request.form.get("start_time")) > time(request.form.get("end_time")):
            error = "not a valid time range"
            return render_template("check.html", error = error, options = options)
        # Select list of bookings on the date
        starttime = time(request.form.get("start_time"))
        endtime = time(request.form.get("end_time"))
        rows = db.execute("SELECT * FROM bookings WHERE date = ? AND users_id = ?", request.form.get("date"), session["user_id"])
        for row in rows:
            if starttime >= row["start_time"] and starttime <= row["end_time"]:
                error = "Time Slot has been taken"
                clash.append(row)
            elif endtime >= row["start_time"] and endtime <= row["end_time"]:
                error = "Time Slot has been taken"
                clash.append(row)
            elif starttime < row["start_time"] and endtime >= row["end_time"]:
                error = "Time Slot has been taken"
                clash.append(row)
        if error == None:
            return render_template("check.html", success="Slot is avaliable", options=options)

        return render_template("check.html", error=error, options=options, clash=clash)




    return render_template("check.html", options = options)


@app.route("/delete", methods=["POST"])
@login_required
def delete():
    """Delete Bookings"""

    def delete_bookings(bookingsarray):
        for bookings in bookingsarray:
            db.execute("DELETE FROM bookings_tags WHERE bookings_id = ?", bookings["id"])
            db.execute("DELETE FROM bookings WHERE  id = ? AND users_id=?", bookings["id"], session["user_id"])
        session["feedback"] = "Deleted ALL Bookings"
        return redirect("/")

    today = datetime.today()

    if request.form.get("delete_all"):
        print("Deleted ALL Bookings")
        all_bookings = db.execute("SELECT * FROM bookings WHERE users_id =?", session["user_id"])
        delete_bookings(all_bookings)

    if request.form.get("today_delete"):
        today_bookings = db.execute("SELECT * FROM bookings WHERE users_id =? AND date =?", session["user_id"], today.strftime("%Y-%m-%d"))
        delete_bookings(today_bookings)

    if request.form.get("upcoming_delete"):
        upcoming_bookings = db.execute("SELECT * FROM bookings WHERE users_id =? AND date >?", session["user_id"], today.strftime("%Y-%m-%d"))
        delete_bookings(upcoming_bookings)

    if request.form.get("previous_delete"):
        previous_bookings = db.execute("SELECT * FROM bookings WHERE users_id =? AND date <?", session["user_id"], today.strftime("%Y-%m-%d"))
        delete_bookings(previous_bookings)

    if request.form.get("delete_id"):
        id = request.form.get("delete_id") # booking id
    # check for tags in this ID
        tags = db.execute("SELECT * FROM bookings_tags WHERE bookings_id = ?", id)
        if tags:
            db.execute("DELETE from bookings_tags WHERE bookings_id = ?", id)

        db.execute("DELETE from bookings WHERE id = ? AND users_id = ?", id, session["user_id"])
        session["feedback"] = "Deleted Booking"
        return redirect("/")
    return redirect("/")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()
    feedback = None
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username and password was submitted
        if not request.form.get("username") or not request.form.get("password"):
            feedback = "Please enter your username and password"
            return render_template("login.html", feedback=feedback)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            feedback = "Incorrect username or password"
            return render_template("login.html", feedback=feedback)

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


@app.route("/book", methods=["GET", "POST"])
@login_required
def book():
    """Book appointment"""
    options = []
    # defining all options for start/end time
    for hour in range(24):
        for minute in range(4):
            h = hour
            m = minute * 15
            options.append("{:02d}:{:02d}".format(h, m))
    tags = db.execute("SELECT * from usertags WHERE users_id = ?", session["user_id"])
    if request.method == "POST":
        if not request.form.get("date") or not request.form.get("start_time") or not request.form.get("end_time"):
            error = "form not filled"
            return render_template("book.html", error = error, options = options, tags = tags)
        if time(request.form.get("end_time")) <= time(request.form.get("start_time")):
            error = "invalid time range"
            return render_template("book.html", error = error, options = options, tags = tags)
        # if details / contact if left blank
        try:
            details = request.form.get("details")
            contact = int(request.form.get("contact"))
        except ValueError:
            db.execute("INSERT INTO bookings (users_id, date, start_time, end_time, type) VALUES (?, ?, ?, ?, ?)",
                             session["user_id"], request.form.get("date"), time(request.form.get("start_time")),
                            time(request.form.get("end_time")), details)
        else:
            db.execute("INSERT INTO bookings (users_id, date, start_time, end_time, contact, type) VALUES (?, ?, ?, ?, ?, ?)",
                            session["user_id"], request.form.get("date"), time(request.form.get("start_time")),
                            time(request.form.get("end_time")), contact, details)

        booking_id = db.execute("SELECT last_insert_rowid() as id")[0]["id"]


        selected_tag = []
        for key in request.form:
            if key.startswith('tag'):
                tag_name =  key.replace('tag', '')
                selected_tag.append(tag_name)

        if selected_tag:
            for tag in selected_tag:
                db.execute("INSERT INTO bookings_tags (bookings_id, usertags_id) VALUES (?,?)", booking_id, tag)
            # add the tag into the booking
        return redirect("/")


    return render_template("book.html", options = options, tags = tags)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # check for missing inputs
    msg = None
    if request.method == "POST":
        if not request.form.get("username") or not request.form.get("password") or not request.form.get("cfmpassword"):
            msg = "Details was not fully filled"
            return render_template("register.html", errormessage1 = msg)
        # reject duplicate names
        usernamelist = db.execute("SELECT username FROM users where username = ?", request.form.get("username"))
        if len(usernamelist) > 0:
            msg = "Username has been taken"
            return render_template("register.html", errormessage1 = msg)
        # reject password that is less than 8 characters long
        password = request.form.get("password")
        if len(password) < 8:
            msg ="Password does not meet requirements"
            return render_template("register.html", errormessage1 = msg)
        # reject if password and cfm password is different
        if request.form.get("password") != request.form.get("cfmpassword"):
            msg ="Password does not match"
            return render_template("register.html", errormessage1 = msg)
        hash = generate_password_hash(request.form.get("password"))
        db.execute("INSERT into users (username, hash) VALUES(?,?)", request.form.get("username"), hash)
        return render_template("login.html")

    return render_template("register.html")


@app.route("/edit", methods=["GET", "POST"])
@login_required
def edit():
    """edit bookings"""
    feedback = None
    if request.method == 'POST':
        # check valid inputs
        if not request.form.get('date') or not request.form.get('start_time') or not request.form.get('end_time'):
            session["feedback"] = "Invalid input"
            return redirect("/")
        # check valid time ranges
        start_time = time(request.form.get('start_time'))
        end_time = time(request.form.get('end_time'))
        if start_time >= end_time:
            session["feedback"] = "Invalid time range"
            return redirect("/")
        # if all okay, update bookings
        #  TODO check for tags and update into data
        selected_tag = []
        selected_tag_id = []
        for key in request.form:
            if key.startswith('tag'):
                tag_name =  key.replace('tag', '')
                selected_tag.append(tag_name)
        for i in selected_tag:
            tag_id = db.execute("SELECT id FROM usertags WHERE tag_name = ?", i)
            if tag_id:
                selected_tag_id.append(tag_id[0]["id"])
        bookingtags = db.execute("SELECT * FROM bookings_tags WHERE bookings_id = ?", request.form.get("booking_id"))
        if bookingtags:
            for bookingtag in bookingtags:
                db.execute("DELETE FROM bookings_tags WHERE id =?", bookingtag["id"])
        if selected_tag_id:
            for tag in selected_tag_id:
                db.execute("INSERT INTO bookings_tags (bookings_id, usertags_id) VALUES (?,?)", request.form.get("booking_id"), tag)
        try:
            details = request.form.get("details")
            contact = int(request.form.get("contact"))
        except ValueError:
            db.execute("UPDATE bookings SET date = ?, start_time = ?, end_time = ?, type = ? WHERE id = ?",
                        request.form.get("date"), start_time, end_time, details, request.form.get("booking_id"))
            session["feedback"] = "Booking updated!"
            return redirect("/")
        else:
            db.execute("UPDATE bookings SET date = ?, start_time = ?, end_time = ?, contact = ?, type = ? WHERE id  = ?",
                       request.form.get("date"), start_time, end_time, contact, details, request.form.get("booking_id") )
            session["feedback"] = "Booking updated!"
        return redirect("/")
    return redirect("/")


@app.route("/deletetags", methods=["POST"])
@login_required
def deletetags():
    if not request.form.get("tag_name"):
        return redirect("/tags")
    usertagid = db.execute("SELECT id FROM usertags WHERE tag_name = ? AND users_id = ?",
                 request.form.get("tag_name"), session["user_id"])
    bookingtags = db.execute("SELECT * FROM bookings_tags WHERE usertags_id = ?", usertagid[0]["id"])
    if len(bookingtags) > 0:
        for bookingtag in bookingtags:
            db.execute("DELETE FROM bookings_tags WHERE id =?", bookingtag["id"])
    db.execute("DELETE FROM usertags WHERE id = ?", usertagid[0]["id"])
    return redirect("/tags")


@app.route("/removeaccount", methods=["GET", "POST"])
@login_required
def removeaccount():
    name = db.execute("SELECT username from users where id = ?", session["user_id"])
    if request.method == "POST":
        if "yes" in request.form:
            #TODO delete all info of this user
            #Delete from booking tags(if any exist)
            usertags = db.execute("SELECT id FROM usertags WHERE users_id = ?", session["user_id"])
            if usertags:
                for usertag in usertags:
                    db.execute("DELETE FROM bookings_tags WHERE usertags_id =?", usertag["id"])
            # Delete from user tags(if any exist)
            db.execute("DELETE FROM usertags WHERE users_id =?", session["user_id"])
            # Delete from bookings
            db.execute("DELETE FROM bookings WHERE users_id =?", session["user_id"])
            # delete the user
            db.execute("DELETE FROM users WHERE id =?", session["user_id"])
            session.clear()
            return render_template("removeaccount.html")

        if "no" in request.form:
            return redirect("/")

        else:
            return apology("Invalid Input", 404)

    return render_template("removeaccount.html", name=name[0]["username"])

@app.route("/changepassword", methods=["GET", "POST"])
@login_required
def changepassword():
    """changes user password"""
    feedback = None
    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]["username"]
    if request.method == "POST":
        if not request.form.get("old_password") or not request.form.get("new_password") or not request.form.get("confirm_password"):
            feedback = "form is not filled correctly"
            return render_template("changepassword.html", feedback=feedback, username = username)
        old_hash = db.execute("SELECT hash FROM users WHERE id =?", session["user_id"])[0]["hash"]
        if not check_password_hash(old_hash, request.form.get("old_password")):
            feedback = "Incorrect Old Password"
            return render_template("changepassword.html", feedback=feedback, username = username)
        if request.form.get("new_password")!= request.form.get("confirm_password"):
            feedback = "New passwords do not match"
            return render_template("changepassword.html", feedback=feedback, username = username)
        if len(request.form.get("new_password")) < 8:
            feedback = "Password must be at least 8 characters long"
            return render_template("changepassword.html", feedback=feedback, username = username)
        new_hash = generate_password_hash(request.form.get("new_password"))
        db.execute("UPDATE users SET hash = ? WHERE id = ?", new_hash, session["user_id"])
        session["feedback"] = "You have changed your password successfully!"
        return redirect("/")
    return render_template("changepassword.html", username = username)
