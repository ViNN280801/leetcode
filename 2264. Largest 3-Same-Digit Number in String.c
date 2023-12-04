#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *largestGoodInteger(char *num)
{
    size_t len = strlen(num);
    if (!num || len == 0ul)
        return "";

    char *res = (char *)calloc(4ul, sizeof(char));
    for (char *p = num; p[2]; ++p)
        if (p[0] == p[1] && p[0] == p[2])
            res[0] = (res[0] < p[0]) ? p[0] : res[0];
    res[2] = res[1] = res[0];
    return res;
}

int main()
{
    char *res = largestGoodInteger("6777133339");
    if (!res)
        return 0;
    printf("%s\n", res);
    res = largestGoodInteger("2300019");
    printf("%s\n", res);
    free(res);
    printf("%s\n", largestGoodInteger("42352338"));

    return 0;
}
