-- Learn about the crime
SELECT id, description FROM crime_scene_reports
WHERE year = 2021
    AND month = 7
    AND day = 28
    AND street = "Humphrey Street"
    AND description LIKE "%CS50 duck%";

-- (295)    Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
--          Interviews were conducted today with three witnesses who were present at the
--          time – each of their interview transcripts mentions the bakery.

-- Learn about what the interviewee's know
SELECT id, name, transcript FROM interviews
WHERE year = 2021
    AND month = 7
    AND day = 28
    AND transcript LIKE "%bakery%";

-- (161 :: Ruth)    Sometime within ten minutes of the theft, I saw the thief
--                  get into a car in the bakery parking lot and drive away.
--                  If you have security footage from the bakery parking lot, you might
--                  want to look for cars that left the parking lot in that time frame.
-- (162 :: Eugene)  I don't know the thief's name, but it was someone I recognized.
--                  Earlier this morning, before I arrived at Emma's bakery, I was walking
--                  by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- (163 :: Raymond) As the thief was leaving the bakery, they called someone who talked to them
--                  for less than a minute. In the call, I heard the thief say that they were
--                  planning to take the earliest flight out of Fiftyville tomorrow. The thief then
--                  asked the person on the other end of the phone to purchase the flight ticket.

-- "Who's the Thief" Master SQL (copy+paste to keep building off of)
SELECT atm_transactions.account_number, atm_transactions.amount,
    bank_accounts.person_id, people.name, people.phone_number,
    people.passport_number, people.license_plate, airports.city, passengers.seat FROM bank_accounts
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
JOIN people ON bank_accounts.person_id = people.id
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.destination_airport_id = airports.id
WHERE

    -- ATM Information
        atm_location = "Leggett Street"
    AND transaction_type = "withdraw"
    AND atm_transactions.year = 2021
    AND atm_transactions.month = 7
    AND atm_transactions.day = 28

    -- Security Log Information
    AND license_plate IN (
        SELECT license_plate FROM bakery_security_logs
        WHERE activity = "exit"
            AND year = 2021
            AND month = 7
            AND day = 28
            AND hour = 10
            AND minute < (15+10)
    )

    -- Caller Information
    AND phone_number IN (
        SELECT caller FROM phone_calls
        WHERE duration < 60
            AND year = 2021
            AND month = 7
            AND day = 28
    )

    -- Flight Information
    AND passengers.passport_number IN (
        SELECT passport_number FROM passengers
        WHERE flight_id IN (
            SELECT id FROM flights
            WHERE year = 2021
                AND month = 7
                AND day = (28 + 1)
                AND origin_airport_id IN (
                    SELECT id FROM airports
                    WHERE city = "Fiftyville"
                )
            ORDER BY hour, minute
            LIMIT 1
        )
    );

--  | account_number | amount | person_id | name  |  phone_number  | passport_number | license_plate |     city      | seat |
--  +----------------+--------+-----------+-------+----------------+-----------------+---------------+---------------+------+
--  | 49610011       | 50     | 686048    | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | New York City | 4A   |

-- "Who's the accomplice" Master SQL (copy+paste to keep building off of)
SELECT name FROM people
WHERE phone_number IN (
    SELECT receiver FROM phone_calls
    WHERE caller = "(367) 555-5533"
        AND duration < 60
        AND year = 2021
        AND month = 7
        AND day = 28
);