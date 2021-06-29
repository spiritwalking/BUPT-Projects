#include <iostream>
#include <cstring>
using namespace std; 
typedef struct Node 
{ 
  int data; 
  Node *next; 
}LNode,*Linklist; 
Linklist head,p,ptr,s1,s2,t1,t2;
int cnt=0;

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
	int m;
	cin>>m;
	head=new Node;
	ptr=head; 
	while(m!=-1)
	{
		ptr->next = creat(m);
        ptr = ptr->next;
        cin >> m;
	}
	int a,b,c,d;
	cin>>a>>b>>c>>d;
	p=head->next;
	ptr=head;
	while(p->next!=NULL)
	{
		cnt++;
		if(cnt==a){
			s1=ptr;
		}else if(cnt==b){
			s2=p;
		}else if(cnt==c){
			t1 = ptr;
		}else{
			t2 = p;
		}
		
		p=p->next;
		ptr=ptr->next;
	}
	cnt++;
	if(cnt==a){
		s1=ptr;
	}else if(cnt==b){
		s2=p;
	}else if(cnt==c){
		t1 = ptr;
	}else{
		t2 = p;
	}
	
	p=t1->next;
	t1->next=s1->next;
	s1->next=p;
	p=t2->next;
	t2->next=s2->next;
	s2->next=p;
	
	p=head->next;
	cout<<"The new list is:";
	while(p->next!=NULL)
	{
		cout<<p->data<<' ';
		p=p->next; 
	}
	
	cout<<p->data;
	destroy(head);
	return 0;
}
