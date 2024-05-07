#include <iostream>
#include <fstream>
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

void printTree(Node* tree, int level = 0, bool isRight = false) {
  if (tree == NULL) return;
  printTree(tree->right, level + 1, true);

  if (!level) cout << "-->"; // марафет для корня дерева
  else cout << "   ";

  for (int i = 0; i < level; i++) {
    if (i + 1 == level) isRight ? cout << ".-->" : cout << "`-->"; // отступ для уровня
    else cout << "    ";
  }
  
  cout << tree->key << "\n";

  printTree(tree->left, level + 1);
}

void printTreeToFile(Node* tree, ofstream& db, int level = 0, bool isRight = false) {
  if (tree == NULL) return;
  printTreeToFile(tree->right, db, level + 1, true);

  if (!level) db << "-->"; // марафет для корня дерева
  else db << "   ";

  for (int i = 0; i < level; i++) {
    if (i + 1 == level) isRight ? db << ".-->" : db << "`-->"; // отступ для уровня
    else db << "    ";
  }
  
  db << tree->key << "\n";

  printTreeToFile(tree->left, db, level + 1);
}

Node* search(int key, Node* root) {
  if (root != NULL) {
    if (key == root->key) return root;
    if (key < root->key) return search(key, root->left);
    else return search(key, root->right);
  }
  return NULL;
}

void insert(Node* root, int key) { 
  if (key == root->key) return;

  if (key < root->key) {
    if (root->left != NULL) insert(root->left, key);
    else {
      root->left = new Node;
      root->left->key = key;
      root->left->left = NULL; 
      root->left->right = NULL;
    }
  } else {
    if (root->right != NULL) insert(root->right, key);
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

// void readTreeFromFile(Node* root) {
//   ifstream database("tree.txt");
// 	if (database.is_open()) {
// 		char data[1];
// 		while (database.getline(data, DATA_SIZE)) {

//     }
// 		database.close();
// 		return;
// 	}
// }

void writeTreeToFile(Node* root) {
  ofstream database("tree.txt");
	if (!database.is_open()) {
	  cout << '\n' << "Saving error!";
	} else {
    printTreeToFile(root, database);
  }
}


int main() {
  setlocale(LC_ALL, "Russian");

  Node* root = new Node{10};
  root->left = new Node{6};
  root->right = new Node{19};
  root->left->left = new Node{2};
  root->left->left->right = new Node{3};
  root->left->left->right->right = new Node{4};
  root->left->right = new Node{6};
  root->right->left = new Node{11};
  root->right->right = new Node{29};

  printTree(root);
  // insert(root, 18);
  // printTree(root);

  short int workPoint;

  while(true) {
    cout << "\nNavigation\n"
         << "2) Print tree\n";

    cin.clear(); // Clearing the input stream from possible errors
    cin.sync();

    cout << "Select point of work (number 1 to N): ";
    cin >> workPoint;

    switch (workPoint) {   
      case 1: {
        break;
      }
      case 2: {
        cout << "\nBinary tree now on your screens!!11!1:\n";
        printTree(root);
        cout << "\n(And in tree.txt file)\n";
        writeTreeToFile(root);
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