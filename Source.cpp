#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<iomanip>
#include<math.h>
#include<vector>
#include<iterator>
#define MAX_SIZE 10

using namespace std;

typedef struct Node_tag 
{
	bool allot;
	int size;
	// long int startAddr;
	int count;
}Node;

/* allot==false means block exists in free list, allot==true means block does not exist in free list */

typedef enum {SUCCESS, FAILURE} statuscode;

typedef struct ptr_tag 
{
	statuscode sc;
	int address;
	int size;
}ptr;

//total 1024 blocks of each size  can be allocated
vector<ptr> alloclist;

void initialize(Node freeList[]) 
{
	int i,size;
	size = 1;
	for(i=0;i<MAX_SIZE;i++)
	{
		freeList[i].allot = true;
		freeList[i].size = size;
		size *= 2;
		freeList[i].count = 0;
	}
	freeList[i].allot = false;
	freeList[i].size = size;
	freeList[i].count = 0;
}

int findPow(int size) 
{		
	int twopow = 1;
	while(size>twopow)
	{
		twopow *= 2;
	}
	return twopow;
}

int bestFit(int size) 		//returns index of free list array from where block of size "size" will be allocated
{
	int i = 2;
	int retval = 0;
	while (i < size) 
	{
		i *= 2;
	}
	retval = (int)(log(i) / log(2));
	return retval;
}


ptr allocate(int size, Node freeList[]) 
{
	int i, t,offset;
	ptr p;
	i = bestFit(size);
	t = i;
	if (freeList[t].allot == false) 
	{
		freeList[t].allot = true;
		freeList[t].count++;
		p.sc = SUCCESS;
		if(alloclist.empty())
		{	
			p.address = 0;
		}	
		else
		{
			offset = findPow(alloclist[alloclist.size()-1].size);
			p.address = alloclist[alloclist.size()-1].address + offset;
		}
		p.size = size;
	}
	else 
	{
		do 
		{
			t++;
		} while (freeList[t].allot == true && t<=MAX_SIZE);
		if (t <= MAX_SIZE) 
		{
			freeList[t].allot = true;
			do 
			{
				t--;
				freeList[t].allot = false;
			}while (t != i);
			//freeList[t].allot = true;
			freeList[t].count++;
			p.sc = SUCCESS;
			if(alloclist.empty())
			{	
				p.address = 0;
			}	
			else
			{
				offset = findPow(alloclist[alloclist.size()-1].size);
				p.address = alloclist[alloclist.size()-1].address + offset;
			}
			p.size = size;
		}
		else 
		{
			p.sc = FAILURE;
		}
	}
	return p;
}

statuscode deallocate(int addr,Node freeList[]) 
{
	int offset,j;
	vector<ptr>::iterator temp,i;
	bool found = false;
	statuscode sc = SUCCESS;
	found=0;
	for(temp=alloclist.begin();temp!=alloclist.end() && !found;temp++)
	{
		if((*temp).address == addr)
		{
			found = true;
			i = temp;
		}	
	}
	if(!found)
		sc = FAILURE;
	else
	{
		offset = findPow((*i).size);
		alloclist.erase(i);
		while(i!=alloclist.end())
		{
			(*i).address -= offset;
			i++;
		}	
		j = bestFit(offset);
		freeList[j].count--;
		while (freeList[j].allot == false) 		//coalescing
		{	
			freeList[j].allot = true;
			j++;
		}
		freeList[j].allot = false;
	}	
	return sc;
}

void display()
{
	vector<ptr>::iterator i;
	cout<<"\nAllocated memory:\n";
	for(i=alloclist.begin();i!=alloclist.end();i++)
	{
		cout<<"Address: "<<(*i).address<<"\tSize: "<<(*i).size<<"\n";
	}
	cout<<"\n";
}

int main() 
{
	statuscode sc;
	Node freeList[MAX_SIZE + 1];
	initialize(freeList);
	ptr p;
	int size,addr;
	char opt,ch;
	ch = 'Y';
	cout<<"-----------------------HEAP MANAGEMENT!!!!!!!--------------------------\n";
	while(ch=='y' || ch=='Y')
	{
		cout<<"1)ALLOCATE\n2)DEALLOCATE\n";
		cout<<"Select option(1-2): ";
		cin>>opt;
		switch(opt)
		{
			case '1':
				{
					cout<<"\nEnter size to be allocated: ";
					cin>>size;
					p = allocate(size,freeList);
					if(p.sc == FAILURE)
					{
						cout<<"\nFailed to allocate block!!!!!!!\n";
					}
					else
					{
						alloclist.push_back(p);
						display();
					}
					break;
				}	
			case '2':
				{
					cout<<"\nEnter address to be deallocated: ";
					cin>>addr;
					sc = deallocate(addr,freeList);
					if(sc == FAILURE)
						cout<<"Address does not exist!!!!!\n";
					else
					{
						if(!alloclist.empty())
							display();
					}
					break;
				}	
			default: cout<<"\nEnter Valid Choice !!!!!!!!!!!!!!!\n";	
		}
		cout<<"\nPress 'Y' to continue, 'N' to exit: ";
		cin>>ch;
	}	
	return 0;
}
