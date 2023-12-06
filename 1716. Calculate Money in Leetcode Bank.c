#include <stdio.h>

int totalMoney(int n)
{
    int money = 0, increase = 1, tmp = 1;
    for (int i = 1; i <= n; ++i)
    {
        money += increase++;
        if (i % 7 == 0)
            increase = ++tmp;
    }
    return money;
}

int main()
{   
    printf("%d\n", totalMoney(4));
    printf("%d\n", totalMoney(6));
    printf("%d\n", totalMoney(10));
    printf("%d\n", totalMoney(20));

    return 0;
}

