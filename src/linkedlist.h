#include <vector>
#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

//Linked-list

using namespace std;

class Linkedlist;

class Listnode
{
public:
    Listnode(int m, vector<double> w, vector<double> h);

private:
    vector<double> width;
    vector<double> height;
    int module;
    Listnode *next;

    friend class Linkedlist;
    friend class slicingtree;
};

class Linkedlist
{
public:
     Linkedlist():first(0),cur(0){};
     void Printlist();
     void Push_back(int m, vector<double> w, vector<double> h);
     double avg();
     Listnode *get_shape(int m);

private:
    Listnode *first;
    Listnode *cur;

};

#endif // LINKEDLIST_H_INCLUDED
