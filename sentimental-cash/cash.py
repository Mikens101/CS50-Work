from cs50 import get_float

valid = False

# User input cents owed
while (valid == False):
    print("Change owed: ", end="")
    cents = get_float("")

    if (cents >= 0):
        valid = True

cents = cents * 100
# Calculate number of quarters to give the customer
quarters = int(cents / 25)

# Calculate number of dimes to give the customer
dimes = int((cents % 25) / 10)

# Calculate number of nickels to give the customer
nickels = int(((cents % 25) % 10) / 5)

# Calculate number of pennies to give the customer
pennies = int((((cents % 25) % 10) % 5) / 1)

# Sum coins
coins = quarters + dimes + nickels + pennies

# Print total number of coins to give the customer
print(coins)
