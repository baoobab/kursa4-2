#include <iostream>
using namespace std;


struct Node {
  int key;
  Node* left;
  Node* right;
};

unsigned sizeTree(Node* root, unsigned size = 0) {
	if (root == NULL) return 0;
	
	size += sizeTree(root->left, size);
	size += sizeTree(root->right, size);
  return size;
}

void printTree(Node* tree, int level = 0) {
  if (tree == NULL) return;

  printTree(tree->right, level + 1);

  for (int i = 0; i < level; i++) cout << "    "; // отступ для уровня
  
  cout << tree->key << endl;

  printTree(tree->left, level + 1);
}

Node* search(int key, Node* root) {
  if (root != NULL) {
    if (key == root->key) return root;
    if (key < root->key) return search(key, root->left);
    else return search(key, root->right);
  }
  return NULL;
}

void insert(int key, Node* root) {                   
  if (key < root->key) {
    if (root->left != NULL) insert(key, root->left);
    else {
      root->left = new Node;
      root->left->key = key;
      root->left->left = NULL; 
      root->left->right = NULL;
    }
  } else if (key >= root->key) {
    if (root->right != NULL) insert(key, root->right);
    else {
      root->right = new Node;
      root->right->key = key;
      root->right->left = NULL;
      root->right->right = NULL;
    }
  }
}

void destroyTree(Node* &root) {
  if(root == NULL) return;

  destroyTree(root->left);
  destroyTree(root->right);
  delete root;
  root = NULL;
}

int getRandomValueFromRange(int leftLimit, int rightLimit) {
    return leftLimit + rand() % (rightLimit - leftLimit + 1);
}


int main() {
  setlocale(LC_ALL, "Russian");

  Node* root = new Node{10};
  root->left = new Node{6};
  root->right = new Node{19};
  root->left->left = new Node{-2};
  root->left->left->right = new Node{-12};
  root->left->left->right->right = new Node{-22};
  root->left->right = new Node{6};
  root->right->left = new Node{-7};
  root->right->right = new Node{-10};

  printTree(root);
  short int workPoint;

  while(true) {
    cout << "\nNavigation\n"
         << "N) ????\n";

    cin.clear(); // Clearing the input stream from possible errors
    cin.sync();

    cout << "Select point of work (number 1 to N): ";
    cin >> workPoint;

    switch (workPoint) {   
      case 1: {
        break;
      }
      default: {
        cout << "\n" << "You did not enter a number in the range from 1 to N";
        break;
      }
    }
    cin.clear(); // Clearing the input stream from possible errors
    cin.sync();

    char stopFlag;
    cout << "\n" << "Continue the program? (Y/N) ";
    cin >> stopFlag;
        
    if (stopFlag != 'Y' && stopFlag != 'y') break;
  }

  return 0;
}