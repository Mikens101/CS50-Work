

# Check validity of input
validHeight = False
while (validHeight == False):

    # Ask for input
    height = input("Height: ")
    if height.isnumeric():
        if int(height) > 0 and int(height) < 9:
            validHeight = True
            height = int(height)
        else:
            validHeight = False

# Build height
for i in range(height):

    # Build length L
    for j in range(height):
        if (i + j) < (height - 1):
            print(" ", end="")
        else:
            print("#", end="")
        j += 1
    print("  ", end="")

    # Build lenght R
    for j in range(height):
        if (j - i) <= 0:
            print("#", end="")
        j += 1
    print("")
    i += 1

