
# Initialize validHeight
validHeight = False

# Looping input until correct
while (validHeight == False):
    # Asking for height
    print("Height: ", end="")
    height = input()

    # Checking for int
    try:
        int(height)
        validHeight = True
    except ValueError:
        validHeight = False

    if validHeight == True:
        # Checking if within bounds
        height = int(height)
        if height > 0 and height < 9:
            validHeight = True
        else:
            validHeight = False

# Looping build process
for i in range(height):
    for j in range(height):
        if (i + j) < (height - 1):
            print(" ", end="")
        else:
            print("#", end="")
    print("")
