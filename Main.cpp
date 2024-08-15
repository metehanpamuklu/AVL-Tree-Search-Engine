// Created by Metehan Pamuklu
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
string not_exist = "not here";
struct DocumentItem {
    string documentName;
    int count;
};
struct WordItem {
    string word;
    vector<DocumentItem> documentList;
};
template <class Key, class Value>
struct AvlNode
{
    Key key;
    Value value;
    AvlNode* left;
    AvlNode* right;
    int        		height;

    AvlNode(const Key& theKey, const Value& theValue,
        AvlNode* lt, AvlNode* rt, int h = 0)
        : key(theKey), value(theValue), left(lt), right(rt), height(h) { }

};
//all needed structs for avl tree

//functions to manipulate string for AVlsearchtree
bool isAlphaphtic(const string str) {
    for (char c : str) {
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            return false;
        }

    }
    return true;
}
string toLower(string& str) {
    string res = "";
    for (char c : str) {
        res += tolower(c);

    }
    return res;
}

//from this line all of the functions taken from lecture
template <class Key, class Value>
class AVLSearchTree {
private:
    const Key &elementAt(AvlNode<Key,Value>* t) const;
    AvlNode<Key, Value>* root;
    void insert(const Key& x,const Value &y, AvlNode<Key,Value>*& t) const;
    void remove(const Key& x, AvlNode<Key,Value>*& t) const;
    AvlNode<Key,Value>* findMin(AvlNode<Key, Value>* t) const;
    AvlNode<Key, Value>* findMax(AvlNode<Key, Value>* t) const;
    AvlNode<Key, Value>* find(const Key& x, AvlNode<Key, Value>* t) const;
    void makeEmpty(AvlNode<Key, Value>*& t) const;
    // Avl manipulations
    int height(AvlNode<Key, Value>* t) const;
    int max(int lhs, int rhs) const;
    void rotateWithLeftChild(AvlNode<Key, Value>*& k2) const;
    void rotateWithRightChild(AvlNode<Key, Value>*& k1) const;
    void doubleWithLeftChild(AvlNode<Key, Value>*& k3) const;
    void doubleWithRightChild(AvlNode<Key, Value>*& k1) const;
public:
    // Constructor
    AVLSearchTree() : root(nullptr) {}
    AVLSearchTree(const AVLSearchTree& rhs);
    ~AVLSearchTree();
    const Key& findMin() const;
    const Key& findMax() const;
    const Key& find(const Key& x) const;
    bool isEmpty() const;
    void makeEmpty();
    void insert(const Key &x,const Value & y);
    void remove(const Key& x);
    Value getValue(const Key& x);
};

