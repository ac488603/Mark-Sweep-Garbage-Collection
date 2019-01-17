#include <iostream>
using namespace std; 
/*
Adam Castillo 
Csci 117 lab 
The purpose of this lab is to simulate garbage collection using the mark sweep mechanism.   
*/

// node class, blueprint for a memory location 
class node{
     public:
	 node(){mark = 0; next = NULL;}
		int getMark(){ return mark;}
		void setMark(int m){ mark = m;}
		void setKey(int k) {key = k;}
		int getKey(){return key;}
		node *getNext() {return next;}
		void setNext(node *n){next = n; }
		~node(){}
	private:
	int key;
	int mark;
	node *next;  
};

//function prototypes
int findIndex(node *p); 
void Delete(node**,  int);
void Garbage_Collect(); 
void mark(node**);
void sweep(node**); 
void Insert(node**,int); 
void Print_memory();
void Print_List(node*);
void InsertToFreeList(node**); 
void resetMark(); 

node * memConfig = NULL;  // memory 
node *Free = NULL;       // pointer to head of free list 
node *H1,*H2 = NULL;   // head nodes for two linked list

int main(){
	bool  con = true; 
	// switch variables
	string dec;
	int key;
	int choice = 0; 
	
	InsertToFreeList(&memConfig); //  Putting all nodes on to free list 
	Free = memConfig;   // all blocks of memory begin on freelist 

	while(con){
		
	cout << "what would you like to do" << endl; 
	cout << "1. Print Memory Contents"<< endl;
	cout << "2. Insert new node from freelist to specific linked list" << endl;
	cout << "3. Delete a node from a specific link list and place it on linked list" << endl;
	cout << "4. Garbage Collection " << endl; 
	cout << "5. Quit" << endl; 
	cout << "Choice:";  
	if (!(cin >> choice)){    // error checking 
	cin.clear();             //  clear error flag 
	cin.ignore(10000,'\n'); //  ignore anything in buffer
	}
	switch(choice){
	case 1:
		Print_memory();
		break; 
	case 2:
		cout << "Insert into list1 or list2: ";
		cin >> dec; // error checking prevents cases "list 2"
		cout << endl << "The insert key is:";
		cin >> key; 
		if (dec == "list1"){
			Insert(&H1, key);
		}
		else if(dec == "list2"){
			Insert(&H2, key); 
		}
		else {
			cerr << "The list you entered does not exist" << endl << endl;  //  error checking for wrong list 
			cin.clear();             //  clear error flag 
			cin.ignore(10000,'\n'); //  ignore anything in buffer
		}
		break;
		
	case 3:
		cout << "Delete from list1 or list2: ";
		cin >> dec;
		cout << endl << "The key to be deleted is:";
		cin >> key; 
		if (dec == "list1"){
			Delete(&H1, key);
		}
		else if(dec == "list2"){
			Delete(&H2, key); 
		}
		else {
			cout << "The list you entered does not exist" << endl<< endl;  // error checking 
			cin.clear();             //  clear error flag 
	cin.ignore(10000,'\n'); //  ignore anything in buffer
		}
		break;
	case 4:
		Garbage_Collect();  //  contains mark and sweep functions 
		break;  
	case 5:
		cout << "Exiting..." << endl;
		con = false; //  change loop condtion to false to exit 
		break;
	default:
		cout << "Input not recognized" << endl << endl; //  error checking for invalid input
		break;  	
	}	
	}
	return 0; 
}
	
	//Add a newly created nodes into freelist 
	void InsertToFreeList(node **mem){  // pass pointer by reference
	 node *memConfig =  new node[10];   // allocate 10 space of memory on the heap
	 for(int i =0; i < 9; i++){
		node *temp = &memConfig[i];
		temp->setNext(&memConfig[i+1]); // connect memory spaces
	 }
	*mem = memConfig; // dereference pointer to use it,  mem now points to 10 spaces
					  // of memory that are linked together 
	}
	
	//starts at the begining of memory and prints all information to screen 
	// also prints where the head of each list is located  
	void Print_memory(){
		
		cout << "     [1] [2] [3] [4] [5] [6] [7] [8] [9] [10]" << endl; 
		cout << "Key:  ";
		
		for(int i = 0;  i < 10; i++){
			if (memConfig[i].getKey()== 0)  //starts in first location in memory prints keys
			cout << "    ";                    // if key zero,  print space 
		else
			cout << memConfig[i].getKey() << "   "; //  else print key
		}
		cout<< endl << "Next: "; 
		for(int i = 0; i < 10; i++){
		int j = findIndex(memConfig[i].getNext());  // finds index of an elements given its address
			if(j > 0 )
			cout << j << "   ";	   
			else 
			cout << j << "  ";   // if zero print less spaces after )(visual)
	}
		cout << endl << "Mark: ";
		for(int i = 0; i < 10; i++)
			cout << memConfig[i].getMark() << "   ";  //  prints markbits of each space in memory
		
		cout << endl << endl << "       "; 
		cout << "H1 = " << findIndex(H1); // prints index of head of list
		cout << "  " << "H2 = " << findIndex(H2) << "       "; 
		cout << "Free = " << findIndex(Free) << endl << endl; 
	}
	
	// Prints an individual list 
	void Print_List(node *head){
		while(head != NULL){   // null is end of list
			cout << head->getKey() << " ";  //  prints key 
			head = head->getNext();        //  increments pointer 
		}
			cout << endl;  
	}

	//  inserts nodes into desired list
	//  all nodes come from free list 
	// if freelist empty, error occurs 
	void Insert(node** head, int k){
		node *temp = Free; 
		
		if(temp == NULL){ //  error checking
			cerr << "No more nodes in free list" << endl << endl;
			return;  //  changes made so we can return 
		}
		
		Free = Free->getNext(); //  move head of freelist
		temp->setKey(k);        //  set key that user specified
		temp->setNext(NULL);   //  detatch from freelist 
		
		if(*head == NULL){ // if list is empty then 
		   *head = temp;   // new node becomes head of linklist
		   return;      // changes made so we can return
		}
		else if(k < (*head)->getKey()){ // key is less then head, then key should be head of list 
			temp ->setNext(*head);  //  new node placed in front of list  
			*head = temp;      //  new node becomes head of list
			return; // changes made so return 
		}
		
		node *p = *head;
		while(p->getNext() != NULL){
			if (k > (p->getNext())->getKey()){ // if key is greater than curr 
				p = p->getNext();              // increment pointer 
			}
			else{
				temp->setNext(p->getNext());  //  key is not greater so we can insert new node 
				p->setNext(temp);            
				return; 
			}
		}
		
		if(k > p->getKey())   // end of list condition 
			p->setNext(temp); //  add new node to end of list 
	}

	// given the address of element finds location in memory array 
