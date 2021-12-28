#include <iostream>
#include <iomanip>
#include <vector>
#include "./src/linkedlist.h"

using namespace std;

Listnode::Listnode(int m, vector<double> w, vector<double> h)
{
    width = w;
    height = h;
    module = m;
    next = 0;
}

double Linkedlist::avg()
{
    int t = 0;
    double delta_avg = 0;

    Listnode *current = first;

    while (current->next != 0)
    {
        if(current->width[0]*current->height[0] > current->next->width[0]*current->next->height[0])
            delta_avg = (current->width[0]*current->height[0]) - (current->next->width[0]*current->next->height[0]) + delta_avg;

        else
            delta_avg = -(current->width[0]*current->height[0]) + (current->next->width[0]*current->next->height[0]) + delta_avg;

        current = current->next;
        t++;
    }

    delta_avg = delta_avg/t;

    return delta_avg;

}


void Linkedlist::Printlist()
{

    if (first == 0)
    {
        cout << "List is empty.\n";
        return;
    }

    Listnode *current = first;

    while (current != 0)
    {

        cout << current->module << ":" << endl;

        for(int i = 0; i < current->width.size(); i++)
        {
            cout << fixed << setprecision(2) << current->width[i] << "X" << current->height[i] << endl;
        }

        current = current->next;

    }

}

void Linkedlist::Push_back(int m, vector<double> w, vector<double> h)
{

    Listnode *newNode = new Listnode(m, w, h);


    if (first == 0)
    {
        first = newNode;
        cur = first;
        return;
    }

    cur->next = newNode;
    cur = cur->next;

}

Listnode *Linkedlist::get_shape(int m)
{
    Listnode *current = first;

    while(current != 0)
    {
        if(m == current->module)
            return current;

        current = current->next;
    }

    return 0;
}
