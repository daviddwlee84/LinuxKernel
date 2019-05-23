#include "../header/BCM2837_GPIO.h"
#include <stdio.h>

void testNumTable()
{
    printf("Number to 7-seg display table\n");
    printf("   a b c d e f g\n");
    for (int num = 0; num < 10; num++)
    {
        printf("%d: ", num);
        for (int dig = 0; dig < 7; dig++)
        {
            printf("%d ", NumTable[num][dig]);
        }
        printf("\n");
    }
}

int gpioBlinkTest()
{
    if (map_peripheral(&g_gpio) == -1)
    {
        printf("Failed to map the physical GPIO registers into the virtual memory space. \n");
        return -1;
    }

    const int testPin = 4; // Physical pin 7

    printf("Enable GPIO %d\n", testPin);
    INP_GPIO(testPin);
    printf("Set GPIO %d to output mode\n", testPin);
    OUT_GPIO(testPin);

    while (1)
    {
        printf("Led is on...\n");
        GPIO_SET = 1 << testPin;
        sleep(1);

        printf("Led is off...\n\n");
        GPIO_CLR = 1 << testPin;
        sleep(1);
    }
}

int main()
{
    testNumTable();
    return gpioBlinkTest();
}