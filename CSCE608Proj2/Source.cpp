// C++ program for implementing B+ Tree 
#include <climits> 
#include <fstream> 
#include <iostream> 
#include <sstream> 
#include <time.h>
using namespace std;
//13&7, 24&13
const int MAX       = 24;
const int MAXsparse = 12; //floor((MAX+1)/2)

const int AMOUNT = 10000; //10k values eventually
int value[AMOUNT]; //array to store the random numbers in
bool isPresent = false;

// BP node 
class Node {
    bool IS_LEAF;
    int* key, size;
    Node** ptr;
    friend class BPTree;

public:
    Node();
    void display();
    void beforeInsert(int);
};

// BP tree 
class BPTree {
    Node* root;
    void insertInternal(int,Node*,Node*);
    void insertInternalSparse(int, Node*, Node*);
    Node* findParent(Node*, Node*);

public:
    BPTree();
    Node search(int);
    void rangeSearch(int, int);
    void insert(int);
    void insertSparse(int);
    Node* getRoot();
    void deleteKey(BPTree, int, Node);
};

// Constructor of Node 
Node::Node(){
    key = new int[MAX];
    ptr = new Node * [MAX + 1];
}

void Node::display() {
    int nodes = 0; //counter for number of nodes
    if (key != NULL) {
        for (int i = 0; i < MAX; i++){
            if (key[i] > 0) {
                std::cout << "[" << key[i] << "] ";
                nodes++;
            }
        }
        std::cout << " <-- " << nodes << " nodes present\n";
    } else {
        std::cout << "key is null \n";
    }
}

void Node::beforeInsert(int x) {
    int nodes = 0; //counter for number of nodes
    if (key != NULL) {
        for (int i = 0; i < MAX; i++) {
            if (key[i] > 0 && key[i] != x) {
                std::cout << "[" << key[i] << "] ";
                nodes++;
            } else if(key[i] > 0 && key[i] == key[i+1]){
                std::cout << "[" << key[i] << "] ";
                nodes++;
            }
        }
        std::cout << " <-- " << nodes << " nodes present\n";
    }
    else {
        std::cout << "key is null \n";
    }
}

// Initialise the BPTree Node 
BPTree::BPTree()
{
    root = NULL;
}

