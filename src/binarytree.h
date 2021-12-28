#include "linkedlist.h"
#include <queue>
#ifndef BINARYTREE_H_INCLUDED
#define BINARYTREE_H_INCLUDED

using namespace std;

class slicingtree;

class treenode
{
public:
    treenode(int moc, double w, double h);

private:
    int module;
    double width, height, area;
    double down_left_corner_x, down_left_corner_y;
    treenode *left;
    treenode *right;
    treenode *parent;

    friend class slicingtree;
};

class slicingtree
{
public:
     slicingtree():root(0){};
     void InitialConstruct(vector<int> block, vector<double> wh);
     void move1(int module1, int module2, Linkedlist table);
     void move2(int cut_num, Linkedlist table);
     void move3(int cut_num, int module, int LorR, Linkedlist table);
     void clone(slicingtree &to_copy);
     void printNPE(int module_number, char *argv);
     void get_axis();
     double compare(double num1, double num2);
     double cost();
     queue<treenode*> axis_prepare();

private:
    queue<treenode*> Postorder();
    void copy(treenode *&new_root, treenode *root);
    void connect_parent(treenode *current);
    treenode *Search(int module);
    treenode *Search_cut(int cut_num);
    treenode *leftmost();
    treenode *rightmost();
    treenode *root;

};


#endif // BINARYTREE_H_INCLUDED
