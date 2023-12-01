#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
using namespace std;

// AVL tree
class Node
{
	public:
	int data;
	Node* left;
	Node* right;
	int height;
};

// new node
Node* newNode(Node* node, int data)
{
	node = new Node();
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;

	return node;
}

// height
int height(Node* node)
{
	if (node == NULL)
		return 0;

	return node->height;
}

// right rotate
Node* right_rotate(Node* root)
{
	Node* root_left = root->left;
	Node* left_right = root_left->right;
	root_left->right = root;
	root->left = left_right;

	if (root != NULL || root->left != NULL || root->right != NULL)
		root->height = ((height(root->left) > height(root->right)) ? height(root->left) : height(root->right)) + 1;
	if (root_left != NULL || root_left->left != NULL || root_left->right != NULL)
		root_left->height = ((height(root_left->left) > height(root_left->right)) ? height(root_left->left) : height(root_left->right)) + 1;

	return root_left;
}

// left rotate
Node* left_rotate(Node* root)
{
	Node* root_right = root->right;
	Node* right_left = root_right->left;
	root_right->left = root;
	root->right = right_left;

	if (root != NULL || root->left != NULL || root->right != NULL)
		root->height = ((height(root->left) > height(root->right)) ? height(root->left) : height(root->right)) + 1;
	if (root_right != NULL || root_right->left != NULL || root_right->right != NULL)
		root_right->height = ((height(root_right->left) > height(root_right->right)) ? height(root_right->left) : height(root_right->right)) + 1;

	return root_right;
}

// get balance
int get_balance(Node* node)
{
	if (node != NULL) {
		if (node->left == NULL && node->right == NULL)
			return 0;
		else if (node->left == NULL && node->right != NULL)
			return 0 - height(node->right);
		else if (node->right == NULL && node->left != NULL)
			return height(node->left) - 0;
		else
			return height(node->left) - height(node->right);
	}

	return 0;
}

Node* minval(Node* node)
{
	Node* current = node;

	while (current->right != NULL)
		current = current->right;

	return current;
}

// insert node
Node* insert_node(Node* node, int data)
{
	if (node == NULL)
		return (newNode(node, data));

	if (data < node->data)
		node->left = insert_node(node->left, data);
	else if (data > node->data)
		node->right = insert_node(node->right, data);
	else
		return node;

	if (node != NULL)
		node->height = 1 + max(height(node->left), height(node->right));

	int balance = get_balance(node);

	// left left
	if (balance > 1 && data < node->left->data)
		return right_rotate(node);

	// right right
	if (balance < -1 && data > node->right->data)
		return left_rotate(node);

	// left right
	if (balance > 1 && data > node->left->data) {
		node->left = left_rotate(node->left);

		return right_rotate(node);
	}

	// right left
	if (balance < -1 && data < node->right->data) {
		node->right = right_rotate(node->right);

		return left_rotate(node);
	}

	return node;
}

// delete node
Node* delete_node(Node* node, int data)
{
	if (node == NULL)
		return node;

	if (data < node->data)
		node->left = delete_node(node->left, data);
	else if (data > node->data)
		node->right = delete_node(node->right, data);
	else
	{
		if ((node->left == NULL) && (node->right == NULL))
			node = NULL;
		else if ((node->left == NULL) && (node->right != NULL))
			node = node->right;
		else if ((node->left != NULL) && (node->right == NULL))
			node = node->left;
		else
		{
			Node* temp = minval(node->left);
			node->data = temp->data;
			node->left = delete_node(node->left, temp->data);
		}
	}

	if (node == NULL)   return node;

	if (node != NULL)
		node->height = 1 + max(height(node->left), height(node->right));

	int balance = get_balance(node);

	// left left
	if (balance > 1 && get_balance(node->left) >= 0)
		return right_rotate(node);

	// right right
	if (balance < -1 && get_balance(node->right) <= 0)
		return left_rotate(node);

	// left right
	if (balance > 1 && get_balance(node->left) < 0) {
		node->left = left_rotate(node->left);

		return right_rotate(node);
	}

	// right left
	if (balance < -1 && get_balance(node->right) > 0) {
		node->right = right_rotate(node->right);

		return left_rotate(node);
	}

	return node;
}

// postorder £¨left, right, root)
void post_order(Node* node)
{
	if (node != NULL)
	{
		post_order(node->left);
		post_order(node->right);
		cout << node->data << " ";
	}
}

// inorder (left, root, right)
void in_order(Node* node)
{
	if (node != NULL)
	{
		in_order(node->left);
		cout << node->data << " ";

		in_order(node->right);
	}

}

// preorder (root, left, right)
void pre_order(Node* node)
{
	if (node != NULL)
	{
		cout << node->data << " ";

		pre_order(node->left);
		pre_order(node->right);
	}

}

int main()
{

	Node* root = NULL;

	string S, T;
	getline(cin, S);
	stringstream X(S);

	vector<string> v;

	while (getline(X, T, ' ')) {
		v.push_back(T);
	}

	int length = v.size();
	for (int i = 0; i < length; i++)
	{
		string moves = v[i];

		// Aint insert
		if (moves[0] == 'A')
		{
			int len = moves.size();
			string d = moves.substr(1, len);
			int data = stoi(d);

			root = insert_node(root, data);
		}

		// Dint delete
		else if (moves[0] == 'D')
		{
			int len = moves.size();
			string d = moves.substr(1, len);
			int data = stoi(d);

			root = delete_node(root, data);
		}
	}

	string order = v[v.size() - 1];
	if (order == "IN")
	{
		in_order(root);
		if (root == NULL)
			cout << "EMPTY" << endl;
	}
	else if (order == "POST")
	{
		post_order(root);
		if (root == NULL)
			cout << "EMPTY" << endl;
	}
	else if (order == "PRE")
	{
		pre_order(root);
		if (root == NULL)
			cout << "EMPTY" << endl;
	}

	return 0;
}