int findIndex(node *p){
	for(int i = 0;  i < 10; i++){
		if(p == &memConfig[i]) //  compare address rather than key to avoid 
			return i+1;       // finding dupicate key 
	}
	return -1;               // if not found return -1
}

//deletes key from desired list 
// not placed on freelist, just detatched
// placing on freelist handled by garbage collection

void Delete(node **head,  int k){
	node *temp = *head;
	if(temp == NULL){  //  error cheking 
		cerr << "Nothing to be deleted" << endl << endl; 
		return; 
	}
	
	if((*head)->getKey() == k){ //   if key to be deleted is in head
		*head = (*head)->getNext();  //  increment pointer
		return;                     // no reference to head 
	}
	
	while(temp->getNext() != NULL){
	if (k != (temp->getNext())->getKey()){ // if current key doesnt need to be deleted
				temp = temp->getNext();    //  increment pointer 
		}
	else{
		node* delnode = temp->getNext(); // node to be deleted
		temp->setNext(delnode->getNext()); // node before delnode pointer is changed removing delnode from list
		return;                            //  no refernece to delnode 
		}
	}
}

// marks nodes that are accesible through pointers
// places any nodes that arent accesible on freellist 
// clears all markbits 
 void Garbage_Collect(){
	mark(&H1);
	mark(&H2);
	mark(&Free); 
	sweep(&Free); 
	resetMark(); 
}

// marks any node that is accesible through head pointer 
void mark(node **list){
	node *temp = *list; // use temp to avoid changing head of list  
	 while(temp != NULL){ //  null  is end of list
	temp->setMark(1);    //   set mark bit 
 	temp = temp->getNext(); //  increment  pointer 
	 }	
}

// starts at first memory location , checks mark bits 
// if mark bit not set,  node is garbage
// otherwise node is still in use 
void sweep(node **f){
	
	for(int i = 0;  i < 10; i++){
		if(memConfig[i].getMark() != 0 )
			continue;  // move on to next location
		else{
			memConfig[i].setNext(*f); // add node to begining of freelist 
			memConfig[i].setKey(0);   // remove key from node 
			*f = &memConfig[i];       // freelist pointer has to point to beginin 
		}
	}
}

//  clears markbits 
void resetMark(){
		for(int i = 0;  i < 10; i++)
			memConfig[i].setMark(0); 
	}
