-- keep a log of any SQL queries you execute as you solve the mystery.

-- find description of the crime

SELECT description
  FROM crime_scene_reports
 WHERE month = 7
       AND day = 28
       AND year = 2021
       AND street LIKE 'humphrey street';

-- look up interviews schema to find out what info I can gather from that table

-- get the names and transcripts from the witnesses who were interviewed on the day of the crime

SELECT name, transcript
  FROM interviews
 WHERE month = 7
       AND day = 28
       AND year = 2021;

-- isolate the 3 witnesses from the table based on the mention of the bakery in their testimony

-- the witnesses are Ruth, Eugene, and Raymond

-- Ruth says security cam footage might be useful. Eugene has ATM info I can check, and Raymond knows
-- which flight the criminal will take

-- check bakery security logs schema

-- get the activity and license plate of the car the criminal used to leave the bakery between 10:15 and 10:25am

SELECT activity, license_plate
  FROM bakery_security_logs
 WHERE month = 7
       AND day = 28
       AND year = 2021
       AND hour = 10
       AND minute BETWEEN 15 AND 25;

-- this returned 8 license plates. '.schema people' tells me I can look up the plate owners

-- select the names of people whose plates are in the table I got as result from the previous query

SELECT name
  FROM people
 WHERE license_plate IN
       (SELECT license_plate
          FROM bakery_security_logs
         WHERE month = 7
               AND day = 28
               AND year = 2021
               AND hour = 10
               AND minute BETWEEN 15 AND 25);

-- I now have the 8 people who left the bakery within the timeframe specified in Ruth's statement

-- They are: Vanessa, Barry, Iman, Sofia, Luca, Diana, Kelsey, and Bruce

-- follow up on Eugene's statement by using '.schema atm_transactions'

-- get the names of everyone who withdrew money from leggett st atm on the day of the crime based on their account number

-- I'll have to join bank_accounts to people and crosscheck with account numbers the atm transactions return

SELECT name
  FROM people
       JOIN bank_accounts
       ON people.id = bank_accounts.person_id
 WHERE account_number IN
       (SELECT account_number
          FROM atm_transactions
         WHERE month = 7
               AND day = 28
               AND year = 2021
               AND transaction_type = 'withdraw'
               AND atm_location = 'Leggett Street');

-- 8 people withdrew money from that atm on that day

-- they are: Bruce, Diana, Brooke, Kenny, Iman, Luca, Taylor, and Benista

-- My suspects have been narrowed to: Bruce, Diana, Iman, and Luca, since they show up on both suspect lists

-- Next I'll search the names of callers and receivers who made phone calls for less than a minute on the day of the crime

SELECT name, caller, receiver
  FROM people
       JOIN (SELECT caller, receiver
               FROM phone_calls
              WHERE month = 7
                AND day = 28
                AND year = 2021
                AND duration < 60)
       ON people.phone_number = caller;

-- Sofia, Kelsey, Bruce, Taylor, Diana, Carina, Kenny, and Benista all made calls under a minute long.

-- Bruce and Diana are my main suspects now. I've written down their phone numbers and will get the names of the people who received their calls

SELECT name, caller, receiver
  FROM people
       JOIN (SELECT caller, receiver
               FROM phone_calls
              WHERE month = 7
                AND day = 28
                AND year = 2021
                AND duration < 60)
       ON people.phone_number = receiver
 WHERE caller IN
       ('(367) 555-5533', '(770) 555-1861');

-- Bruce called Robin. Diana called Philip.

-- Check for the earliest flights out of fiftyville on the 29th

SELECT *
  FROM flights
 WHERE origin_airport_id =
       (SELECT id
          FROM airports
         WHERE city LIKE 'fiftyville')
       AND month = 7
       AND day = 29
       AND year = 2021
 ORDER BY hour, minute;

-- the earliest flight out of fiftyville leaves at 8:20am, has an id of 36, and the destination's id is 4. I'll find the name of the destination, and then check that flight id's passenger list for our suspects.

SELECT city
  FROM airports
 WHERE id = 4;

-- New York City is the destination our suspects fled to. Now I'll check the passenger list.

SELECT name
  FROM people
 WHERE passport_number IN
       (SELECT passport_number
          FROM passengers
         WHERE flight_id = 36);

-- Bruce is on the flight. He must be the thief, with Robin's assistance.