#ifndef BTREENODE_H
#define BTREENODE_H
#include <iostream>

using namespace std;

class BTreeNode {
    int *keys;//Arreglo de Keys
    int t;//Degree minimo
    BTreeNode **C;//Hijos
    int n;//N�mero de Keys
    bool leaf;//Plain af
public:
    BTreeNode(int _t, bool _leaf);
    void insertNonFull(int k);
    void splitChild(int i, BTreeNode *y);
    void traverse();
    BTreeNode* search(int k);
    //eliminar
    int findKey(int k);
    void remove(int k);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    int getPred(int idx);
    int getSucc(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
    friend class BTree;
};

#endif // BTREENODE_H
