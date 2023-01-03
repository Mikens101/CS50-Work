from cs50 import get_string

# Ask user for text
text = get_string("Text: ")

# Calculate length
letters = 0
words = 1
sent = 0
for i in text:
    if i.isalpha():
        letters += 1
    elif i == " ":
        words += 1
    elif i == "." or i == "!" or i == "?":
        sent += 1

# Coleman-Liau index
INDEX = round(0.0588 * (letters / words * 100) - 0.296 * (sent / words * 100) - 15.8)

# Print grade level
if INDEX < 1:
    print("Before Grade 1")
elif INDEX >= 1 and INDEX <= 16:
    print("Grade", INDEX)
elif INDEX > 16:
    print("Grade 16+")

