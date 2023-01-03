#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sent(string text);

int main(void)
{
    string text = get_string("Text: ");

    int length = count_letters(text);
    int words = count_words(text);
    int sent = count_sent(text);

    // printf("%i letters\n", length);
    // printf("%i words\n", words);
    // printf("%i sentences\n", sent);

    int INDEX = round(0.0588 * ((float) length / words * 100) - 0.296 * ((float) sent / words * 100) - 15.8);

    if (INDEX < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (INDEX >= 1 && INDEX <= 16)
    {
        printf("Grade %i\n", INDEX);
    }
    else if (INDEX > 16)
    {
        printf("Grade 16+\n");
    }
}

int count_letters(string text)
{
    int length = 0;
    // Count # of char
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            length ++;
        }
    }
    return length;
}

int count_words(string text)
{
    int words = 0;
    // Count # of words
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isspace(text[i]))
        {
            words++;
        }
    }
    return words + 1;
}

int count_sent(string text)
{
    int sent = 0;
    // Count # of sentences
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == 46 || text[i] == 33 || text[i] == 63)
        {
            sent++;
        }
    }
    return sent;
}