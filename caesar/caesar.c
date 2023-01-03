#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int CAP[] = {65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90};
int LOW[] = {97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122};

string rotate(string text, int k);

int main(int argc, string argv[])
{
    //    1. Don't accpet key if... (more than 1 input)
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    //     ...(not an integer)
    string key = argv[1];
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    //    3. get_string("plaintext: ");
    int k = atoi(key);
    string ptext = get_string("plaintext: ");

    //    4. For every char, shift by key
    //    5. printf("ciphertext: %s\n", ctext);
    printf("ciphertext: ");

    for (int i = 0; i < strlen(ptext); i++)
    {
        if (isupper(ptext[i]))
        {
            printf("%c", (((ptext[i] - 'A') + k) % 26) + 'A');
        }
        else if (islower(ptext[i]))
        {
            printf("%c", (((ptext[i] - 'a') + k) % 26) + 'a');
        }
        else
        {
            printf("%c", ptext[i]);
        }
    }

    printf("\n");
}