// Function to find any element in B+ Tree 
Node BPTree::search(int x){
    // If tree is empty 
    if (root == NULL) {
        std::cout << "Tree is empty\n";
    }
    
    // Traverse to find the value 
    else {

        Node* cursor = root;
        isPresent = false; //initialize boolean
        // Till we reach leaf node 
        while (cursor->IS_LEAF == false) {

            for (int i = 0; i < cursor->size; i++) {

                // If the element to be 
                // found is not present 
                if (x < cursor->key[i]) {
                    cursor = cursor->ptr[i];
                    break;
                }

                // If reaches end of the 
                // cursor node 
                if (i == cursor->size - 1) {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }

        // Traverse the cursor and find 
        // the node with value x 
        for (int i = 0; i < cursor->size; i++) {

            // If found then return 
            if (cursor->key[i] == x) {
                isPresent = true;
                //std::cout <<  x << "; isPresent= " << isPresent << "\n";       
                std::cout << x; // << "\n";
                return *cursor;
            }
        }
        // Else element is not present 
        //cout << x << "; isPresent= " << isPresent << "\n";
        std::cout << x; // << "\n";
        return *cursor;
    }
}

void BPTree::rangeSearch(int min, int max) {
    for (int i = min; i <= max; i++){
        BPTree::search(i);
        if (isPresent == true)
            cout << " FOUND\n";
        else
            cout << " NOT FOUND\n";
    }
}

void BPTree::deleteKey(BPTree a, int x, Node toDelete) {
    cout << "Deleting number: " << x << "\n";
    //toDelete.display();
    if (isPresent == false) {
        a.insert(x);
    }
    std::cout << "Before Delete:\n";
    toDelete.display();
    for (int i = 0; i < MAX; i++){
        if (toDelete.key[i] == x) {
            toDelete.key[i] = -8000;
            if (toDelete.key[i] == -8000 && toDelete.key[i-1] == -8000) {
                toDelete.key[i] == x;
            }
        }
    }
    std::cout << "After Delete:\n";
    toDelete.display();
}

// Function to implement the Insert operation in B+ Tree 
void BPTree::insert(int x)
{

    // If root is null then return 
    // newly created node 
    if (root == NULL) {
        root = new Node;
        root->key[0] = x;
        root->IS_LEAF = true;
        root->size = 1;
    }

    // Traverse the B+ Tree 
    else {
        Node* cursor = root;
        Node* parent = NULL;

        // Till cursor reaches the 
        // leaf node 
        while (cursor->IS_LEAF
            == false) {

            parent = cursor;

            for (int i = 0;
                i < cursor->size;
                i++) {

                // If found the position 
                // where we have to insert 
                // node 
                if (x < cursor->key[i]) {
                    cursor
                        = cursor->ptr[i];
                    break;
                }

                // If reaches the end 
                if (i == cursor->size - 1) {
                    cursor
                        = cursor->ptr[i + 1];
                    break;
                }
            }
        }

        if (cursor->size < MAX) {
            int i = 0;
            while (x > cursor->key[i]
                && i < cursor->size) {
                i++;
            }

            for (int j = cursor->size;
                j > i; j--) {
                cursor->key[j]
                    = cursor->key[j - 1];
            }

            cursor->key[i] = x;
            cursor->size++;

            cursor->ptr[cursor->size]
                = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size - 1] = NULL;
        }
        else {

            // Create a newLeaf node 
            Node* newLeaf = new Node;

            int virtualNode[MAX + 1];

            // Update cursor to virtual 
            // node created 
            for (int i = 0; i < MAX; i++) {
                virtualNode[i]
                    = cursor->key[i];
            }
            int i = 0, j;

            // Traverse to find where the new 
            // node is to be inserted 
            while (x > virtualNode[i]
                && i < MAX) {
                i++;
            }

            // Update the current virtual 
            // Node to its previous 
            for (int j = MAX + 1;
                j > i; j--) {
                virtualNode[j]
                    = virtualNode[j - 1];
            }

            virtualNode[i] = x;
            newLeaf->IS_LEAF = true;

            cursor->size = (MAX + 1) / 2;
            newLeaf->size
                = MAX + 1 - (MAX + 1) / 2;

            cursor->ptr[cursor->size]
                = newLeaf;

            newLeaf->ptr[newLeaf->size]
                = cursor->ptr[MAX];

            cursor->ptr[MAX] = NULL;

            // Update the current virtual 
            // Node's key to its previous 
            for (i = 0;
                i < cursor->size; i++) {
                cursor->key[i]
                    = virtualNode[i];
            }

            // Update the newLeaf key to 
            // virtual Node 
            for (i = 0, j = cursor->size;
                i < newLeaf->size;
                i++, j++) {
                newLeaf->key[i]
                    = virtualNode[j];
            }

            // If cursor is the root node 
            if (cursor == root) {

                // Create a new Node 
                Node* newRoot = new Node;

                // Update rest field of 
                // B+ Tree Node 
                newRoot->key[0] = newLeaf->key[0];
                newRoot->ptr[0] = cursor;
                newRoot->ptr[1] = newLeaf;
                newRoot->IS_LEAF = false;
                newRoot->size = 1;
                root = newRoot;
            }
            else {

                // Recursive Call for 
                // insert in internal 
                insertInternal(newLeaf->key[0],
                    parent,
                    newLeaf);
            }
        }
    }
}

// Function to implement the Insert operation in B+ Tree 
void BPTree::insertSparse(int x){
    float order = MAX / 2; 
    // If root is null then return 
    // newly created node 
    if (root == NULL) {
        root = new Node;
        root->key[0] = x;
        root->IS_LEAF = true;
        root->size = 1;
    } else {
        Node* cursor = root;
        Node* parent = NULL;

        // Till cursor reaches the 
        // leaf node 
        while (cursor->IS_LEAF == false) {

            parent = cursor;

            for (int i = 0;
                i < cursor->size;
                i++) {

                // If found the position 
                // where we have to insert 
                // node 
                if (x < cursor->key[i]) {
                    cursor
                        = cursor->ptr[i];
                    break;
                }

                // If reaches the end 
                if (i == cursor->size - 1) {
                    cursor
                        = cursor->ptr[i + 1];
                    break;
                }
            }
        }

        if (cursor->size < floor(order)) {
            int i = 0;
            while (x > cursor->key[i]
                && i < cursor->size) {
                i++;
            }

            for (int j = cursor->size;
                j > i; j--) {
                cursor->key[j]
                    = cursor->key[j - 1];
            }

            cursor->key[i] = x;
            cursor->size++;

            cursor->ptr[cursor->size]
                = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size - 1] = NULL;
        }
        else {

            // Create a newLeaf node 
            Node* newLeaf = new Node;

            int* virtualNode = new int[floor(order) + 1];

            // Update cursor to virtual 
            // node created 
            for (int i = 0; i < floor(order); i++) {
                virtualNode[i]
                    = cursor->key[i];
            }
            int i = 0, j;

            // Traverse to find where the new 
            // node is to be inserted 
            while (x > virtualNode[i] && i < floor(order)) {
                i++;
            }

            // Update the current virtual 
            // Node to its previous 
            for (int j = floor(order) + 1;
                j > i; j--) {
                virtualNode[j] = virtualNode[j - 1];
            }

            virtualNode[i] = x;
            newLeaf->IS_LEAF = true;

            cursor->size = (floor(order) + 1) / 2;
            newLeaf->size
                = floor(order) + 1 - (floor(order) + 1) / 2;

            cursor->ptr[cursor->size]
                = newLeaf;

            newLeaf->ptr[newLeaf->size]
                = cursor->ptr[int(order)];

            cursor->ptr[int(order)] = NULL;

            // Update the current virtual 
            // Node's key to its previous 
            for (i = 0;
                i < cursor->size; i++) {
                cursor->key[i]
                    = virtualNode[i];
            }

            // Update the newLeaf key to 
            // virtual Node 
            for (i = 0, j = cursor->size;
                i < newLeaf->size;
                i++, j++) {
                newLeaf->key[i]
                    = virtualNode[j];
            }

            // If cursor is the root node 
            if (cursor == root) {

                // Create a new Node 
                Node* newRoot = new Node;

                // Update rest field of 
                // B+ Tree Node 
                newRoot->key[0] = newLeaf->key[0];
                newRoot->ptr[0] = cursor;
                newRoot->ptr[1] = newLeaf;
                newRoot->IS_LEAF = false;
                newRoot->size = 1;
                root = newRoot;
            }
            else {

                // Recursive Call for 
                // insert in internal 
                insertInternalSparse(newLeaf->key[0],
                    parent,
                    newLeaf);
            }
        }
    }
}

