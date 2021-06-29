#include <stdio.h>

int main()
{
    int n,i;
    scanf("%d", &n);
    FILE *input = fopen("dict.dic", "rb");

    char a, aa[5];
    short b, bb[5];
    int c, cc[5];
    long d, dd[5];
    long long e, ee[5];
    float g, gg[5];
    double h, hh[5];
    
    for (i = 1; i <= n; i++)
    {
        fread(&a, sizeof(a), 1, input);
        fread(&b, sizeof(b), 1, input);
        fread(&c, sizeof(c), 1, input);
        fread(&d, sizeof(d), 1, input);
        fread(&e, sizeof(e), 1, input);
        fread(&g, sizeof(g), 1, input);
        fread(&h, sizeof(h), 1, input);
        fread(aa, sizeof(a), 5, input);
        fread(bb, sizeof(b), 5, input);
        fread(cc, sizeof(c), 5, input);
        fread(dd, sizeof(d), 5, input);
        fread(ee, sizeof(e), 5, input);
        fread(gg, sizeof(g), 5, input);
        fread(hh, sizeof(h), 5, input);
    }

    printf("%c\n", a);
    printf("%hd\n", b);
    printf("%d\n", c);
    printf("%ld\n", d);
    printf("%lld\n", e);
    printf("%f\n", g);
    printf("%lf\n", h);
    printf("%s\n", aa);
    for (i = 0; i < 4; i++)
    {
        printf("%hd ", bb[i]);
    }
    printf("%hd\n", bb[i]);
    for (i = 0; i < 4; i++)
    {
        printf("%d ", cc[i]);
    }
    printf("%d\n", cc[i]);
    for (i = 0; i < 4; i++)
    {
        printf("%ld ", dd[i]);
    }
    printf("%ld\n", dd[i]);
    for (i = 0; i < 4; i++)
    {
        printf("%lld ", ee[i]);
    }
    printf("%lld\n", ee[i]);
    for (i = 0; i < 4; i++)
    {
        printf("%f ", gg[i]);
    }
    printf("%f\n", gg[i]);
    for (i = 0; i < 4; i++)
    {
        printf("%lf ", hh[i]);
    }
    printf("%lf\n", hh[i]);
    fclose(input);
    return 0;
}