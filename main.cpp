#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
using namespace std;

#define MAX_STRING_SIZE 50
#define MAX_TASKS_COUNT 5
#define TASK_TOPICS_COUNT 4

enum TasksTopics {
  CREATE = 0,
  READ = 1,
  UPDATE = 2,
  DELETE = 3
};

struct Node {
  int key;
  Node* left;
  Node* right;
};

int getRandomValueFromRange(int leftLimit, int rightLimit) {
    return leftLimit + rand() % (rightLimit - leftLimit + 1);
}

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

Node* findMaxNode(Node* root) {
  while (root->right) {
    root = root->right;
  }
  return root;
}

Node* findMinNode(Node* root) {
  while (root->left) {
    root = root->left;
  }
  return root;
}

Node* search(int key, Node* root) {
  if (root == NULL) return NULL;
    
  if (key == root->key) return root;
  if (key < root->key) return search(key, root->left);
  else return search(key, root->right);
  
  return NULL;
}

Node* searchParentByChild(Node* root, Node* child) {
  if (root == NULL || child == NULL) return NULL;
    
  if (root->left == child || root->right == child) return root;
  if (child->key < root->key) return searchParentByChild(root->left, child);
  else return searchParentByChild(root->right, child);
  
  return NULL;
}

void destroyTree(Node* &root) {
  if(root != NULL) {
    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
    root = NULL;
  }
}

void del(Node* root, int key) {
  Node* delNode = search(key, root);
  Node* parentNode = searchParentByChild(root, delNode);

  if (delNode == NULL) return;
  if (parentNode == NULL) { // если удаляемый узел - корень
    if (delNode->right != NULL && delNode->left != NULL) {
      Node* delChild = findMaxNode(delNode->left);
      int childKey = delChild->key;

      del(delNode, childKey);
      delNode->key = childKey;
      return;
    }

    if (delNode->right != NULL) {
      root = delNode->right;
      delete delNode;
      return;
    } else if (delNode->left != NULL) {
      root = delNode->right;
      delete delNode;
      return;
    }

    destroyTree(root);
    return;
  }

  if (delNode->right == NULL && delNode->left == NULL) {
    delNode->key < parentNode->key ? parentNode->left = NULL : parentNode->right = NULL;

    delete delNode;
    delNode = NULL;
    return;
  } else if (delNode->right == NULL && delNode->left != NULL) {
    delNode->key < parentNode->key ? parentNode->left = delNode->left : parentNode->right = delNode->left;

    delete delNode;
    delNode = NULL;
    return;
  } else if (delNode->right != NULL && delNode->left == NULL) {
    delNode->key < parentNode->key ? parentNode->left = delNode->right : parentNode->right = delNode->right;

    delete delNode;
    delNode = NULL;
    return;
  } else {
    Node* delChild = findMaxNode(delNode->left);
    int childKey = delChild->key;

    del(delNode, childKey);
    delNode->key = childKey;
    return;
  }
}

void insert(Node* &root, int key) { 
  if (root == NULL) {
    root = new Node;
    root->key = key;
    root->left = NULL; 
    root->left = NULL;
    return;
  } 

  if (key < root->key) {
    if (root->left != NULL) insert(root->left, key);
    else {
      root->left = new Node;
      root->left->key = key;
      root->left->left = NULL; 
      root->left->right = NULL;
    }
  } else if (key > root->key) {
    if (root->right != NULL) insert(root->right, key);
    else {
      root->right = new Node;
      root->right->key = key;
      root->right->left = NULL;
      root->right->right = NULL;
    }
  }
}

void readTreeFromFile(Node* &root, char* filename) {
  ifstream database(filename);
  if (!database.is_open()) {
    cout << "\nFile with name " << filename << " doesn't exists!\n";
    return;
  }

  char data[MAX_STRING_SIZE];
  while (database.getline(data, MAX_STRING_SIZE)) insert(root, atoi(data));
      
  database.close();
  return;
  
}

void writeTreeToFile(Node* root, ofstream& db) {
  if (!db.is_open()) {
    cout << '\n' << "Saving error!";
  } else {
    printTreeToFile(root, db);
  }
}

