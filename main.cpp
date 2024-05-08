#include <iostream>
#include <fstream>
using namespace std;

#define MAX_STRING_SIZE 50

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

void printNode(Node* node) {
  if (node == NULL) return;
  cout << "\nNode:"
  << "\n L: " << node->left
  << "\n Key: " << node->key
  << "\n R: " << node->right
  << "\n";
}

Node* search(int key, Node* root) {
  if (root != NULL) {
    if (key == root->key) return root;
    if (key < root->key) return search(key, root->left);
    else return search(key, root->right);
  }
  return NULL;
}

void insert(Node* &root, int key) { 
  if (root == NULL) {
    root = new Node;
    root->key = key;
    root->left = NULL; 
    root->left = NULL;
    return;
  } 
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

void readTreeFromFile(Node* &root, char* filename) {
  ifstream database(filename);
	if (!database.is_open()) {
    cout << "\nFile with name " << filename << " doesn't exists!\n";
    return;
  }
  database.seekg(0);
	char data[MAX_STRING_SIZE];
	while (database.getline(data, MAX_STRING_SIZE)) insert(root, atoi(data));
  		
  database.close();
	return;
	
}

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
  srand(time(0));

  Node* root = new Node{10};
  root->left = new Node{6};
  root->right = new Node{19};
  root->left->left = new Node{2};
  root->left->left->right = new Node{3};
  root->left->left->right->right = new Node{4};
  root->right->left = new Node{11};
  root->right->right = new Node{29};

  printTree(root);
  // insert(root, 18);
  // printTree(root);

  short int workPoint;
  short int chooseType;
  while(true) {
    cout << "\nNavigation\n"
         << "1) Create a new tree\n"
         << "2) Print tree\n";

    cin.clear(); // Clearing the input stream from possible errors
    cin.sync();

    cout << "Select point of work (number 1 to N): ";
    cin >> workPoint;

    switch (workPoint) {   
      case 1: {
        cout << "\nChoose how you want to init the tree\n"
        << "\n(1) Enter the size of the tree and fill with random"
        << "\n(2) Enter the numbers whatever you want"
        << "\n(3) Read from file\n";

        cin >> chooseType;
        destroyTree(root);
        printTree(root);

        switch (chooseType) {
          case 1: {
            short elementsCount;
            cout << "The size of tree is: ";
            cin >> elementsCount;
            if (!cin.good()) {
              cout << "\nYou entered an incorrect value\n";
              break;
            }
            for (int i = 0; i < elementsCount; i++) insert(root, getRandomValueFromRange(-99, 99));
            break;
          }
          case 2: {
            int item;

            cout << "Enter items, to stop it - enter any char\n";

            cin.clear(); // Clearing the input stream from possible errors
            cin.sync();

            while (cin >> item) insert(root, item);
            break;
          }
          case 3: {
            char filename[MAX_STRING_SIZE];

            cout << "Enter filename (with extension, from 0 to " << MAX_STRING_SIZE << " letters, end of name is </>): ";
            
            cin.get();
            cin.getline(filename, MAX_STRING_SIZE, '/');
            readTreeFromFile(root, filename);
            break;
          }
          default: {
            cout << "\nYou entered an incorrect value\n";
            break;
          }
        }
        cout << "\nCreated tree:\n";
        printTree(root);

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