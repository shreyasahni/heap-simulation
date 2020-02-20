#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<iomanip>
#include<math.h>
#define MAX_SIZE 10

using namespace std;

typedef struct Node_tag {
	bool allot;
	int size;
	long int startAddr;
	int count;
}Node;

/* allot==false means block exists in free list, allot==true means block does not exist in free list, i.e. it is ALLOTTED */

typedef enum {SUCCESS, FAILURE}statuscode;

typedef struct ptr_tag {
	statuscode sc;
	long int address;
}ptr;

//total 1024 blocks of each size  can be allocated

void initialise(Node freeList[]) {
	int i, addr;
	freeList[0].startAddr = addr = 0;
	for (i = 0;i < MAX_SIZE;i++) {
		freeList[i].allot = true;
		freeList[i].count = 0;
		freeList[i].size = (int)pow(2, i);
		freeList[i + 1].startAddr = addr = addr + (1024*freeList[i].size);	//BAKCHODI FOR TEJOMAY TO HANDLE
		//cout << freeList[i].startAddr << "\n";
	}
	freeList[MAX_SIZE].allot = false;
	freeList[MAX_SIZE].size = (int)pow(2, MAX_SIZE);
	return;
}

int findIndex(long addr, Node freeList[]) {		//BAKCHODI
	int i, flag = 0, retval = -1;
	for (i = 0;(i < MAX_SIZE) && (flag == 0);i++) {
		if (addr>=freeList[i].startAddr && addr<freeList[i+1].startAddr) {
			flag = 1;
			retval = i;
		}
	}
	if (addr >= freeList[i].startAddr && addr < 2096128) {
		retval = i;
	}
	return retval;
}

int bestFit(int size) {		//returns index of free list array from where block of size "size" will be allocated
	int i = 2;
	int retval = 0;
	while (i < size) {
		i *= 2;
	}
	retval = (int)(log(i) / log(2));
	//retval++;
	//cout << retval<< endl;
	return retval;
}


ptr allocate(int size, Node freeList[]) {
	int i, t;
	ptr p;
	i = bestFit(size);
	t = i;
	if (freeList[t].allot == false) {
		freeList[t].allot = true;
		freeList[t].count++;
		//cout << freeList[i].count;
		p.sc = SUCCESS;
		p.address = freeList[t].startAddr + (freeList[i].size * freeList[i].count);
		//cout << p.address << "\n";
	}
	else {
		do {
			t++;
		} while (freeList[t].allot == true && t<=MAX_SIZE);
		if (t <= MAX_SIZE) {
			freeList[t].allot = true;
			do {
				t--;
				freeList[t].allot = false;
			} while (t != i);
			freeList[t].allot = true;
			freeList[t].count++;
			p.sc = SUCCESS;
			p.address = freeList[t].startAddr + (freeList[i].size * freeList[i].count);
		}
		else {
			p.sc = FAILURE;
		}
	}
	return p;
}

statuscode deallocate(ptr *p, Node freeList[]) {
	int i;
	statuscode sc=FAILURE;
	i = findIndex(p->address, freeList);
	//cout << i << "\n";
	if (i == -1 || p->sc == FAILURE) {
		sc = FAILURE;
	}
	else {
		p->address = -1;
		p->sc = FAILURE;
		freeList[i].count--;
		sc = SUCCESS;
		while (freeList[i].allot == false) {	//coalescing
			freeList[i].allot = true;
			i++;
		}
		freeList[i].allot = true;
	}
	return sc;
}


void main() {
	Node freeList[MAX_SIZE + 1];
	initialise(freeList);
	ptr p1, p2, p3, p4;
	int size;
	statuscode sc;
	cout << "Enter size of block you want to allocate: ";
	cin >> size;
	p1 = allocate(size, freeList);
	if (p1.sc == FAILURE) {
		cout << "\nError allocating memory";
	}
	else {
		cout << "\nMemory allocated at address: " << p1.address;
	}
	cout << "\nEnter size of block you want to allocate: ";
	cin >> size;
	p2 = allocate(size, freeList);
	if (p2.sc == FAILURE) {
		cout << "\nError allocating memory";
	}
	else {
		cout << "\nMemory allocated at address: " << p2.address;
	}
	cout << "\nEnter size of block you want to allocate: ";
	cin >> size;
	p3 = allocate(size, freeList);
	if (p3.sc == FAILURE) {
		cout << "\nError allocating memory";
	}
	else {
		cout << "\nMemory allocated at address: " << p3.address;
	}
	cout << "\nEnter size of block you want to allocate: ";
	cin >> size;
	p4 = allocate(size, freeList);
	if (p4.sc == FAILURE) {
		cout << "\nError allocating memory";
	}
	else {
		cout << "\nMemory allocated at address: " << p4.address;
	}
	sc=deallocate(&p1, freeList);
	if (sc == FAILURE) {
		cout << "\nError deallocating memory";
	}
	else {
		cout << "\nMemory freed";
	}
	sc=deallocate(&p2, freeList);
	if (sc == FAILURE) {
		cout << "\nError deallocating memory";
	}
	else {
		cout << "\nMemory freed";
	}
	sc=deallocate(&p3, freeList);
	if (sc == FAILURE) {
		cout << "\nError deallocating memory";
	}
	else {
		cout << "\nMemory freed";
	}
	sc=deallocate(&p4, freeList);
	if (sc == FAILURE) {
		cout << "\nError deallocating memory";
	}
	else {
		cout << "\nMemory freed";
	}
	sc=deallocate(&p1, freeList);
	if (sc == FAILURE) {
		cout << "\nError deallocating memory";
	}
	else {
		cout << "\nMemory freed";
	}
	return;
}