void preOrderTravers(Node* root) { // прямой обход - сверху вниз
  if (root) {
    cout << root->key << " ";
    preOrderTravers(root->left);
    preOrderTravers(root->right);
  }
}

void inOrderTravers(Node* root) { // симм. обход - слева направо
  if (root) {
    inOrderTravers(root->left);
    cout << root->key << " ";
    inOrderTravers(root->right);
  }
}

void postOrderTravers(Node* root) { // обратный обход - снизу вверх
  if (root) {
    postOrderTravers(root->left);
    postOrderTravers(root->right);
    cout << root->key << " ";
  }
}

// Array functions

bool checkArrayIndex(int* &arr, const unsigned size, unsigned idx) {
  return (idx >= 0 && idx < size);
}

int countArrayEntries(int* &arr, const unsigned size, int data) {
  int counter = 0;
  for (int i = 0; i < size; i++) if (arr[i] == data) counter++;
  return counter;
}

void printElement(int data, unsigned idx = -1) {
  cout << "\nItem\n"
  << " Data: " << data << "\n";
  if (idx != -1) cout << " Index: " << idx << "\n";
}

int findArrayItemByIndex(int* &arr, const unsigned size, const unsigned idx) {
  if (!checkArrayIndex(arr, size, idx)) {
    cout << "\nIncorrect index value\n";
    return 0;
  }
  for (int i = 0; i < size; i++) if (i == idx) return arr[i];
  return 0;
}

unsigned getIndexOfArrayItem(int* &arr, const unsigned size, int data, unsigned startIdx = 0) {
  for (int i = startIdx; i < size; i++) if (arr[i] == data) return i;
  return -1;
}

void printArrayItemsByData(int* &arr, const unsigned size, int data) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == data) printElement(arr[i], i);
  }
}

void delArrayItemByIndex(int* &arr, unsigned &size, unsigned idx) {
  if (!checkArrayIndex(arr, size, idx)) {
    cout << "\nIncorrect index value\n";
    return;
  }
  int* res = new int[size - 1];
  for (int i = 0, j = 0; i < size; i++) {
    if (i != idx) {
      res[j] = arr[i];
      j++;
    }
  }
  delete[] arr;
  size--;
  arr = res;
}

void delArrayItemByValue(int* &arr, unsigned &size, int data) {
  while (countArrayEntries(arr, size, data) > 0) {
    int index = getIndexOfArrayItem(arr, size, data);
    delArrayItemByIndex(arr, size, index);
  }
}

void insArrayItem(int* &arr, unsigned &size, unsigned idx, int data){
  if (!checkArrayIndex(arr, size, idx) && idx != size) {
    cout << "\nIncorrect index value\n";
    return;
  }
  size++;
  int *res = new int[size];
  for (int i = 0; i < idx; i++) {
      res[i] = arr[i];
  }
  res[idx] = data;
  for (int i = idx; i < size - 1; i++) {
      res[i+1] = arr[i];
  }
  delete[] arr;
  arr = res;
}

void clearArray(int* &arr, unsigned &size) {
  delete[] arr;
  arr = NULL;
  size = 0;
}

void printArray(int* &arr, const unsigned size) {
  for (int i = 0; i < size; i++) printElement(arr[i], i);
}

void swapArrayElementsByIndexes(int* &arr, const unsigned size, unsigned idx1, unsigned idx2) {
  if (!checkArrayIndex(arr, size, idx1) || !checkArrayIndex(arr, size, idx2)) {
    cout << "\nIncorrect index value(-s)\n";
    return;
  }
  int buffer = arr[idx1];
  arr[idx1] = arr[idx2];
  arr[idx2] = buffer;
}

void arrShuffle(int* arr, const int size) {
  for (int i = size - 1; i >= 1; i--) {
    int j = rand() % (i + 1);
    int tmp = arr[j];
    arr[j] = arr[i];
    arr[i] = tmp;
  }
}

//


