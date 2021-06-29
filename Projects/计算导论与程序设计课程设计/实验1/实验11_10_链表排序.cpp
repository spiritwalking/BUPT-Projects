#include <iostream>
#include <cstring>
using namespace std;

typedef struct Node
{
    int data;
    Node *next;
}LNode,*Linklist;

Linklist creat(int e);
void sort(Linklist& L);
void print(Linklist L);
void destroy(Linklist& L);
int length = 0;

Node *creat(int e)
{
    Linklist p = new LNode;
    memset(p,0,sizeof(LNode));
    p->data = e;
    return p;
}

void sort(Linklist& L)
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
    cout << "The new list is:";
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
    int t;

    cin >> t;
    while(t!=-1)
    {
        p->next = creat(t);
        length++;
        p = p->next;
        cin >> t;
    }

    sort(L);
    print(L);
    destroy(L);

    return 0;
}