// Function to implement the Insert 
// Internal Operation in B+ Tree 
void BPTree::insertInternal(int x, Node* cursor, Node* child)
{

    // If we doesn't have overflow 
    if (cursor->size < MAX) {
        int i = 0;

        // Traverse the child node 
        // for current cursor node 
        while (x > cursor->key[i]
            && i < cursor->size) {
            i++;
        }

        // Traverse the cursor node 
        // and update the current key 
        // to its previous node key 
        for (int j = cursor->size;
            j > i; j--) {

            cursor->key[j]
                = cursor->key[j - 1];
        }

        // Traverse the cursor node 
        // and update the current ptr 
        // to its previous node ptr 
        for (int j = cursor->size + 1;
            j > i + 1; j--) {
            cursor->ptr[j]
                = cursor->ptr[j - 1];
        }

        cursor->key[i] = x;
        cursor->size++;
        cursor->ptr[i + 1] = child;
    }

    // For overflow, break the node 
    else {

        // For new Interval 
        Node* newInternal = new Node;
        int virtualKey[MAX + 1];
        Node* virtualPtr[MAX + 2];

        // Insert the current list key 
        // of cursor node to virtualKey 
        for (int i = 0; i < MAX; i++) {
            virtualKey[i] = cursor->key[i];
        }

        // Insert the current list ptr 
        // of cursor node to virtualPtr 
        for (int i = 0; i < MAX + 1; i++) {
            virtualPtr[i] = cursor->ptr[i];
        }

        int i = 0, j;

        // Traverse to find where the new 
        // node is to be inserted 
        while (x > virtualKey[i]
            && i < MAX) {
            i++;
        }

        // Traverse the virtualKey node 
        // and update the current key 
        // to its previous node key 
        for (int j = MAX + 1;
            j > i; j--) {

            virtualKey[j]
                = virtualKey[j - 1];
        }

        virtualKey[i] = x;

        // Traverse the virtualKey node 
        // and update the current ptr 
        // to its previous node ptr 
        for (int j = MAX + 2;
            j > i + 1; j--) {
            virtualPtr[j]
                = virtualPtr[j - 1];
        }

        virtualPtr[i + 1] = child;
        newInternal->IS_LEAF = false;

        cursor->size
            = (MAX + 1) / 2;

        newInternal->size
            = MAX - (MAX + 1) / 2;

        // Insert new node as an 
        // internal node 
        for (i = 0, j = cursor->size + 1;
            i < newInternal->size;
            i++, j++) {

            newInternal->key[i] = virtualKey[j];
        }

        for (i = 0, j = cursor->size + 1;
            i < newInternal->size + 1;
            i++, j++) {

            newInternal->ptr[i]
                = virtualPtr[j];
        }

        // If cursor is the root node 
        if (cursor == root) {

            // Create a new root node 
            Node* newRoot = new Node;

            // Update key value 
            newRoot->key[0]
                = cursor->key[cursor->size];

            // Update rest field of 
            // B+ Tree Node 
            newRoot->ptr[0] = cursor;
            newRoot->ptr[1] = newInternal;
            newRoot->IS_LEAF = false;
            newRoot->size = 1;
            root = newRoot;
        }

        else {

            // Recursive Call to insert 
            // the data 
            insertInternal(cursor->key[cursor->size],
                findParent(root,
                    cursor),
                newInternal);
        }
    }
}

