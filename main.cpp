#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <queue>
#include <algorithm>
#include <unistd.h>
#include "./src/binarytree.h"

using namespace std;

/**
This program is a slicing floorplan algorithm
Input soft module, get normalized polish expression
**/

vector<int> parser(slicingtree &, Linkedlist &, char *);//parser input file
void intial_wh(int area, vector<double> &);//get initial solution
void get_all_shape(int area, int name, Linkedlist &);//get shape curve
void SA (vector<int> &, slicingtree &, Linkedlist);//SA-based floorplan algorithm

int main(int argc, char *argv[])
{
    slicingtree st;
    Linkedlist module_table;
    vector<int> pe;

    pe = parser(st, module_table, argv[1]);

    SA(pe, st, module_table);

    st.printNPE(pe.size()/2+1, argv[2]);


    return 0;
}

vector<int> parser(slicingtree &st, Linkedlist &m_table, char *argv)
{
    ifstream infile (argv, ios::in);
    vector<int> module_name;
    vector<double> module_wh;
    int module_number;

    infile >> module_number;

    for(int i=0; i<module_number; i++)
    {
        int name, area;

        infile >> name >> area;
        module_name.push_back(name);

        intial_wh(area, module_wh);
        get_all_shape(area, name, m_table);
    }

    for(int i=2; i<=module_name.size(); i++)
    {
        module_name.insert(module_name.begin()+i,-1);
        i++;
    }

    st.InitialConstruct(module_name, module_wh);
    return module_name;
}

void intial_wh(int area, vector<double> &wh)
{
    double mid = sqrt(area);
    double width, height;

    width = mid;

    height = area/width;
    wh.push_back(width);
    wh.push_back(height);

}

void get_all_shape(int area, int name, Linkedlist &m_table)
{
    vector<double> width, height;

    double as_ratio[4] = {0.8, 1, 1.5, 1.7};

    for(int i = 0; i < 4; i++)
    {
        double w = sqrt(area*as_ratio[i]);

        double h = area/w;

        width.push_back(w);
        height.push_back(h);

    }

    m_table.Push_back(name, width, height);
}

void SA (vector<int> &pe, slicingtree &st, Linkedlist m_table)
{
    slicingtree nst;
    st.clone(nst);

    clock_t start, ending;

    vector<int> npe = pe;

    int p = pe.size()/2 + 1;
    double temperature = m_table.avg()*log(exp(1))/log(p);
    double final_temperature = 100;

    int reject = 0;
    int uphill = 0;
    int move_time = uphill;
    int itr;

    int n = 10*p;

    start = clock();

    do
    {
        move_time = 0;
        uphill = 0;
        reject = 0;

        cout << "T = " << temperature << endl;

        int cut_location, selectmove, cut_num;


        do
        {
            srand(time(NULL));

            if(move_time <= 3*p)
                selectmove = rand()%2+1;
            else
                selectmove = rand()%3+1;

            itr = 0;

            if(selectmove == 1)
            {
                int module1 = rand()%p;
                int module2 = rand()%p;

                while(module1 == module2)
                    module2 = rand()%p;

                int two_module[2] = {module1, module2};
                int index[2];
                vector<int>::iterator itr;

                for(int i = 0; i < 2; i++)
                {
                    itr = find(npe.begin(), npe.end(), two_module[i]);

                    if (itr != npe.cend())
                    {
                        index[i] = distance(npe.begin(), itr);
                    }
                    else
                    {
                        cout << "Element not found";
                    }
                }

                nst.move1(module1, module2, m_table);
                swap(npe[index[0]], npe[index[1]]);

            }

            else if(selectmove == 2)
            {
                int upper = p-1;

                cut_num = rand()%upper+1;
                int counter = 0;

                nst.move2(cut_num, m_table);

                for(int i = npe.size()-1 ; i >= 0; i--)
                {
                    if(npe[i] == -1 || npe[i] == -2)
                    {
                        counter++;

                        if(counter == cut_num)
                        {

                            if(npe[i] == -1)
                                npe[i] = -2;
                            else
                                npe[i] = -1;

                            while(true)
                            {
                                int up_chain = 1;
                                int down_chain = 1;


                                if(npe[i+up_chain] != -1 && npe[i+up_chain] != -2 && npe[i-down_chain] != -1 && npe[i-down_chain] != -2)
                                    break;

                                else
                                {
                                    if(npe[i+up_chain] == -1 || npe[i+up_chain] == -2)
                                    {
                                        nst.move2(cut_num-up_chain, m_table);
                                        if(npe[i+up_chain] == -1)
                                            npe[i+up_chain] = -2;
                                        else
                                            npe[i+up_chain] = -1;

                                        up_chain++;
                                    }

                                    if(npe[i-down_chain] == -1 || npe[i-down_chain] == -2)
                                    {
                                        nst.move2(cut_num+down_chain, m_table);
                                        if(npe[i-down_chain] == -1)
                                            npe[i-down_chain] = -2;
                                        else
                                            npe[i-down_chain] = -1;

                                        down_chain++;
                                    }
                                }
                            }

                            break;
                        }

                    }
                }

            }

            else if(selectmove == 3)
            {
                int done = 0;
                int cut, operand, previous_cut;
                int upper = p-2;

                while(done == 0)
                {

                    cut = rand()%upper+2;
                    itr++;


                    if(itr > 2*(p-1))
                        break;

                    int counter = 0;

                    for(int i = npe.size()-1 ; i >= 0; i--)
                    {
                        if(npe[i] == -1 || npe[i] == -2)
                        {
                            counter++;

                            if(counter == cut)
                            {
                                cut_location = i;
                                break;
                            }


                            if(counter == cut-1)
                            {
                                previous_cut = i;
                            }

                        }
                    }

                    if(npe[cut_location+1] != -1 && npe[cut_location+1] != -2 && npe[cut_location] != npe[cut_location+2])
                    {

                        swap(npe[cut_location], npe[cut_location+1]);

                        int operator_amount = 0;

                        for(int i = 0; i <= cut_location + 1; i++)
                        {
                            if(npe[i] == -1 || npe[i] == -2)
                                operator_amount++;
                        }

                        if(2*operator_amount < cut_location + 1)
                        {
                            done = 1;
                        }
                        else
                        {
                            done = 0;
                            npe = pe;
                        }

                    }
                    else
                    {
                        done = 0;
                        npe = pe;
                        continue;
                    }

                }

                if(itr < 2*(p-1))
                {
                    operand = npe[cut_location];

                    if(previous_cut-cut_location-1 == 1)
                        nst.move3(cut, operand, 1, m_table);

                    else if(previous_cut-cut_location-1 == 2)
                        nst.move3(cut, operand, 2, m_table);

                 }

            }

            if(itr < 2*(p-1))
                move_time++;

            double delta_cost = nst.cost() - st.cost();

            if(delta_cost <= 0 || (double)(rand()&100+1)/100 < exp(-1*delta_cost/temperature))
            {
                if(delta_cost > 0)
                {
                    uphill++;
                }

                nst.clone(st);
                pe = npe;

            }
            else
            {
                reject++;
                npe = pe;
                st.clone(nst);
            }

        }
        while(uphill < n && move_time < 2*n);

        temperature = temperature*0.85;

        ending = clock();

    }
    while(reject/move_time < 0.95 && temperature > final_temperature && (ending-start)/CLOCKS_PER_SEC < 3600);

}
