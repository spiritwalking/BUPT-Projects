#include <iostream>
#include <cstring>
using namespace std;
typedef struct Node
{
	int data;
	Node *next;
}LNode,*Linklist;
Linklist heada,headb,pa,pb,p;

Node *creat(int e)
{
    Linklist p = new LNode;
    memset(p,0,sizeof(LNode));
    p->data = e;
    return p;
}

void destroy(Linklist& L)
{
    Linklist p = L->next, q = L;
    while(p){
        delete (q);
        q = p;
        p = p->next;
    }
    delete (q);
}

int main()
{
	heada=new Node;
	pa=heada;
	
	headb=new Node;
	pb=headb;
	
	int t,a=0,b=0,i,j;
	bool flag = false;

	p = heada;
	cin>>t;
	while(t!=-1)
	{
		a++;
		p->next = creat(t);
        p = p->next;
        cin >> t;
	}

	p = headb;
	cin>>t;
	while(t!=-1)
	{
		b++;
		p->next = creat(t);
        p = p->next;
        cin >> t;
	}

	
	if(a<b)
	{
		cout<<"ListB is not the sub sequence of ListA.";
	}
	else
	{
		i=1;
		j=0;
		pa=heada->next;
		p=pa; 
		pb=headb->next;
		while(i<=a&&i+j-1<a)
		{
			if(pb->data==pa->data)
			{
				flag=true;
				j++;
				pb=pb->next;
				pa=pa->next;
			}
			else
			{
				j=0;
				flag=0;
				pb=headb->next; 
				p=p->next;
				pa=p;
				i++;
			}
			if(j==b&&flag)
			{
				cout<<"ListB is the sub sequence of ListA.";
				break;
			}
		}
		if(j!=b||flag==0)
		{
			cout<<"ListB is not the sub sequence of ListA.";
		}
	}

	destroy(heada);
	destroy(headb);
	return 0;
}