void BPTree::insertInternalSparse(int x, Node* cursor, Node* child){
    float order = MAX / 2;
    // If we doesn't have overflow 
    if (cursor->size < floor(order)) {
        int i = 0;

        // Traverse the child node 
        // for current cursor node 
        while (x > cursor->key[i]
            && i < cursor->size) {
            i++;
        }

        // Traverse the cursor node 
        // and update the current key 
        // to its previous node key 
        for (int j = cursor->size; j > i; j--) {
            cursor->key[j] = cursor->key[j - 1];
        }

        // Traverse the cursor node 
        // and update the current ptr 
        // to its previous node ptr 
        for (int j = cursor->size + 1; j > i + 1; j--) {
            cursor->ptr[j] = cursor->ptr[j - 1];
        }

        cursor->key[i] = x;
        cursor->size++;
        cursor->ptr[i + 1] = child;
    }

    // For overflow, break the node 
    else {

        // For new Interval 
        Node* newInternal = new Node();
        int virtualKey[MAXsparse + 1];
        Node* virtualPtr[MAXsparse + 2];

        // Insert the current list key 
        // of cursor node to virtualKey 
        for (int i = 0; i < floor(order); i++) {
            virtualKey[i] = cursor->key[i];
        }

        // Insert the current list ptr 
        // of cursor node to virtualPtr 
        for (int i = 0; i < floor(order) + 1; i++) {
            virtualPtr[i] = cursor->ptr[i];
        }

        int i = 0, j;

        // Traverse to find where the new 
        // node is to be inserted 
        while (x > virtualKey[i] && i < floor(order)) {
            i++;
        }

        // Traverse the virtualKey node 
        // and update the current key 
        // to its previous node key 
        for (int j = floor(order) + 1;j > i; j--) {

            virtualKey[j]= virtualKey[j - 1];
        }

        virtualKey[i] = x;

        // Traverse the virtualKey node 
        // and update the current ptr 
        // to its previous node ptr 
        for (int j = floor(order) + 2;j > i + 1; j--) {
            virtualPtr[j]= virtualPtr[j - 1];
        }

        virtualPtr[i + 1] = child;
        newInternal->IS_LEAF = false;

        cursor->size
            = (floor(order) + 1) / 2;

        newInternal->size = floor(order) - (floor(order) + 1) / 2;

        // Insert new node as an 
        // internal node 
        for (i = 0, j = cursor->size + 1;i < newInternal->size;i++, j++) {

            newInternal->key[i]
                = virtualKey[j];
        }

        for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
            if (virtualPtr != NULL)
                newInternal->ptr[i] = virtualPtr[j];
        }

        // If cursor is the root node 
        if (cursor == root) {

            // Create a new root node 
            Node* newRoot = new Node;

            // Update key value 
            newRoot->key[0]
                = cursor->key[cursor->size];

            // Update rest field of 
            // B+ Tree Node 
            newRoot->ptr[0] = cursor;
            newRoot->ptr[1] = newInternal;
            newRoot->IS_LEAF = false;
            newRoot->size = 1;
            root = newRoot;
        }

        else {

            // Recursive Call to insert 
            // the data 
            insertInternalSparse(cursor->key[cursor->size],
                findParent(root,
                    cursor),
                newInternal);
        }
    }
}
// Function to find the parent node 
Node* BPTree::findParent(Node* cursor, Node* child)
{
    Node* parent = NULL;

    // If cursor reaches the end of Tree 
    if (cursor->IS_LEAF
        || (cursor->ptr[0])->IS_LEAF) {
        return NULL;
    }

    // Traverse the current node with 
    // all its child 
    for (int i = 0;
        i < cursor->size + 1; i++) {

        // Update the parent for the 
        // child Node 
        if (cursor->ptr[i] == child) {
            parent = cursor;
            return parent;
        }

        // Else recursively traverse to 
        // find child node 
        else {
            parent
                = findParent(cursor->ptr[i],
                    child);

            // If parent is found, then 
            // return that parent node 
            if (parent != NULL)
                return parent;
        }
    }

    // Return parent node 
    return parent;
}


