#include <stdio.h>

void filter(FILE *input, int begin, int end);

int main()
{
    FILE *input = fopen("dict.dic", "r");
    int cur = 6, lineNum[5];
    int i, n;
    scanf("%d", &n);
    for (i = 0; i < 5; i++)
    {
        fscanf(input, "%d", &lineNum[i]);
    }

    fgetc(input);
    char c;
    if (n >= 2)
    {
        while (cur < lineNum[n - 2] + 1)
        {
            c = fgetc(input);
            if (c == '\n')
            {
                cur++;
            }
        }
    }

    filter(input, cur, lineNum[n - 1]);
    return 0;
}

void filter(FILE *input, int begin, int end)
{
    char c = 0, next = 0;
    int i = begin;
    while ((c = fgetc(input)) != EOF && i <= end)
    {
        if (c == '/')
        {
            next = fgetc(input);
            //单行注释
            if (next == '/')
            {
                while (c != '\n')
                {
                    c = fgetc(input);
                }
                printf("\n");
                i++;
            }
            //多行注释
            else if (next == '*')
            {
                c = fgetc(input);
                next = fgetc(input);
                while (c != '*' || next != '/')
                {
                    if (next == '\n')
                    {
                        i++;
                    }
                    if (next == EOF)
                    {
                        break;
                    }
                    c = next;
                    next = fgetc(input);
                }
            }
            else
            {
                printf("/%c", next);
            }
        }
        else
        {
            if (c == '\n')
            {
                i++;
            }
            printf("%c", c);
        }
    }
}