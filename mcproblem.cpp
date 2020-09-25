#include <bits/stdc++.h>
using namespace std;
#define MIN(X, Y, Z) min(X, min(Y, Z))
struct Item{
	int missionaries; //the missionaries on the left bank
	int cannibals;	  //the number of cannibals on the left bank
	char boat;		  //the number of ships on the left bank
	int depth;		  //depth in search tree
	int f;			  /*hueristic  function f(x)=g(x)+h(x)
						here g(x) = depth and
						h(x) = missionaries + cannibals - 2 * boat;
					  */
	Item * parent;    //parent node
};

struct Node{
	Item item;
	struct Node * next;
};

typedef Node * List;
//number of missionaries and cannibals
int NUMBER = 3;
int CAPACITY = 2;

//interface functions   
void InitializeList(List * plist){
	*plist = NULL;
}

bool ListIsEmpty(const List * plist){
	if (*plist == NULL)
		return true;
	else
		return false;
}

bool ListIsFull(const List * plist){
	Node * pt = new Node;
	bool full;
	if (pt == NULL)
		full = true;
	else
		full = false;
	free(pt);

	return full;
}

unsigned int ListItemCount(const List * plist){
	unsigned int count = 0;
	Node * pnode = *plist;    // set to start of list 
	while (pnode != NULL){
		++count;			  // increment the count of nodes
		pnode = pnode->next;  // advance to next node   
	}
	return count;
}

// local function definition  
// copies an item into a node 
static void CopyToNode(Item item, Node * pnode){
	pnode->item = item; 
}

bool AddItem(const Item * pItem, List * plist){	
	Node * temp = new Node;
	if (temp == NULL)
		return false;  // Not able to create more nodes
	CopyToNode(*pItem, temp);
	if (ListIsEmpty(plist)){
		*plist = temp;
		temp->next = NULL;
	}
	else{
		Node * p = *plist;
		//As we are using A* Star Algo, we need to insert the node in the OPEN LIST according to the heuristic function values.
		//Ascending Order by heuristic function value
		if (p->item.f >= pItem->f){
			*plist = temp;
			temp->next = p;
		}
		else{	
			while (p->next != NULL && p->next->item.f < pItem->f)
				p = p->next;
			temp->next = p->next;
			p->next = temp;
		}
	}
	return true;
}


bool headInserted(const Item * pItem, List * plist){
	Node * pnew = new Node;
	if (pnew == NULL)	// Not able to create more nodes
		return false;
	CopyToNode(*pItem, pnew);
	pnew->next = *plist;
	*plist = pnew;
	return true;
}


bool DeleteItem(List * plist){
	if (ListIsEmpty(plist))	//empty list
		return false;
	else{
		Node * temp = *plist;
		*plist = temp->next;
		delete temp;
		return true;
	}
}

// visit each node 
bool Traverse(const List * plist, const Item * pItem){
	Node * pnode = *plist;    // set to start of list   
	Item temp;
	while (pnode != NULL){
		temp = pnode->item;
		if ((pItem->missionaries == temp.missionaries) && (pItem->cannibals == temp.cannibals) && (pItem->boat == temp.boat) && (pItem->depth > temp.depth))
			return true;
		pnode = pnode->next;  // advance to next item   
	}
	return false;
}

// delete memory allocated by new
// set list pointer to NULL          
void EmptyTheList(List * plist){
	Node * psave;
	while (*plist != NULL){
		psave = (*plist)->next; // save address of next node 
		delete *plist;           // delete current node         
		*plist = psave;         // advance to next node      
	}
}

void showItem(const Item * pItem, FILE * fp)	{
	printf(" (%2d,%2d,%2d) \n", pItem->missionaries, pItem->cannibals, pItem->boat);
	fprintf(fp, " (%2d,%2d,%2d) \n", pItem->missionaries, pItem->cannibals, pItem->boat);
}

bool isLegal(const Item * pItem){
	if ((pItem->missionaries != 0) && (pItem->missionaries != NUMBER)){
		if (pItem->missionaries == pItem->cannibals)
			return true;
		else
			return false;
	}
	else
		return true;
}

char compute_F(Item * pItem){
	pItem->f = pItem->depth + (pItem->missionaries + pItem->cannibals - 2 * pItem->boat);
	return pItem->f;
}

void update(Item * pItem, List * pOpenList){
	compute_F(pItem);
	if (AddItem(pItem, pOpenList) == false){
		fprintf(stderr, "Problem allocating memory\n\n");
		exit(EXIT_FAILURE);
	}
}

