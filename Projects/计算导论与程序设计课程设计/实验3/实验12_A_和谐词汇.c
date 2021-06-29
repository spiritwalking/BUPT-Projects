#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char toReplace[12][12] = {0}, replace[9] = "!@#$%^&*";
    char article[120];
    int i = 0, j = 0;
    char tmp[1000] = {0};
    int pos, len, all, flag, pos2, flag2;
    FILE *input = fopen("dict.dic", "r");
    while (fgets(toReplace[i], 11, input))
    {
        if (toReplace[i][strlen(toReplace[i]) - 1] == '\n')
        {
            toReplace[i][strlen(toReplace[i]) - 1] = '\0';
        }
        i++;
    }
    all = i;
    fclose(input);

    while (gets(article) != NULL)
    {
        pos2 = 0;
        len = strlen(article);
        for (pos = 0; pos < len; pos++)
        {
            flag = 0;
            for (i = 0; i < all; i++)
            {
                //判断是否相同
                flag2 = 0;
                for (j = 0; j < strlen(toReplace[i]); j++)
                {
                    if (article[pos + j] != toReplace[i][j])
                    {
                        flag2 = 1;
                        break;
                    }
                }
                if (flag2 == 0)
                {
                    flag = 1;
                    for (j = 0; j < 8; j++)
                    {
                        tmp[pos2] = replace[j];
                        pos2++;
                    }
                    pos += strlen(toReplace[i]) - 1;
                    break;
                }
            }
            if (flag == 0)
            {
                tmp[pos2] = article[pos];
                pos2++;
            }
        }
        printf("%s\n", tmp);
        for (j = 0; j < 1000; j++)
        {
            tmp[j] = 0;
        }
    }
    return 0;
}