void generateTopicHead(int topic, string& head) {
  switch (topic) {
    case TasksTopics::CREATE: {
      head = "***Creation tasks***";
      return;
    }
    case TasksTopics::READ: {
      head = "\n\n***Search tasks***";
      return;
    }
    case TasksTopics::UPDATE: {
      head = "\n\n***Insert tasks***";
      return;
    }
    case TasksTopics::DELETE: {
      head = "\n\n***Removal tasks***";
      return;
    }
  }
}

void convertTreeToArray(Node* root, int* &arr, unsigned& size) {
  if (root) {
    convertTreeToArray(root->left, arr, size);
    insArrayItem(arr, size, size, root->key);
    convertTreeToArray(root->right, arr, size);
  }
}

void generateTaskCondition(int topic, string& head, Node* &taskRoot, int* taskValues) {
  int elementsCount = getRandomValueFromRange(6, 12);
  int nodesMinBorder = getRandomValueFromRange(0, 100);
  int nodesMaxBorder = nodesMinBorder + elementsCount - 1;

  switch (topic) {
    case TasksTopics::CREATE: {
      head = "\nCreate a tree of ";
      head += to_string(elementsCount);
      head += " elements, with nodes from ";
      head += to_string(nodesMinBorder);
      head += " to ";
      head += to_string(nodesMaxBorder);
      taskValues[0] = nodesMinBorder;
      taskValues[1] = nodesMaxBorder;
      return;
    }
    case TasksTopics::READ: {
      taskValues[0] = getRandomValueFromRange(0, 10);
      for (int i = 0; i < elementsCount; i++) insert(taskRoot, getRandomValueFromRange(0, 10));

      head = "\nCheck if the value ";
      head += to_string(taskValues[0]);
      head += " exists in the tree below\n";
      return;
    }
    case TasksTopics::UPDATE: {
      taskValues[0] = getRandomValueFromRange(-20, 20);
      for (int i = 0; i < elementsCount; i++) insert(taskRoot, getRandomValueFromRange(-30, 30));

      head = "\nCheck where node with value ";
      head += to_string(taskValues[0]);
      head += " will be after inserting it into the tree from below\n";
      return;
    }
    case TasksTopics::DELETE: {
      for (int i = 0; i < elementsCount; i++) insert(taskRoot, getRandomValueFromRange(-30, 30));

      head = "\nCheck what the tree will look like after removing node with value ";
      int* arr = NULL;
      unsigned arrSize = 0;
      convertTreeToArray(taskRoot, arr, arrSize);
      taskValues[0] = arr[rand() % arrSize];
      head += to_string(taskValues[0]);
      head += " from it\n";
      return;
    }
  }
}

void generateTaskShortAnswer(int topic, string& head, Node* &taskRoot, int* taskValues) {
  switch (topic) {
    case TasksTopics::CREATE: {
      head = "\nOne of the possible variants of tree\n";
      int arrSize = taskValues[1] + 1 - taskValues[0];
      int arr[arrSize] = {0};
      int idx = 0;
      for (int i = taskValues[0]; i < taskValues[1] + 1; i++) arr[idx++] = i;
      arrShuffle(arr, arrSize);
      for (int i = 0; i < arrSize; i++) insert(taskRoot, arr[i]);
      return;
    }
    case TasksTopics::READ: {
      if (search(taskValues[0], taskRoot) != NULL) {
        head = "\nValue ";
        head += to_string(taskValues[0]);
        head += " is IN the tree\n";
      } else {
        head = "\nValue ";
        head += to_string(taskValues[0]);
        head += " is NOT IN the tree\n";
      }
      return;
    }
    case TasksTopics::UPDATE: {
      head = "\nTree after inserting\n";
      insert(taskRoot, taskValues[0]);
      return;
    }
    case TasksTopics::DELETE: {
      head = "\nTree after deleting\n";
      del(taskRoot, taskValues[0]);
      return;
    }
  }
}

