#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Test KEY input (only 1!)
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Test KEY array length
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Test isalpha(KEY)
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
    }

    // Test unique characters
    string key = argv[1];
    for (int i = 0; i < strlen(key); i++)
    {
        for (int j = i + 1; j < strlen(key); j++)
        {
            if (toupper(key[i]) == toupper(key[j]))
            {
                printf("Usage: ./substitution key\n");
                return 1;
            }
        }
    }

    // Ask user for plain text
    string ptext = get_string("plaintext:  ");
    printf("ciphertext: ");

    // Turn upper to lower
    for (int i = 0; i < strlen(key); i++)
    {
        if (islower(key[i]))
        {
            key[i] = key[i] - 32;
        }
    }

    for (int i = 0; i < strlen(key); i++)
    {
        printf("%c",key[i]);
    }
    printf("\n");

    char ciphertext[strlen(ptext) + 1];
    ciphertext[strlen(ptext)] = 0;

    // Cipher...
    for (int i = 0; i < strlen(ptext); i++)
    {
        if (isupper(ptext[i]))
        {
            int letter = ptext[i] - 65;
            ciphertext[i] = key[letter];
            printf("%c", ciphertext[i]);
        }
        else if (islower(ptext[i]))
        {
            int letter = ptext[i] - 97;
            ciphertext[i] = key[letter] + 32;
            printf("%c", ciphertext[i]);
        }
        else
        {
            ciphertext[i] = ptext[i];
            printf("%c", ciphertext[i]);

        }
    }

    printf("\n");
    printf("%s, ", ptext);
    printf("%s\n", ciphertext);

}