void display(const Item * pItem, FILE * fp){
	static int step;
	if (pItem->parent->parent == NULL){
		step = 1;
		printf("\tStep %3d: ", step);
		fprintf(fp, "\tStep %3d: ", step);
		if (pItem->boat == 0){
			cout<<" send "<<pItem->parent->missionaries - pItem->missionaries<<" missionaries and "<<pItem->parent->cannibals - pItem->cannibals<<" cannibals, so final state is";
			fprintf(fp, " send %2d missionaries and %2d cannibals, so final state is ", pItem->parent->missionaries - pItem->missionaries, pItem->parent->cannibals - pItem->cannibals);
		}
		else{
			cout<<" return "<<pItem->missionaries - pItem->parent->missionaries<<" missionaries and "<<pItem->cannibals - pItem->parent->cannibals<<" cannibals, so final state is";
			fprintf(fp, " return %2d missionaries and %2d cannibals, so final state is ", pItem->missionaries - pItem->parent->missionaries, pItem->cannibals - pItem->parent->cannibals);
		}
		showItem(pItem, fp);
	}
	else{
		display(pItem->parent, fp);
		printf("\tStep %3d: ", ++step);
		fprintf(fp, "\tStep %3d: ", step);
		if (pItem->boat == 0){
			cout<<" send "<<pItem->parent->missionaries - pItem->missionaries<<" missionaries and "<<pItem->parent->cannibals - pItem->cannibals<<" cannibals, so final state is";
			fprintf(fp, " send %2d missionaries and %2d cannibals, so final state is ", pItem->parent->missionaries - pItem->missionaries, pItem->parent->cannibals - pItem->cannibals);
		}
		else{
			cout<<" return "<<pItem->missionaries - pItem->parent->missionaries<<" missionaries and "<<pItem->cannibals - pItem->parent->cannibals<<" cannibals, so final state is";
			fprintf(fp, " return %2d missionaries and %2d cannibals, so final state is ", pItem->missionaries - pItem->parent->missionaries, pItem->cannibals - pItem->parent->cannibals);
		}
		showItem(pItem, fp);
	}
}

bool Expand(Item * pItem, List * pOpenList, const List * pCloseList){
	bool boolFlag = false;
	int deltaM, deltaC;	// change in number of missionaries and cannabils
	Item temp;
	//Generating all the child nodes
	if (pItem->boat == 0){
		//CASE 1: BOAT ON RIGHT SIDE
		for (deltaM = 1; deltaM <= min(NUMBER - pItem->missionaries, CAPACITY); deltaM++){
			for (deltaC = 0; deltaC <= MIN(deltaM, NUMBER - pItem->cannibals, CAPACITY - deltaM); deltaC++){
				temp = *pItem;
				temp.missionaries += deltaM;
				temp.cannibals += deltaC;
				temp.boat = 1;
				temp.depth++;
				//Verifing whether a legal step or not and whether it is a duplicate of a node present in CLOSED LIST but with greater depth in search space(
				//this is done because we desire the shortest path and according to that, this node is useless for us.
				if (isLegal(&temp) && !Traverse(pCloseList, &temp)){
					temp.parent = pItem;
					boolFlag = true;
					update(&temp, pOpenList);
				}
			}
		}
		deltaM = 0;
		for (deltaC = 1; deltaC <= min(NUMBER - pItem->cannibals, CAPACITY); deltaC++){
			temp = *pItem;
			temp.cannibals += deltaC;
			temp.boat = 1;
			temp.depth++;
			//Verifing whether a legal step or not and whether it is a duplicate of a node present in CLOSED LIST but with greater depth in search space(
			//this is done because we desire the shortest path and according to that, this node is useless for us.
			if (isLegal(&temp) && !Traverse(pCloseList, &temp)){
				temp.parent = pItem;
				boolFlag = true;
				update(&temp, pOpenList);
			}
		}
	}
	else{
		//CASE 2: BOAT ON LEFT SIDE 
		for (deltaM = 1; deltaM <= min(pItem->missionaries, CAPACITY); deltaM++){
			for (deltaC = 0; deltaC <= MIN(deltaM, pItem->cannibals, CAPACITY - deltaM); deltaC++){
				temp = *pItem;
				temp.missionaries -= deltaM;
				temp.cannibals -= deltaC;
				temp.boat = 0;
				temp.depth++;
				//Verifing whether a legal step or not and whether it is a duplicate of a node present in CLOSED LIST but with greater depth in search space(
				//this is done because we desire the shortest path and according to that, this node is useless for us.
				if (isLegal(&temp) && !Traverse(pCloseList, &temp)){
					temp.parent = pItem;
					boolFlag = true;
					update(&temp, pOpenList);
				}
			}
		}
		deltaM = 0;
		for (deltaC = 1; deltaC <= min(pItem->cannibals, CAPACITY); deltaC++){
			temp = *pItem;
			temp.cannibals -= deltaC;
			temp.boat = 0;
			temp.depth++;
			//Verifing whether a legal step or not and whether it is a duplicate of a node present in CLOSED LIST but with greater depth in search space(
			//this is done because we desire the shortest path and according to that, this node is useless for us.
			if (isLegal(&temp) && !Traverse(pCloseList, &temp)){
				temp.parent = pItem;
				boolFlag = true;
				update(&temp, pOpenList);
			}
		}
	}
	//Tells whether expansion was successful as if it is not, we need to check the OPEN LIST
	return boolFlag;
}

