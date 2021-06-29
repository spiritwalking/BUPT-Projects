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
    while(p)
    {
        delete (q);
        q = p;
        p = p->next;
    }
    delete (q);
}

void adjust(Linklist& La,Linklist& Lb)
{
    int flag;
    Linklist pa = La->next, pb = Lb->next;
    Linklist qa = La, qb = Lb;
    while(pb)
    {
        flag = 0;
        pa = La->next;
        qa = La;
        while(1)
        {
            if(pa==NULL)
            {
                qa->next = pb;
                qb->next = NULL;
                break;
            }

            //have same member
            if(pa->data==pb->data)
            {
                break;
            }

            //no same member
            if (pa->data > pb->data)
            {
                qa->next = pb;
                qb->next = pb->next;
                pb->next = pa;

                flag = 1;
                pb = qb->next;
                break;
            }
            qa = qa->next;
            pa = pa->next;
        }
        if(pa==NULL)
        {
            break;
        }
        if(flag==0)
        {
            qb = qb->next;
            pb = pb->next;
        }
    }

}

int main(void)
{
    Linklist La = creat(0),Lb = creat(0),p = La;
    int t;

    //read list A
    cin >> t;
    while(t!=-1)
    {
        p->next = creat(t);
        p = p->next;
        cin >> t;
    }

    //read list B
    p = Lb;
    cin >> t;
    while(t!=-1)
    {
        p->next = creat(t);
        p = p->next;
        cin >> t;
    }

    adjust(La, Lb);

    if(La->next)
    {
        cout << "The new list A:";
        print(La);
    }
    else
    {
        cout << "There is no item in A list." << endl;
    }
    
    if(Lb->next)
    {
        cout << "The new list B:";
        print(Lb);
    }
    else
    {
        cout << "There is no item in B list." << endl;
    }
    
    destroy(La);
    destroy(Lb);

    return 0;
}