// Function to get the root Node 
Node* BPTree::getRoot()
{
    return root;
}

void generateData() {
    srand(time(NULL)); //always seed your RNG before using it
    //generate random numbers:
    for (int i = 0;i < AMOUNT+10;i++)
    {
        bool check; //variable to check or number is already used
        int n; //variable to store the number in
        do
        {
            n = rand() % 100000 + 100000;
            //check or number is already used:
            check = true;
            for (int j = 0;j < i;j++)
                if (n == value[j]) //if number is already used
                {
                    check = false; //set check to false
                    break; //no need to check the other elements of value[]
                }
        } while (!check); //loop until new, unique number is found
        value[i] = n; //store the generated number in the array
        //cout << value[i] << ' ';
    }
}

void partC1(BPTree a) {
    int c = rand() % 100000 + 100000;
    a.insert(c);
    cout << "Inserting Random Value ";
    Node c1 = a.search(c);
    cout << "\n";
    cout << "Node before insert:\n";
    c1.beforeInsert(c);
    cout << "Node after insert:\n";
    c1.display();
    cout << "\n";
}

void partC2(BPTree a) {
    int index = rand() % AMOUNT; //index for value array 0-9999
    //cout << "index = " << index << "\n";
    int deletion = value[index];
    //cout << "c = " << deletion << "\n";
    cout << "Randomly Selected Number: ";
    Node c2 = a.search(deletion);
    cout << "\n";
    a.deleteKey(a,deletion, c2);
    cout << "\n";
}

void partC3(BPTree a) {
    int choice = rand() % 2;
    if (choice == 1) { //perform insertion 
        cout << "Perform Insertion\n";
        partC1(a);
    } else { //perform deletion
        cout << "Perform Deletion\n";
        partC2(a);
    }
}