void solveMC(Item state, FILE * fp){
	printf("Initial state: ");
	fputs("Initial state: ", fp);
	showItem(&state, fp);
	cout<<"The capacity of the ship is "<<CAPACITY<<endl;
	fprintf(fp, "The capacity of the ship is %d.\n", CAPACITY);
	List openList;
	List closeList;

	InitializeList(&openList);
	if (ListIsFull(&openList)){
		fprintf(stderr, "No Memory available! Bye!\n\n");
		exit(EXIT_FAILURE);
	}

	InitializeList(&closeList);
	if (ListIsFull(&closeList)){
		fprintf(stderr, "No Memory available! Bye!\n\n");
		exit(EXIT_FAILURE);
	} 

	if (AddItem(&state, &openList) == false){
		fprintf(stderr, "Problem allocating memory\n\n");
		exit(EXIT_FAILURE);
	}

	int methodNumber = 1;
	int minimalSteps = 0; // For storing the steps in best path

	while (ListIsEmpty(&openList) == false){
		//Insert the first element from the OPEN LIST to the CLOSED LIST
		if (headInserted(&(openList->item), &closeList) == false){
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}
		//Delete the inserted element from the OPEN LIST
		DeleteItem(&openList);
		if((methodNumber == 1) || (closeList->item.depth <= minimalSteps)){
			if ((closeList->item.missionaries == 0) && (closeList->item.cannibals == 0) && (closeList->item.boat == 0)){
				if (methodNumber == 1){
					minimalSteps = closeList->item.depth;
					printf("\nThe minimal steps are: \t%d steps.\n", minimalSteps);
					fprintf(fp, "\nThe minimal steps are: \t%d steps.\n", minimalSteps);
				}
				fprintf(fp, "\nMethod %d: \n", methodNumber);
				printf("\nMethod %d: \n", methodNumber++);
				display(&(closeList->item), fp);
			}
			else
				Expand(&(closeList->item), &openList, &closeList);
		}
		else
			continue;
	}
	
	if (!minimalSteps){
		cout<<"\nNo solution!\n";
		fputs("\nNo solution!\n\n", fp);
	}

	//empty
	EmptyTheList(&openList);
	EmptyTheList(&closeList);
}

int main()
{
	cout<<"The program is implemented in C++ language, using the A* star,the status information in a state contains the following:\nthe missionaries on the left bank, the number of cannibalson the left bank and number of ships on the left bank, represented as a triplet(m,c,b),\nalso when the ship is on the left bank, the corresponding value is 1 and when on the right side, it is 0.\n\n";
	//Opening output file to store the output 
	FILE * fp;
	char filename[] = "output.txt";
	if ((fp = fopen(filename, "w")) == NULL)
	{
		printf("Can't create output file \"%s\"!\n", filename);
		exit(EXIT_FAILURE);
	}
	//printing to the user
	cout<<"Look at an example first: \n";
	//Writing in the file
	fputs("Look at an example first: \n\n", fp);
	Item state0 = { NUMBER, NUMBER, 1, 0, 0, NULL };
	compute_F(&state0);
	solveMC(state0, fp);
	cout<<"Please enter the number of missionaries and cannibals(to exit enter -1): \n";
	putchar('\t');
	while (scanf("%d", &NUMBER)){
		if(NUMBER==-1){
			break;
		}
		cout<<"Please enter the capacity of the ship: \n";
		scanf("%d", &CAPACITY);
		Item state = { NUMBER, NUMBER, 1, 0, 0, NULL };
		putchar('\n');
		solveMC(state, fp);
		cout<<"\n\n";
		cout<<"Please enter the number of missionaries and cannibals(to exit enter -1): \n";
	}
	cout<<"\nThank you!\n";
	fputs("\nThank you!\n", fp);
	cout<<"\nBye!\n";
	//closing the output file
	if (fclose(fp) != 0)
		printf("Error in closing file %s!\n", filename);
	getchar();
	getchar();
	return 0;
}