template <class Key,class Value>
int AVLSearchTree<Key,Value>::height(AvlNode<Key,Value>* t)const
{
    if (t == NULL)
        return -1;
    return t->height;
}
template <class Key, class Value>
int AVLSearchTree<Key,Value>::max(int lhs, int rhs) const
{
    if (lhs > rhs)
        return lhs;

    return rhs;
}
template <class Key, class Value>
const Key& AVLSearchTree<Key, Value>::elementAt(AvlNode<Key, Value>* t) const
{
    if (t != NULL)
    {
        return t->key;
    }
    return not_exist;
}
template <class Key,class Value>
void AVLSearchTree<Key,Value>::insert(const Key& x,const Value& y, AvlNode<Key,Value>*& t) const
{
    if (t == NULL)
        t = new AvlNode<Key,Value>(x,y, NULL, NULL);

    else if (x < t->key) {
        // X should be inserted to the left tree!
        insert(x,y, t->left);

        // Check if the left tree is out of balance (left subtree grew in height!)
        if (height(t->left) - height(t->right) == 2)
            if (x < t->left->key)  // X was inserted to the left-left subtree!
                rotateWithLeftChild(t);
            else			     // X was inserted to the left-right subtree!
                doubleWithLeftChild(t);
    }
    else if (t->key < x)
    {    // Otherwise X is inserted to the right subtree
        insert(x,y, t->right);
        if (height(t->right) - height(t->left) == 2)
            // height of the right subtree increased
            if (t->right->key < x)
                // X was inserted to right-right subtree
                rotateWithRightChild(t);
            else // X was inserted to right-left subtree
                doubleWithRightChild(t);
    }
    else
        ;  // Duplicate; do nothing

  // update the height the node
    t->height = max(height(t->left), height(t->right)) + 1;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithLeftChild(AvlNode<Key, Value>*& k2) const
{
    AvlNode<Key, Value>* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithRightChild(AvlNode<Key, Value>*& k1) const
{
    AvlNode<Key, Value>* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithLeftChild(AvlNode<Key, Value>*& k3) const
{
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithRightChild(AvlNode<Key, Value>*& k1) const
{
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}

template <class Key, class Value>
const Key& AVLSearchTree<Key, Value>::find(const Key& x) const
{
    return elementAt(find(x, root));
}

template <class Key, class Value>
const Key& AVLSearchTree<Key, Value>::findMin() const
{
    return elementAt(findMin(root));
}

template <class Key, class Value>
AvlNode<Key, Value>* AVLSearchTree<Key, Value>::findMin(AvlNode<Key, Value>* t) const
{
    if (t == NULL)
        return NULL;
    if (t->left == NULL)
        return t;
    return findMin(t->left);
}

template <class Key, class Value>
const Key& AVLSearchTree<Key, Value>::findMax() const
{
    return elementAt(findMax(root));
}

template <class Key, class Value>
AvlNode<Key, Value>* AVLSearchTree<Key, Value>::findMax(AvlNode<Key, Value>* t) const
{
    if (t != NULL)
        while (t->right != NULL)
            t = t->right;
    return t;

}

template <class Key, class Value>
AvlNode<Key, Value>* AVLSearchTree<Key, Value>::find(const Key& x, AvlNode<Key, Value>* t) const
{
    if (t == NULL)
        return NULL;
    else if (x < t->key)
        return find(x, t->left);
    else if (t->key < x)
        return find(x, t->right);
    else
        return t;    // Match
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key& x, const Value& y)
{
    insert(x, y, root);
}
template <class Key, class Value>
bool AVLSearchTree<Key, Value>::isEmpty() const
{
    return root == NULL;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty()
{
    makeEmpty(root);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty(AvlNode<Key, Value>*& t) const
{
    if (t != NULL)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

template <class Key, class Value>
AVLSearchTree<Key, Value>::~AVLSearchTree()
{
    makeEmpty();
}

template <class Key, class Value>
Value AVLSearchTree<Key, Value>::getValue(const Key& x)
{
    AvlNode<Key, Value>* result = find(x, root);
    if (result != nullptr) {
        return result->value; // Return the value associated with the key
    }
    else {
        return nullptr; // Key not found, return nullptr or throw an exception
    }
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key& x)
{
    remove(x, root);
}
template<class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key& x, AvlNode<Key, Value>*& t)const{
    if (t == NULL)
        return;   // Item not found ; do nothing
    if (x < t->key) {
        remove(x, t->left);
    }
    else if (t->key < x) {
        remove(x, t->right);
    }
    else if (t->left != NULL && t->right != NULL) // Two children
    {
        t->key = findMax(t->left)->key;
        t->value = findMax(t->left)->value;
        remove(t->key, t->left);
    }
    else // one or no children
    {
        AvlNode<Key, Value>* oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
    }
    if (t != NULL) {//just like adding a  new node
        t->height = max(height(t->left), height(t->right)) + 1;
        if (height(t->left) - height(t->right) == 2) {//deletin from right "probably no need because we always deleting from left side 
            if (t->left == NULL)
            {
                rotateWithLeftChild(t);
            }
            else if (x > t->right->key)//right of left
            {
                doubleWithLeftChild(t);
            }
            else //left of left
            {
                rotateWithLeftChild(t);
            }
        }
        if (height(t->right) - height(t->left) == 2) {//deletion from left
           if (t->left == NULL)
           {
             rotateWithRightChild(t);
           }
           else if (x > t->left->key)//right of left
           {
           rotateWithRightChild(t);
           }
           else //left of left
           {
            doubleWithRightChild(t);
           }
        }
    }
}

bool checking_for_tree(vector<string>& query, AVLSearchTree<string, WordItem*>& avlTree) {
    for (const string& query : query)
    {
        if (avlTree.find(query) == "not here")
        {
            return false;
        }
    }
    return true;
}
bool checking_for_docs(vector<string>& queries, const string& doc_name, AVLSearchTree<string, WordItem*>& myTree) {
    for (const string& query : queries) {
        WordItem* wordItem = myTree.getValue(query);
        if (wordItem != nullptr) {
            bool found = false;
            for (const DocumentItem& doc : wordItem->documentList) {
                if (doc.documentName == doc_name) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return true;
}

int main() {
    AVLSearchTree<string, WordItem*> AvlTree;//creating a AvlsearchTree   
    int file_count;
    vector<string> file_names;//hold them for printing
    cout << "Enter number of input files: ";
    cin >> file_count;
    for (int i = 0; i < file_count; i++) {        
        string file;
        cout << "Enter " << i + 1 << ". " << "file name: ";
        cin >> file;
        file_names.push_back(file);
        ifstream file1(file);
        if (!file1.is_open()) {
            cout << "Failed to open" << file<< endl;
        }
        string words;//key for worditem
        DocumentItem doc_1;//struct of document vector in word item
        WordItem* word_1;
        while (file1 >> words) {
            if (isAlphaphtic(words)) {
                words = toLower(words);//all check for string and make it lower case 
                if (AvlTree.find(words) == "not here") {//if word not exist
                    word_1 = new WordItem;
                    word_1->word = words;//adiding word key
                    doc_1.count = 1;
                    doc_1.documentName = file;//creating element for document vector
                    word_1->documentList.push_back(doc_1);
                    AvlTree.insert(words, word_1);//adding to avltree
                }
                else { 
                    WordItem* wordItem = AvlTree.getValue(words);//reaching value of avltree  
                    bool foundDocument = false;
                    for (DocumentItem& document : wordItem->documentList) {//for loop for reaching document in pouter for loop to see if it is exist
                        if (document.documentName == file) {
                            document.count++;  
                            foundDocument = true;
                            break;
                        }
                    }
                    if (!foundDocument) {// if there is no document create a new document element vector  
                        DocumentItem doc;//new element created
                        doc.documentName = file;
                        doc.count = 1;
                        wordItem->documentList.push_back(doc);
                    }
                }
            }
        }
    }
    // queries finding words in avltree
    string quer;
    cout << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, quer); 
    while (quer != "ENDOFINPUT") {
        istringstream iss(quer);
        string word;
        vector<string> query_list;
        while (iss >> word) 
        {
            if (word != "REMOVE")
                word = toLower(word); 
            query_list.push_back(word);//making vector to find words in avltree 
        }
        if (query_list[0] != "REMOVE") {
            if (checking_for_tree(query_list, AvlTree)) {//to see if word exist in tree        
                for (const string& file : file_names) {//making a for loop to reach words document by document to print like wanted
                    if (checking_for_docs(query_list, file, AvlTree)) {//if word in document
                        cout << "in Document" << " " << file << ",";
                        for (const string& word : query_list) {//reaching every word in query 
                            {
                                WordItem* wordItem = AvlTree.getValue(word);
                                vector<DocumentItem> doc = wordItem->documentList;                          
                                int index = 0;
                                for (size_t i = 0; i < doc.size(); ++i) {
                                    if (doc[i].documentName == file) {
                                        index = i;//loop for finding correct index of in the document vector in worditem pointer in avltree
                                        break; 
                                    }
                                }
                                DocumentItem doc_1 = doc[index];
                                cout << word << " found " << doc_1.count << " times";
                                if (word != query_list[query_list.size() - 1]) {
                                    cout << ", ";
                                }
                            }
                        }
                        cout << endl;
                    }
                }
            }
            else {
                cout << "No document contains the given query" << endl;
            }
        }
        else {
            AvlTree.remove(query_list[1]); //removing the given word from the tree
            cout << query_list[1] << " has been REMOVED";
        }
        cout << endl;   
        cout << "Enter queried words in one line: ";
        getline(cin, quer); 
    }
    return 0;
}