void partC4(BPTree a) {
    int choice = rand() % 2;
    if (choice == 1) { //perform insertion 
        int look = rand() % 100000 + 100000;
        cout << "Perform Search for " << look << "\n";
        a.search(look);
        if (isPresent == true)
            cout << " FOUND\n";
        else
            cout << " NOT FOUND\n";
        cout << "\n";
    }
    else { //perform deletion
        int minIndex = rand() % 5000;
        int lookMin = value[minIndex];
        int lookMax = value[minIndex] + rand() % 99 + 1;
        cout << "Perform Range Search between " << lookMin << " and " << lookMax << "\n";
        a.rangeSearch(lookMin, lookMax);
        cout << "\n";
    }
}

// Driver Code 
int main()
{
    BPTree dense;
    BPTree sparse; 
    // Create B+ Tree

    generateData();
    cout << "!--------------------Part 1: Generate 10,000 Records = COMPLETE ------------------------------------------------------------------------!\n\n";

    // populate B trees
    for (int i = 0; i < AMOUNT; i++) {
        //cout << value[i] << " ";
        dense.insert(value[i]);
        sparse.insertSparse(value[i]);
    }
    cout << "!--------------------Part 2: Build Dense and Sparse B+ tree of order " << MAX << " = COMPLETE------------------------------------------------------!\n\n";
 
    cout << "!--------------------Part 3c1: Apply 2 Randomly Generated Insertion Operations on Dense Tree--------------------------------------------!\n";
    partC1(dense);
    partC1(dense);
    
    cout << "!--------------------Part 3c2: Apply 2 Randomly Generated Deletion Operations on Sparse Tree--------------------------------------------!\n";    
    partC2(sparse);
    partC2(sparse);
    cout << "\n";
    cout << "!--------------------Part 3c3: Apply 2 Randomly Generated Insertion OR Deletion Operations on both trees:-------------------------------!\n";
    //5 random operations on dense tree
    cout << "1ST DENSE OPERATION: ";
    partC3(dense);
    cout << "2ND DENSE OPERATION: ";
    partC3(dense);
    cout << "3RD DENSE OPERATION: ";
    partC3(dense);
    cout << "4TH DENSE OPERATION: ";
    partC3(dense);
    cout << "5TH DENSE OPERATION: ";
    partC3(dense);
    
    //5 random operations on sparse tree 
    cout << "---------------------\n1ST SPARSE OPERATION: ";
    partC3(sparse);
    cout << "2ND SPARSE OPERATION: ";
    partC3(sparse);
    cout << "3RD SPARSE OPERATION: ";
    partC3(sparse);
    cout << "4TH SPARSE OPERATION: ";
    partC3(sparse);
    cout << "5TH SPARSE OPERATION: ";
    partC3(sparse);


    cout << "!--------------------Part 3c4: Apply 2 Randomly Generated Search OR Range Search Operations on both trees:------------------------------!\n";
    //5 random operations on dense tree
    cout << "1ST DENSE SEARCH OPERATION: ";
    partC4(dense);
    cout << "2ND DENSE SEARCH OPERATION: ";
    partC4(dense);
    cout << "3RD DENSE SEARCH OPERATION: ";
    partC4(dense);
    cout << "4TH DENSE SEARCH OPERATION: ";
    partC4(dense);
    cout << "5TH DENSE SEARCH OPERATION: ";
    partC4(dense);

    //5 random operations on sparse tree 
    cout << "---------------------\n1ST SPARSE SEARCH OPERATION: ";
    partC4(sparse);
    cout << "2ND SPARSE SEARCH OPERATION: ";
    partC4(sparse);
    cout << "3RD SPARSE SEARCH OPERATION: ";
    partC4(sparse);
    cout << "4TH SPARSE SEARCH OPERATION: ";
    partC4(sparse);
    cout << "5TH SPARSE SEARCH OPERATION: ";
    partC4(sparse);

    return 0;
}




