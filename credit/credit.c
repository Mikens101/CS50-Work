#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    int ncount = 0;
    long number;

    // Card # input
    do
    {
        number = get_long("Number: ");
    } while (number < 0);

    long keepnum = number;

    // Count # of digits
    while (number > 0)
    {
        number = number / 10;
        ncount++;
    }
    printf("%i\n", ncount);

    // Checking card type... store in var for later
    string type;

    long snum = keepnum / pow(10, ncount - 2);
    printf("%li\n", snum);
    if(ncount == 15 && (snum == 34 || snum == 37))
    {
        type = "AMEX";
    }
    else if(ncount == 16 && (snum >=51 && snum <= 55))
    }
        type = "MASTERCARD";
    }
    else if (((ncount == 13) || (ncount == 16)) && (snum / 10) == 4)
    {
        type = "VISA";
    }
    else
    {
        type = "INVALID";
    }

    // Luhn's validity check
    int lsum = 0;

    while (keepnum != 0)
    {
        int d1 = keepnum % 10;                          // Digit 1 = xx.rxxx (where r is # extracted)
        lsum += d1;                                     // Add d1 to Luhn's sum
        int d2 = 2 * ((keepnum / 10) % 10);             // Digit 2 = ??
        int r1 = (d2 % 10) + floor((d2 / 10) % 10);     // 
        lsum += r1;                                     //
        keepnum /= 100;                                 // Move CC# over 2 places, then repeat
    }

    // Print Answer
    if(lsum % 10 == 0)
        printf("%s\n", type);
    else
        printf("INVALID\n");
}