void generateTaskAnswer(int topic, string& head, Node* &taskRoot, int* taskValues) {
  switch (topic) {
    case TasksTopics::CREATE: {
      head = "\nOne of the possible variants of tree ";
      head += "(because you can fill the tree with any value, in any order)\n";
      return;
    }
    case TasksTopics::READ: {
      if (search(taskValues[0], taskRoot) != NULL) {
        head = "\nValue ";
        head += to_string(taskValues[0]);
        head += " is IN the tree\n";
      } else {
        head = "\nValue ";
        head += to_string(taskValues[0]);
        head += " is NOT IN the tree\n";
      }
      return;
    }
    case TasksTopics::UPDATE: {
      head = "\nTree after inserting node with value ";
      head += to_string(taskValues[0]);
      head += " in it\n";
      insert(taskRoot, taskValues[0]);
      return;
    }
    case TasksTopics::DELETE: {
      head = "\nTree after deleting node with value ";
      head += to_string(taskValues[0]);
      head += " from it\n";
      del(taskRoot, taskValues[0]);
      return;
    }
  }
}


void writeTasksInFiles(unsigned tasksCount) {
  ofstream dbk("outputKey.txt"); // короткие ответы
  ofstream dba("outputAns.txt"); // полные ответы
  ofstream dbt("outputTask.txt"); // условия
  if (!dbt.is_open()) {
    cout << '\n' << "Saving error!";
    return;
  }

  string topicHead; // заголовок раздела
  string taskCondition; // условие задачи
  string taskShortAnswer; // короткий ответ на задачу
  string taskAnswer; // полный ответ на задачу
  
  Node* taskRoot = NULL;

  for (int topic = 0; topic < TASK_TOPICS_COUNT; topic++) { // для разделов задач
    generateTopicHead(topic, topicHead);
    dbt << topicHead;
    dbk << topicHead;
    dba << topicHead;
    for (int taskIdx = 0; taskIdx < tasksCount; taskIdx++) { // для задач внутри раздела
      int taskValues[2] = {0, 0}; // неизвестные, которые используются в задаче

      generateTaskCondition(topic, taskCondition, taskRoot, taskValues);
      dbt << taskCondition;
      if (taskRoot) writeTreeToFile(taskRoot, dbt);

      generateTaskShortAnswer(topic, taskShortAnswer, taskRoot, taskValues);
      dbk << taskShortAnswer;
      if (taskRoot) writeTreeToFile(taskRoot, dbk);

      generateTaskAnswer(topic, taskAnswer, taskRoot, taskValues);
      dba << taskAnswer;
      if (taskRoot) writeTreeToFile(taskRoot, dba);

      destroyTree(taskRoot);
      taskRoot = NULL;
    }
  }
  
  dbt.close();
  dba.close();
  dbk.close();
}

