#include <iostream>
#include <cstring>
using namespace std;

typedef struct Node
{
    char data;
    Node *next;
}LNode,*Linklist;

Linklist creat(char e);
void sort(Linklist& L);
void print(Linklist L);
void destroy(Linklist& L);

Node *creat(char e)
{
    Linklist p = new LNode;
    memset(p,0,sizeof(LNode));
    p->data = e;
    return p;
}

void sort(Linklist& L,int length)
{
    Linklist p = L->next, pre = L;
    int i, j;
    for (i = 0; i < length - 1; i++)
    {
        p = L->next;
        pre = L;
        for (j = 0; j < length - 1 - i; j++)
        {
            if(p->data > p->next->data)
            {
                pre->next = p->next;
                p->next = p->next->next;
                pre->next->next = p;
                pre = pre->next;
            }else if(p)
            {
               p = p->next;
               pre = pre->next;
            }
        }
    }
}

void print(Linklist L)
{
    Linklist p = L->next;
    while(p->next)
    {
        cout << p->data << ' ';
        p = p->next;
    }
    cout << p->data << endl;
}

void destroy(Linklist& L)
{
    LNode *p = L->next, *q = L;
    while(p){
        delete (q);
        q = p;
        p = p->next;
    }
    delete (q);
}

int main(void)
{
    Linklist L = creat(0),p = L;
    Linklist La = creat(0), Lb = creat(0), Lc = creat(0);
    Linklist pa = La, pb = Lb, pc = Lc;
    char s[1000]={0};
    int len1 = 0, len2 = 0, len3 = 0;

    gets(s);
    for (int i = 0; s[i] != '-' || s[i + 1] != '1'; i++)
    {
        if(s[i]==' ')
        {
            continue;
        }
        p->next = creat(s[i]);
        p = p->next;
    }

    //distribute members
    p = L->next;
    while(p)
    {
        if((p->data>='a'&&p->data<='z')||(p->data>='A'&&p->data<='Z'))
        {
            pa->next = creat(p->data);
            pa = pa->next;
            len1++;
        }
        else if(p->data>='0'&&p->data<='9')
        {
            pb->next = creat(p->data);
            pb = pb->next;
            len2++;
        }
        else
        {
            pc->next = creat(p->data);
            pc = pc->next;
            len3++;
        }
        p = p->next;
    }

    // print(La);
    // print(Lb);
    // print(Lc);
    sort(La,len1);
    sort(Lb,len2);
    sort(Lc,len3);

    if(La->next)
    {
        cout << "The list A is: ";
        print(La);
    }
    else
    {
        cout << "There is no item in A list." << endl;
    }

    if(Lb->next)
    {
        cout << "The list B is: ";
        print(Lb);
    }
    else
    {
        cout << "There is no item in B list." << endl;
    }

    if(Lc->next)
    {
        cout << "The list C is: ";
        print(Lc);
    }
    else
    {
        cout << "There is no item in C list." << endl;
    }


    destroy(L);
    destroy(La);
    destroy(Lb);
    destroy(Lc);

    return 0;
}