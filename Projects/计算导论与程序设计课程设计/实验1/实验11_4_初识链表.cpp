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

Node *creat(int e)
{
    Linklist p = new LNode;
    memset(p,0,sizeof(LNode));
    p->data = e;
    return p;
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
    int t,sum=0,min,max;

    cin >> t;
    min = max = t;
    while(t!=-1)
    {
        p->next = creat(t);
        p = p->next;
        cin >> t;
    }

    p = L->next;
    while(p)
    {
        if(min>p->data)
        {
            min = p->data;
        }
        if(max<p->data)
        {
            max = p->data;
        }
        sum += p->data;
        p = p->next;
    }

    cout << "The maximum,minmum and the total are:" << max << ' ' << min << ' ' << sum << endl;
    destroy(L);

    return 0;
}