int main() {
  setlocale(LC_ALL, "Russian");
  srand(time(0));

  Node* root = NULL; // корень дерева

  int *arr = NULL; // дин. массив (для сравнения времени операций)
  unsigned arrSize = 0;

  short int workPoint;
  short int chooseType;
  char actionType;

  auto start = chrono::steady_clock::now();
  auto end = chrono::steady_clock::now();

  while(true) {
    cout << "\nNavigation\n"
         << "(1) Create a new tree\n"
         << "(2) Print tree (and write to file\n"
         << "(3) Operations with tree\n"
         << "(4) Travers tree\n"
         << "(5) Generate quiz\n"
         << "(6) Create a new array\n"
         << "(7) Operations with array\n";

    cin.clear(); // Clearing the input stream from possible errors
    cin.sync();

    cout << "Select point of work (number 1 to 5): ";
    cin >> workPoint;

    switch (workPoint) {   
      case 1: {
        cout << "\nChoose how you want to init the tree\n"
        << "\n(1) Enter the size of the tree and fill with random"
        << "\n(2) Enter the numbers whatever you want"
        << "\n(3) Read from file\n";

        cin >> chooseType;
        destroyTree(root);
        root = NULL;

        switch (chooseType) {
          case 1: {
            short elementsCount;
            cout << "The size of tree is: ";
            cin >> elementsCount;
            if (!cin.good()) {
              cout << "\nYou entered an incorrect value\n";
              break;
            }
            start = chrono::steady_clock::now();
            for (int i = 0; i < elementsCount; i++) insert(root, getRandomValueFromRange(-99, 99));
            end = chrono::steady_clock::now();
            break;
          }
          case 2: {
            int item;
            cout << "Enter items, to stop it - enter any char\n";

            cin.clear(); // Clearing the input stream from possible errors
            cin.sync();

            start = chrono::steady_clock::now();
            while (cin >> item) insert(root, item);
            end = chrono::steady_clock::now();
            break;
          }
          case 3: {
            char filename[MAX_STRING_SIZE];

            cout << "Enter filename (with extension, from 0 to " << MAX_STRING_SIZE << " letters, end of name is </>): ";
            
            cin.get();
            cin.getline(filename, MAX_STRING_SIZE, '/');
            start = chrono::steady_clock::now();
            readTreeFromFile(root, filename);
            end = chrono::steady_clock::now();
            break;
          }
          default: {
            cout << "\nYou entered an incorrect value\n";
            break;
          }
        }
        cout << "\nCreated tree:\n";
        printTree(root);

        cout << "\nTime to Create: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << "\n";

        break;
      }
      case 2: {
        if (root == NULL) {
          cout << "\nThe tree is empty\n"; 
          break;
        }

        cout << "\nBinary tree now on your screens!!11!1:\n";
        printTree(root);
        cout << "\n(And in tree.txt file)\n";
        ofstream db("tree.txt");
        writeTreeToFile(root, db);
        db.close();
        break;
      }
      case 3: {
        cout << "Choose the action (eng)\n"
        << "(G) - Get element\n"
        << "(I) - Insert element\n"
        << "(D) - Delete element\n";
        
        cin >> actionType;
        cin.clear(); // Clearing the input stream from possible errors
        cin.sync();

        int data;
        cout << "Enter an element value: ";
        cin >> data;
        if (!cin.good()) {
          cout << "\nYou entered an incorrect value\n";
          break;
        }

        switch (actionType) {
          case 'G': {
            start = chrono::steady_clock::now();
            Node* foundedItem = search(data, root);
            end = chrono::steady_clock::now();

            if (foundedItem == NULL) cout << "Item not found\n";
            else cout << "Founded item: " << foundedItem->key << "\n";
            
            cout << "\nTime to search: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << "\n";

            break;
          }
          case 'I': {
            start = chrono::steady_clock::now();
            insert(root, data);
            end = chrono::steady_clock::now();
            cout << "\nUpdated Tree:\n";
            printTree(root);

            cout << "\nTime to insert: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << "\n";
            break;
          }
          case 'D': {
            start = chrono::steady_clock::now();
            del(root, data);
            end = chrono::steady_clock::now();

            cout << "\nUpdated Tree:\n";
            printTree(root);

            cout << "\nTime to delete: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << "\n";
            break;
          }
          default: {
            cout << "\nYou entered an incorrect value\n";
            break;
          }
        }
        break;
      }
      case 4: {
        if (root == NULL) {
          cout << "\nThe tree is empty\n";
          break;
        }

        cout << "\nPre-order: ";
        preOrderTravers(root);
        cout << "\nIn-order: ";
        inOrderTravers(root);
        cout << "\nPast-order: ";
        postOrderTravers(root);

        break;
      }
      case 5: {
        unsigned tasksCount;
        cout << "\nEnter the count of tasks for each topic (create, read, update, delete)"
        << "\n(max count of tasks is " << MAX_TASKS_COUNT << "): "; 
        cin >> tasksCount;
        if (tasksCount > MAX_TASKS_COUNT) tasksCount = MAX_TASKS_COUNT;
        writeTasksInFiles(tasksCount);
        cout << "\nTasks generated succesfully";
        break;
      }
      case 6: {
        cout << "Choose the way (eng)\n"
        << "(A) - Enter the dimension and fill it with random\n"
        << "(B) - Enter the numbers whatever you want\n";
        unsigned curSize = 0;
        char creatingType;
        cin >> creatingType;
        if (!cin.good()) {
          cout << "\nYou entered an incorrect value\n";
          break;
        }
        if (creatingType == 'B' || creatingType == 'b') {
          cout << "Enter items, to stop it - enter any char\n";
          int item;
          cin.clear(); // Clearing the input stream from possible errors
          cin.sync();

          clearArray(arr, arrSize);
          start = chrono::steady_clock::now();
          while (cin >> item) insArrayItem(arr, curSize, curSize, item);
          end = chrono::steady_clock::now();
          arrSize = curSize;

          cout << "\nList length: " << arrSize << "\n";
        } else {
          cout << "\nEnter elements count: ";
          clearArray(arr, arrSize);
          
          cin >> arrSize;
          if (!cin.good()) {
            cout << "\nYou entered an incorrect value\n";
            break;
          }
          start = chrono::steady_clock::now();
          for (int i = 0; i < arrSize; i++) {
            insArrayItem(arr, curSize, i, getRandomValueFromRange(0, 99));
          }
          end = chrono::steady_clock::now();
        }
        printArray(arr, arrSize);
        cout << "\nTime to Create: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << "\n";
        break;
      }
      case 7: {
        cout << "Choose the action (eng)\n"
        << "(G) - Get element\n"
        << "(I) - Insert element\n"
        << "(D) - Delete element\n";

        int foundedItem = 0;
        int data;
        unsigned index;

        cin >> actionType;

        switch (actionType) {
          case 'G': {
            cout << "By index or by value (I/V)?: ";
            cin >> actionType;
            if (!cin.good()) {
              cout << "\nYou entered an incorrect value\n";
              break;
            }
            if (actionType == 'I' || actionType == 'i') {
              cout << "Enter an index of element (length is " << arrSize << "): ";
              cin >> index;
              if (!cin.good()) {
                cout << "\nYou entered an incorrect value\n";
                break;
              }
              start = chrono::steady_clock::now();
              foundedItem = findArrayItemByIndex(arr, arrSize, index);
              end = chrono::steady_clock::now();

              if (!foundedItem) {
                cout << "\nYou entered an incorrect value\n";
                break;
              }
              printElement(foundedItem);
            } else {
              cout << "Enter a value of element: ";
              cin >> data;
              if (!cin.good()) {
                cout << "\nYou entered an incorrect value\n";
                break;
              }
              start = chrono::steady_clock::now();
              printArrayItemsByData(arr, arrSize, data);
              end = chrono::steady_clock::now();
            }
            cout << "\nTime to Get: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << "\n";
            break;
          }
          case 'I': {
            cout << "Enter an index of new element (length is " << arrSize << "): ";
            cin >> index;
            if (!cin.good()) {
              cout << "\nYou entered an incorrect value\n";
              break;
            }
            cout << "Enter a value of element (or enter any char to random): ";
            cin >> data;
            start = chrono::steady_clock::now();
            if (!cin.good()) insArrayItem(arr, arrSize, index, getRandomValueFromRange(0, 99));
            else insArrayItem(arr, arrSize, index, data);
            end = chrono::steady_clock::now();

            printArray(arr, arrSize);
            cout << "\nTime to Insert: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << "\n";
            break;
          }
          case 'D': {
            cout << "By index or by value (I/V)?: ";
            cin >> actionType;
            if (!cin.good()) {
              cout << "\nYou entered an incorrect value\n";
              break;
            }
            if (actionType == 'I' || actionType == 'i') {
              cout << "Enter an index of element (length is " << arrSize << "): ";
              cin >> index;
              if (!cin.good()) {
                cout << "\nYou entered an incorrect value\n";
                break;
              }
              start = chrono::steady_clock::now();
              delArrayItemByIndex(arr, arrSize, index);
              end = chrono::steady_clock::now();
            } else {
              cout << "Enter a value of element: ";
              cin >> data;
              if (!cin.good()) {
                cout << "\nYou entered an incorrect value\n";
                break;
              }
              start = chrono::steady_clock::now();
              delArrayItemByValue(arr, arrSize, data);
              end = chrono::steady_clock::now();
            }

            cout << "\nArray after (length is " << arrSize << "):\n";
            printArray(arr, arrSize);
            cout << "\nTime to Delete: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << "\n";
            break;
          }
          default: {
            cout << "\nYou entered an incorrect value\n";
            break;
          }
        }

        break;
      }
      default: {
        cout << "\n" << "You didn't enter a number in the range from 1 to 5";
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