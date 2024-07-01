.schema - // for references
CREATE TABLE crime_scene_reports (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    street TEXT,
    description TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE interviews (
    id INTEGER,
    name TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    transcript TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE atm_transactions (
    id INTEGER,
    account_number INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    atm_location TEXT,
    transaction_type TEXT,
    amount INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE bank_accounts (
    account_number INTEGER,
    person_id INTEGER,
    creation_year INTEGER,
    FOREIGN KEY(person_id) REFERENCES people(id)
);
CREATE TABLE airports (
    id INTEGER,
    abbreviation TEXT,
    full_name TEXT,
    city TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE flights (
    id INTEGER,
    origin_airport_id INTEGER,
    destination_airport_id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    PRIMARY KEY(id),
    FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
    FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
);
CREATE TABLE passengers (
    flight_id INTEGER,
    passport_number INTEGER,
    seat TEXT,
    FOREIGN KEY(flight_id) REFERENCES flights(id)
);
CREATE TABLE phone_calls (
    id INTEGER,
    caller TEXT,
    receiver TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    duration INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE people (
    id INTEGER,
    name TEXT,
    phone_number TEXT,
    passport_number INTEGER,
    license_plate TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE bakery_security_logs (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    activity TEXT,
    license_plate TEXT,
    PRIMARY KEY(id)
);

SELECT * FROM crime_scene_reports WHERE street = 'Humphrey Street' AND month = 7 AND day = 28;
//case ID = 295, time: 10:15am ,
// where interviews have been conducted with 3 witnesses at that time


SELECT * from interviews where year = 2023 AND month = 7 and day =28;
// gets interview on that day
// interview ID 161, Ruth - "Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame"
// interview ID 162, Eugene -  "I don't know the thief's name, but it was someone I recognized. Earlier this morning,
before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money."
// interview ID 163 Raymond - "As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
The thief then asked the person on the other end of the phone to purchase the flight ticket."

SELECT license_plate FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28
    AND HOUR = 10 AND activity = 'exit'
    AND minute >= 15
    AND minute <=25 ORDER BY minute;
// Possible license plates within the timeframe

 SELECT account_number FROM atm_transactions
    WHERE atm_location = 'Leggett Street' AND year = 2023
    AND month = 7 AND day = 28 AND transaction_type = 'withdraw';
// Possible account_numbers within atm withdrawal at Leggett Street on that day

SELECT person_id from bank_accounts WHERE account_number IN
    (SELECT account_number FROM atm_transactions
     WHERE atm_location = 'Leggett Street'
     AND year = 2023 AND month = 7
     AND day = 28 AND transaction_type = 'withdraw');
// Gets all person ids that matches the possible account_number

SELECT id, name FROM people where license_plate IN
    (SELECT license_plate FROM bakery_security_logs
     WHERE year = 2023 AND month = 7 AND day = 28
     AND HOUR = 10 AND activity = 'exit' AND minute >= 15
     AND minute <=25)
AND id IN
    (SELECT person_id from bank_accounts WHERE account_number IN
        (SELECT account_number FROM atm_transactions
         WHERE atm_location = 'Leggett Street'
         AND year = 2023 AND month = 7  AND day = 28
         AND transaction_type = 'withdraw')
     );
// Gets all name and ID that matches BOTH of these conditions,
// Possible Subjects: Iman(396669), Luca(467400), Diana(514354), Bruce(686048)

SELECT caller FROM phone_calls
    WHERE year = 2023 AND month = 7
    AND day = 28 AND duration <= 60;
// Gets all the callers from phone calls of that day which is "also less than a minute"

SELECT * FROM people where license_plate IN
    (SELECT license_plate FROM bakery_security_logs
     WHERE year = 2023 AND month = 7 AND day = 28
     AND HOUR = 10 AND activity = 'exit' AND minute >= 15
     AND minute <=25)
AND id IN
    (SELECT person_id from bank_accounts WHERE account_number IN
        (SELECT account_number FROM atm_transactions
         WHERE atm_location = 'Leggett Street'
         AND year = 2023 AND month = 7  AND day = 28
         AND transaction_type = 'withdraw')
     )
AND phone_number IN
    (SELECT caller FROM phone_calls
    WHERE year = 2023 AND month = 7
    AND day = 28 AND duration <= 60);
// Gets all name and ID that matches ALL 3 of these conditions,
// Possible Subjects: Diana(514354), Bruce(686048)

SELECT id from airports where city = 'Fiftyville'
// gets airport id for fiftyville

SELECT * FROM flights
    WHERE year = 2023 AND MONTH = 7
    AND DAY = 29
    AND origin_airport_id IN
        (SELECT id from airports
        WHERE city = 'Fiftyville'
        )
    ORDER BY hour LIMIT 1;
// gets earliest possible flight from Fiftyville on 29th July 2023
// destination_airport_id = 4
// flight id = 36

SELECT city FROM airports WHERE id = 4;
// the flight went to New York City

SELECT passport_number FROM people WHERE id = 514354 OR id = 686048;
// passport numbers of the two possible suspects

SELECT passport_number FROM passengers
    WHERE flight_id = 36 AND passport_number IN
        (SELECT passport_number FROM people
         WHERE id = 514354 OR id = 686048
        );
// checks if any of the possible suspect is in this flight by comparing the passport numbers
// passport number = 5773159633 is in this flight, seat 4A

SELECT id,name FROM people
    WHERE passport_number IN
       (SELECT passport_number FROM passengers
            WHERE flight_id = 36 AND passport_number IN
                (SELECT passport_number FROM people
                    WHERE id = 514354 OR id = 686048
                )
        );
// gets the id and name of the thief
// id = 686048, name = Bruce

SELECT receiver FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28
    AND duration <= 60 AND caller IN
        (SELECT phone_number FROM people
            WHERE id = 686048
        );
// gets receiver phone number

SELECT id,name FROM people
    WHERE phone_number IN
        (SELECT receiver FROM phone_calls
            WHERE year = 2023 AND month = 7 AND day = 28
             AND duration <= 60 AND caller IN
                (SELECT phone_number FROM people
                    WHERE id = 686048
                 )
        );
// Accomplice name = Robin, id = 864400
