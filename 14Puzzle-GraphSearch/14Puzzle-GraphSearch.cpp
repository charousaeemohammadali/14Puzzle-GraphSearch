#include <iostream>
using namespace std;

struct node {
	node* parent;
	int s[4][4];
	char action;
};

struct listnode {
	node* n;
	listnode* next;
};

listnode* frontier = NULL;
listnode* explored = NULL;
listnode* insert(listnode*, node* n);
node* create_initial_node();
void free_blocks(node*, int[2][2]);
void expand(node*);
void move_up(node* s1, int[2]);
void move_down(node* s1, int[2]);
void move_left(node* s1, int[2]);
void move_right(node* s1, int[2]);
bool check_goal(node*);
bool search(node*, listnode*);
listnode* add_frontier(node*);
listnode* delete_first_frontier();
listnode* add_explored(node*);
void print_output(node* temp);

int main()
{
	frontier = insert(frontier, create_initial_node());
	while (true)
	{
		if (frontier == NULL)
		{
			cout << "FRONTIER IS EMPTY , SOLUTION FAILED ... " << endl;
			system("pause");
			return 0;
		}
		//save the memory of frontier
		node* temp;
		temp = frontier->n;

		//remove from frontier
		frontier = delete_first_frontier();

		//frontier->n > s; add to explored
		explored = insert(explored, temp);

		//goal test
		if (check_goal(temp))
		{
			print_output(temp);
			break;
		}

		//kole farzandan add frontier
		expand(temp);
	}
	system("pause");
	return 0;
}

listnode* insert(listnode* first, node* n)
{
	listnode* temp = new listnode;
	temp->n = n;
	temp->next = NULL;
	if (!first)
		return temp;
	listnode* last = first;
	while (last->next)
		last = last->next;
	last->next = temp;
	return first;
}

node* create_initial_node()
{
	node* n = new node;
	int matrix[4][4] = { { 1, 2, 0, 3 },{ 0,6,7,4 },{ 5,10,11,8 },{ 9,13,14,12 } };
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			n->s[i][j] = matrix[i][j];
	n->parent = NULL;
	return n;
}

void free_blocks(node* s1, int location[2][2])
{
	int flag = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (s1->s[i][j] == 0)
			{
				location[flag][0] = i;
				location[flag][1] = j;
				flag++;
			}
}

void expand(node* L)
{
	int location[2][2];
	free_blocks(L, location);

	move_left(L, location[0]);
	move_right(L, location[0]);
	move_up(L, location[0]);
	move_down(L, location[0]);

	move_left(L, location[1]);
	move_right(L, location[1]);
	move_up(L, location[1]);
	move_down(L, location[1]);
}

void move_up(node* p, int location[2])
{
	int i = location[0];
	int j = location[1];
	if (i == 0) return;
	node* child = new node;
	child->parent = p;
	child->action = 'U';
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			child->s[x][y] = p->s[x][y];
	int v = child->s[i][j];
	child->s[i][j] = child->s[i - 1][j];
	child->s[i - 1][j] = v;
	if (!search(child, frontier) && !search(child, explored))
		frontier = insert(frontier, child);
	else
		delete child;
}

void move_down(node* p, int location[2])
{
	int i = location[0];
	int j = location[1];
	if (i == 3) return;
	node* child = new node;
	child->parent = p;
	child->action = 'D';
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			child->s[x][y] = p->s[x][y];
	int v = child->s[i][j];
	child->s[i][j] = child->s[i + 1][j];
	child->s[i + 1][j] = v;
	if (!search(child, frontier) && !search(child, explored))
		frontier = insert(frontier, child);
	else
		delete child;
}

void move_left(node* p, int location[2])
{
	int i = location[0];
	int j = location[1];
	if (j == 0) return;
	node* child = new node;
	child->parent = p;
	child->action = 'L';
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			child->s[x][y] = p->s[x][y];
	int v = child->s[i][j];
	child->s[i][j] = child->s[i][j - 1];
	child->s[i][j - 1] = v;
	if (!search(child, frontier) && !search(child, explored))
		frontier = insert(frontier, child);
	else
		delete child;
}

void move_right(node* p, int location[2])
{
	int i = location[0];
	int j = location[1];
	if (j == 3) return;
	node* child = new node;
	child->parent = p;
	child->action = 'R';
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			child->s[x][y] = p->s[x][y];
	int v = child->s[i][j];
	child->s[i][j] = child->s[i][j + 1];
	child->s[i][j + 1] = v;
	if (!search(child, frontier) && !search(child, explored))
		frontier = insert(frontier, child);
	else
		delete child;
}

bool check_goal(node* s1)
{
	int goal_matrix[4][4] = { { 1, 2, 3, 4 },{ 5,6,7,8 },{ 9,10,11,12 },{ 13,14,0,0 } };
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (s1->s[i][j] != goal_matrix[i][j])
				return false;
	return true;
}

bool compare(int m1[4][4], int m2[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (m1[i][j] != m2[i][j])
				return false;
	return true;
}

bool search(node* n, listnode* first)
{
	while (first != NULL)
	{
		if (compare(n->s, first->n->s))
			return true;
		first = first->next;
	}
	return false;
}

listnode* delete_first_frontier()
{
	listnode* temp;
	temp = frontier;
	frontier = frontier->next;
	delete(temp);
	return frontier;
}

void print_output(node* goal)
{
	listnode* reverse = new listnode;
	reverse->n = goal;
	reverse->next = NULL;
	goal = goal->parent;
	while (goal)
	{
		listnode* temp = new listnode;
		temp->n = goal;
		temp->next = reverse;
		reverse = temp;
		goal = goal->parent;
	}
	int counter = 1;
	while (reverse)
	{
		cout << counter << endl;
		if (counter != 1)
			cout << reverse->n->action << endl;
		counter++;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				cout << reverse->n->s[i][j] << " ";
			cout << endl;
		}
		cout << "-------------" << endl << endl;
		reverse = reverse->next;
	}
}