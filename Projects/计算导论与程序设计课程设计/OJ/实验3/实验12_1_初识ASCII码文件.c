#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

void task1(FILE *input);
void task2(FILE *input);
void task3(FILE *input);

int main()
{
    FILE *input = fopen("dict.dic", "r");
    int mode;
    scanf("%d", &mode);
    switch (mode)
    {
    case 1:
        task1(input);
        break;
    case 2:
        task2(input);
        break;
    case 3:
        task3(input);
        break;

    default:
        break;
    }
    fclose(input);
    return 0;
}

void task1(FILE *input)
{
    char c;
    int cnt[4] = {0};
    c = fgetc(input);
    while (c != EOF)
    {
        if (c >= 'A' && c <= 'Z')
        {
            cnt[0]++;
        }
        else if (c >= 'a' && c <= 'z')
        {
            cnt[1]++;
        }
        else if (c <= '9' && c >= '0')
        {
            cnt[2]++;
        }
        else
        {
            cnt[3]++;
        }
        c = fgetc(input);
    }
    printf("Task1:\n");
    printf("capital: %d\n", cnt[0]);
    printf("lowercase: %d\n", cnt[1]);
    printf("digit: %d\n", cnt[2]);
    printf("others: %d\n", cnt[3]);
}

void task2(FILE *input)
{
    int cnt1 = 0, cnt2 = 0, max = 0, min = 0;
    char c = fgetc(input);
    while (c != EOF)
    {
        if (c != '\n')
        {
            //char++
            cnt2++;
        }
        else
        {
            //line++
            cnt1++;
            max = MAX(max, cnt2);
            if (min == 0)
            {
                min = cnt2;
            }
            else
            {
                min = MIN(min, cnt2);
            }
            cnt2 = 0;
        }
        c = fgetc(input);
        if (c == EOF)
        {
            max = MAX(max, cnt2);
            if (min == 0)
            {
                min = cnt2;
            }
            else
            {
                min = MIN(min, cnt2);
            }
            break;
        }
    }

    printf("Task2:\nline: %d\n", cnt1);
    printf("%d characters in max line.\n", max);
    printf("%d characters in min line.\n", min);
}

void task3(FILE *input)
{
    int i;
    int capital[30] = {0}, lowercase[30] = {0};
    char ch;
    for (ch = fgetc(input); !feof(input); ch = fgetc(input))
    {
        if (ch - 'A' >= 0 && ch - 'Z' <= 0)
            capital[ch - 'A']++;
        if (ch - 'a' >= 0 && ch - 'z' <= 0)
            lowercase[ch - 'a']++;
    }
    printf("Task3:\n");
    printf("CAPITAL:\n");
    for (i = 0; i <= 25; i++)
    {
        printf("%c:%d\n", i + 'A', capital[i]);
    }
    printf("LOWERCASE:\n");
    for (i = 0; i <= 25; i++)
    {
        printf("%c:%d\n", i + 'a', lowercase[i]);
    }
}