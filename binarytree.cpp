#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <fstream>
#include "./src/binarytree.h"
#include "./src/linkedlist.h"

using namespace std;

treenode::treenode(int moc, double w, double h)
{
    module = moc;
    width = w;
    height = h;
    area = width*height;
    down_left_corner_x = down_left_corner_y = 0;
    parent = left = right = 0;

}

void slicingtree::clone(slicingtree &to_copy)
{
    copy(to_copy.root,root);
    to_copy.connect_parent(to_copy.root);
}

void slicingtree::copy(treenode *&new_root, treenode *root)
{
    if(!root)
    {
        new_root = NULL;
        return;
    }
    new_root = new treenode(0, 0, 0);
    new_root->module = root->module;
    new_root->width = root->width;
    new_root->height = root->height;
    new_root->area = root->area;
    new_root->down_left_corner_x = root->down_left_corner_x;
    new_root->down_left_corner_y = root->down_left_corner_y;
    copy(new_root->left,root->left);
    copy(new_root->right,root->right);
    return;
}

void slicingtree::connect_parent(treenode *current)
{
    if(current)
    {
        if(current->left) current->left->parent = current;
        if(current->right) current->right->parent = current;

        connect_parent(current->left);
        connect_parent(current->right);
    }

}

double slicingtree::cost()
{
    get_axis();

    treenode *leftmost_module = leftmost();
    treenode *rightmost_module = rightmost();

    double delta_x = pow(rightmost_module->down_left_corner_x - leftmost_module->down_left_corner_x, 2);
    double delta_y = pow(rightmost_module->down_left_corner_y - leftmost_module->down_left_corner_y, 2);
    double length = sqrt(delta_x + delta_y);

    return root->area + 0.1*pow(length, 2);
}

treenode *slicingtree::leftmost()
{
    treenode *current = root;

    while(current->left != 0)
    {
        current = current->left;
    }

    return current;
}

treenode *slicingtree::rightmost()
{
    treenode *current = root;

    while(current->right != 0)
    {
        current = current->right;
    }

    return current;
}

void slicingtree::get_axis()
{
    queue<treenode*> postorder_cut, q;

    postorder_cut = axis_prepare();
    double counting = 0;

    while(!postorder_cut.empty())
    {
        treenode *current_cut = postorder_cut.front();
        postorder_cut.pop();

        int choose = 0;

        if(current_cut->left->module != -1 && current_cut->left->module != -2)
            choose++;

        if(current_cut->right->module != -1 && current_cut->right->module != -2)
            choose++;

        if(choose == 2)
        {
            if(current_cut->module == -1)
            {
                current_cut->right->down_left_corner_x = current_cut->left->down_left_corner_x + current_cut->left->width;
                counting++;
            }
            else
            {
                current_cut->right->down_left_corner_y = current_cut->left->down_left_corner_y + current_cut->left->height;
                counting++;
            }

        }
        else if(choose == 1)
        {

            if(current_cut->right->module != -1 && current_cut->right->module != -2)
            {
                if(current_cut->module == -1)
                {
                    current_cut->right->down_left_corner_x = current_cut->left->down_left_corner_x + current_cut->left->width;
                    current_cut->right->down_left_corner_y = current_cut->left->down_left_corner_y;
                    counting++;
                }
                else
                {
                    current_cut->right->down_left_corner_y = current_cut->left->down_left_corner_y + current_cut->left->height;
                    current_cut->right->down_left_corner_x = current_cut->left->down_left_corner_x;
                    counting++;
                }
            }

            else if(current_cut->left->module != -1 && current_cut->left->module != -2)
            {
                if(current_cut->module == -1)
                {
                    current_cut->left->down_left_corner_x = current_cut->right->down_left_corner_x - current_cut->left->width;
                    current_cut->left->down_left_corner_y = current_cut->right->down_left_corner_y;
                    counting--;
                }
                else
                {
                    current_cut->left->down_left_corner_y = current_cut->right->down_left_corner_y - current_cut->left->height;
                    current_cut->left->down_left_corner_x = current_cut->right->down_left_corner_x;
                    counting--;
                }
            }

        }
        else if(choose == 0)
        {
            int fix_x = 0;
            int fix_y = 0;

            if(current_cut->module == -1)
            {
                current_cut->right->down_left_corner_x = current_cut->left->down_left_corner_x + current_cut->left->width;
                fix_x = current_cut->left->width + 1;
                counting++;
            }
            else
            {
                current_cut->right->down_left_corner_y = current_cut->left->down_left_corner_y + current_cut->left->height;
                fix_y = current_cut->left->height + 1;
                counting++;
            }

            q.push(current_cut->right);

            int pass = 0;

            while (!q.empty())
            {
                treenode *current = q.front();
                pass++;
                q.pop();

                if(pass != 0)
                {
                    current->down_left_corner_x = current->down_left_corner_x + fix_x;
                    current->down_left_corner_y = current->down_left_corner_y + fix_y;
                }

                if (current->left != NULL)
                    q.push(current->left);

                if (current->right != NULL)
                    q.push(current->right);
            }

        }

        current_cut->down_left_corner_x = current_cut->left->down_left_corner_x;
        current_cut->down_left_corner_y = current_cut->left->down_left_corner_y;

    }

    root->width = root->right->width + root->right->down_left_corner_x;

}

queue<treenode*> slicingtree::axis_prepare()
{
    queue<treenode*> q;

    stack<treenode*> s;
    int check = 0;

    s.push(root);

    while(s.size() > 0)
    {
        treenode *current = s.top();

        if(current == root->right)
            check = 0;

        if(current->left == 0 && current->right == 0 || check != 0)
        {

            if(current->module == -1 || current->module == -2)
            {
                q.push(current);
            }

            check++;
            s.pop();
        }

        if(current->right != 0 && check == 0)
            s.push(current->right);

        if(current->left != 0 && check == 0)
            s.push(current->left);
    }

    return q;

}

void slicingtree::InitialConstruct(vector<int> block, vector<double> wh)
{

    root = new treenode(block[0], wh[0], wh[1]);
    wh.erase(wh.begin(), wh.begin()+2);
    block.erase(block.begin());

    queue<treenode*> q;
    treenode *current = root;

    while (!block.empty())
    {

        for(int take=0; take<2; take++)
        {
            if (take==0)
            {
                treenode *new_node = new treenode(block[take], wh[take], wh[take+1]);
                q.push(new_node);
            }
            else if (take==1)
            {
                treenode *new_node = new treenode(block[take], 0, 0);
                current->parent = new_node;
                new_node->left = current;

                current = q.front();
                current->parent = new_node;
                new_node->right = current;

                if(new_node->left->height >= new_node->right->height)
                    new_node->height = new_node->left->height;
                else
                    new_node->height = new_node->right->height;

                new_node->width = new_node->left->width + new_node->right->width;
                new_node->area = new_node->width*new_node->height;

                q.pop();
                q.push(new_node);
            }

        }

        block.erase(block.begin(), block.begin()+2);
        wh.erase(wh.begin(), wh.begin()+2);

        current = q.front();
        q.pop();
    }

    root = current;
}

queue<treenode*> slicingtree::Postorder()
{

    queue<treenode*> q;

    stack<treenode*> s;
    int check = 0;

    s.push(root);

    while(s.size() > 0)
    {
        treenode *current = s.top();

        if(current == root->right)
            check = 0;

        if(current->left == 0 && current->right == 0 || check != 0)
        {
            q.push(current);

            check++;
            s.pop();
        }

        if(current->right != 0 && check == 0)
            s.push(current->right);

        if(current->left != 0 && check == 0)
            s.push(current->left);
    }

    return q;

}

void slicingtree::printNPE(int module_number, char *argv)
{
    ofstream outfile (argv, ios::out);

    queue<treenode*> q;

    outfile << (floor(root->width*100))/100 << " " << (floor(root->height*100))/100 << " " << (floor(root->area*100))/100<< endl;

    for(int i = 0; i < module_number; i++)
    {
        treenode *current = Search(i);
        outfile << (floor(current->down_left_corner_x*100))/100 << " " << (floor(current->down_left_corner_y*100))/100 << " " << (floor(current->width*100))/100 << " " << (floor(current->height*100))/100 << endl;
    }

    q = Postorder();

    while(!q.empty())
    {
        treenode *current = q.front();
        q.pop();

        if(current->module == -1)
        {
            outfile << "V ";
        }

        else if(current->module == -2)
        {
            outfile << "H ";
        }

        else
        {
            outfile << current->module << " ";
        }
    }
}

treenode *slicingtree::Search(int module)
{
    queue<treenode*> q;

    q.push(root);


    while (!q.empty())
    {
        treenode *current = q.front();
        q.pop();

        if(current->module == module)
            return current;

        if (current->left != NULL)
        {
            q.push(current->left);
        }
        if (current->right != NULL)
        {
            q.push(current->right);
        }
    }
}

treenode *slicingtree::Search_cut(int cut_num)
{
    stack<treenode*> check;

    int search_time = 0;

    check.push(root);

    while(check.size() > 0)
    {
        treenode *current = check.top();
        check.pop();

        if(current->module == -1 || current->module == -2)
        {
            search_time++;

            if(search_time == cut_num)
                return current;
        }

        if(current->left) check.push(current->left);
        if(current->right) check.push(current->right);
    }

}

void slicingtree::move1(int module1, int module2, Linkedlist table)
{
    treenode *temp;

    treenode *element1 = Search(module1);
    treenode *element2 = Search(module2);


    if(element1->parent == element2->parent) //swap 2 modules with same parent
    {
        temp = element1->parent;

        if(temp->left == element1)
        {
            temp->left = element2;
            temp->right = element1;
        }
        else
        {
            temp->left = element1;
            temp->right = element2;
        }

    }
    else //swap 2 modules with different parent
    {
        treenode *temp1 = element1->parent;
        treenode *temp2 = element2->parent;

        if(temp1->left == element1)
        {
            temp1->left = element2;
        }
        else
        {
            temp1->right = element2;
        }

        element2->parent = temp1;

        if(temp2->left == element2)
        {
            temp2->left = element1;
        }
        else
        {
            temp2->right = element1;
        }

        element1->parent = temp2;
    }

    int swap_module[2] = {module1,module2};

    queue<treenode*> q;

    q.push(element1);
    q.push(element2);

    for(int i = 0; i < 2; i++)
    {

        Listnode *shape = table.get_shape(swap_module[i]);
        treenode *update = q.front();
        q.pop();


        double wid_merge, hei_merge;

        vector<double> w = shape->width;
        vector<double> h = shape->height;

        if(update->parent->left == update)
        {
            wid_merge = update->parent->right->width;
            hei_merge = update->parent->right->height;
        }
        else
        {
            wid_merge = update->parent->left->width;
            hei_merge = update->parent->left->height;
        }


        double smallest_area;

        if(update->parent->module == -1)
        {
            smallest_area = (w[0] + wid_merge)*compare(hei_merge, h[0]);

            for(int i = 0; i < w.size(); i++)
            {
                if(smallest_area >= (w[i] + wid_merge)*compare(hei_merge, h[i]))
                {
                    smallest_area = (w[i] + wid_merge)*compare(hei_merge, h[i]);

                    update->width = w[i];
                    update->height = h[i];
                    update->area = w[i]*h[i];
                    update->parent->width = w[i] + wid_merge;
                    update->parent->height = compare(hei_merge, h[i]);
                    update->parent->area = smallest_area;
                }

            }
        }
        else
        {
            smallest_area = compare(w[0], wid_merge)*(hei_merge + h[0]);

            for(int i = 0; i < w.size(); i++)
            {
                if(smallest_area >= compare(w[i], wid_merge)*(hei_merge + h[i]))
                {
                    smallest_area = compare(w[i], wid_merge)*(hei_merge + h[i]);

                    update->width = w[i];
                    update->height = h[i];
                    update->area = w[i]*h[i];
                    update->parent->width = compare(w[i], wid_merge);
                    update->parent->height = hei_merge + h[i];
                    update->parent->area = smallest_area;
                }

            }
        }

        update = update->parent;

        while(update->parent != 0)
        {

            if(update->parent->left == update)
            {
                wid_merge = update->parent->right->width;
                hei_merge = update->parent->right->height;
            }
            else
            {
                wid_merge = update->parent->left->width;
                hei_merge = update->parent->left->height;
            }

            if(update->parent->module == -1)
            {
                update->parent->width = update->width + wid_merge;
                update->parent->height = compare(hei_merge, update->height);
                update->parent->area = update->parent->width*update->parent->height;
            }
            else
            {
                update->parent->width = compare(update->width, wid_merge);
                update->parent->height = hei_merge + update->height;
                update->parent->area = update->parent->width*update->parent->height;
            }

            update = update->parent;

        }

    }
}

void slicingtree::move2(int cut_num, Linkedlist table)
{
    treenode *element = Search_cut(cut_num);

    if(element->module == -1)
    {
        element->module = -2;
        element->width = (element->left->width > element->right->width) ? element->left->width : element->right->width;
        element->height = element->left->height + element->right->height;
        element->area = element->width*element->height;

    }
    else
    {
        element->module = -1;
        element->width = element->left->width + element->right->width;
        element->height = (element->left->height > element->right->height) ? element->left->height : element->right->height;
        element->area = element->width*element->height;

    }


    treenode *update = element;
    double wid_merge, hei_merge;

    Listnode *shape_left = table.get_shape(element->left->module);
    Listnode *shape_right = table.get_shape(element->right->module);

    vector<double> w_left, h_left, w_right, h_right;

    if(shape_left != 0)
    {
        w_left = shape_left->width;
        h_left = shape_left->height;
    }

    if(shape_right != 0)
    {
        w_right = shape_right->width;
        h_right = shape_right->height;
    }


    if(shape_left != 0 && shape_right != 0)
    {
        if(element->module == -1)
        {
            double smallest_area = (w_left[0] + w_right[0])*compare(h_left[0], h_right[0]);

            for(int i = 0; i < w_left.size(); i++)
            {
                for(int j = 0; j < w_right.size(); j++)
                {
                    if(smallest_area >= (w_left[i] + w_right[j])*compare(h_left[i], h_right[j]))
                    {
                        smallest_area = (w_left[i] + w_right[j])*compare(h_left[i], h_right[j]);

                        update->width = w_left[i] + w_right[j];
                        update->height = compare(h_left[i], h_right[j]);
                        update->area = smallest_area;
                        update->left->width = w_left[i];
                        update->left->height = h_left[i];
                        update->left->area = w_left[i]*h_left[i];
                        update->right->width = w_right[j];
                        update->right->height = h_right[j];
                        update->right->area = w_right[j]*h_right[j];
                    }
                }
            }
        }
        else
        {
            double smallest_area = compare(w_left[0], w_right[0])*(h_left[0] + h_right[0]);

            for(int i = 0; i < w_left.size(); i++)
            {
                for(int j = 0; j < w_right.size(); j++)
                {
                    if(smallest_area >= compare(w_left[i], w_right[j])*(h_left[i] + h_right[j]))
                    {
                        smallest_area = compare(w_left[i], w_right[j])*(h_left[i] + h_right[j]);

                        update->width = compare(w_left[i], w_right[j]);
                        update->height = h_left[i] + h_right[j];
                        update->area = smallest_area;
                        update->left->width = w_left[i];
                        update->left->height = h_left[i];
                        update->left->area = w_left[i]*h_left[i];
                        update->right->width = w_right[j];
                        update->right->height = h_right[j];
                        update->right->area = w_right[j]*h_right[j];
                    }
                }
            }

        }
    }
    else if(shape_left == 0 && shape_right != 0)
    {
        if(element->module == -1)
        {
            double smallest_area = (element->left->width + w_right[0])*compare(element->left->height, h_right[0]);

            for(int j = 0; j < w_right.size(); j++)
            {
                if(smallest_area >= (element->left->width + w_right[j])*compare(element->left->height, h_right[j]))
                {
                    smallest_area = (element->left->width + w_right[j])*compare(element->left->height, h_right[j]);

                    update->width = element->left->width + w_right[j];
                    update->height = compare(element->left->height, h_right[j]);
                    update->area = smallest_area;
                    update->right->width = w_right[j];
                    update->right->height = h_right[j];
                    update->right->area = w_right[j]*h_right[j];
                }
            }

        }
        else
        {
            double smallest_area = compare(element->left->width, w_right[0])*(element->left->height + h_right[0]);

            for(int j = 0; j < w_right.size(); j++)
            {
                if(smallest_area >= compare(element->left->width, w_right[j])*(element->left->height + h_right[j]))
                {
                    smallest_area = compare(element->left->width, w_right[j])*(element->left->height + h_right[j]);

                    update->width = compare(element->left->width, w_right[j]);
                    update->height = element->left->height + h_right[j];
                    update->area = smallest_area;
                    update->right->width = w_right[j];
                    update->right->height = h_right[j];
                    update->right->area = w_right[j]*h_right[j];
                }
            }


        }
    }
    else if(shape_right == 0 && shape_left != 0)
    {
        if(element->module == -1)
        {
            double smallest_area = (w_left[0] + element->right->width)*compare(h_left[0], element->right->height);

            for(int i = 0; i < w_left.size(); i++)
            {
                if(smallest_area >= (w_left[i] + element->right->width)*compare(h_left[i], element->right->height))
                {
                    smallest_area = (w_left[i] + element->right->width)*compare(h_left[i], element->right->height);

                    update->width = w_left[i] + element->right->width;
                    update->height = compare(h_left[i], element->right->height);
                    update->area = smallest_area;
                    update->left->width = w_left[i];
                    update->left->height = h_left[i];
                    update->left->area = w_left[i]*h_left[i];

                }

            }
        }
        else
        {
            double smallest_area = compare(w_left[0], element->right->width)*(h_left[0] + element->right->height);

            for(int i = 0; i < w_left.size(); i++)
            {

                if(smallest_area >= compare(w_left[i], element->right->width)*(h_left[i] + element->right->height))
                {
                    smallest_area = compare(w_left[i], element->right->width)*(h_left[i] + element->right->height);
                    update->width = compare(w_left[i], element->right->width);
                    update->height = h_left[i] + element->right->height;
                    update->area = smallest_area;
                    update->left->width = w_left[i];
                    update->left->height = h_left[i];
                    update->left->area = w_left[i]*h_left[i];

                }

            }

        }
    }


    while(update->parent != 0)
    {

        if(update->parent->left == update)
        {
            wid_merge = update->parent->right->width;
            hei_merge = update->parent->right->height;
        }
        else
        {
            wid_merge = update->parent->left->width;
            hei_merge = update->parent->left->height;
        }

        if(update->parent->module == -1)
        {
            update->parent->width = update->width + wid_merge;
            update->parent->height = compare(hei_merge, update->height);
            update->parent->area = update->parent->width*update->parent->height;
        }
        else
        {
            update->parent->width = compare(update->width, wid_merge);
            update->parent->height = hei_merge + update->height;
            update->parent->area = update->parent->width*update->parent->height;
        }

        update = update->parent;

    }

}

void slicingtree::move3(int cut_num, int module, int LorR, Linkedlist table)//leftchild: 2, rightchild: 1
{
    treenode *element = Search(module);
    treenode *cut = Search_cut(cut_num);
    treenode *parent_cut = Search_cut(cut_num-1);

    treenode *temp1 = cut->right;
    treenode *temp2 = cut->left;

    if(cut->parent->left == cut)
    {
        cut->parent->left = temp2;
        temp2->parent = cut->parent;
    }
    else
    {
        cut->parent->right = temp2;
        temp2->parent = cut->parent;
    }

    cut->parent = parent_cut;
    cut->left = temp1;
    cut->right = element;
    element->parent = cut;

    if(LorR == 2)
    {
        parent_cut->left = cut;
    }
    else if(LorR == 1)
    {
        parent_cut->right = cut;
    }


    Listnode *shape = table.get_shape(temp2->module);
    treenode *update = temp2;

    double wid_merge, hei_merge;
    vector<double> w_left, h_left, w_right, h_right;

    if(shape != 0)
    {

        vector<double> w = shape->width;
        vector<double> h = shape->height;

        if(update->parent->left == update)
        {
            wid_merge = update->parent->right->width;
            hei_merge = update->parent->right->height;
        }
        else
        {
            wid_merge = update->parent->left->width;
            hei_merge = update->parent->left->height;
        }


        double smallest_area;

        if(update->parent->module == -1)
        {
            smallest_area = (w[0] + wid_merge)*compare(hei_merge, h[0]);

            for(int i = 0; i < w.size(); i++)
            {
                if(smallest_area >= (w[i] + wid_merge)*compare(hei_merge, h[i]))
                {
                    smallest_area = (w[i] + wid_merge)*compare(hei_merge, h[i]);

                    update->width = w[i];
                    update->height = h[i];
                    update->area = w[i]*h[i];
                    update->parent->width = w[i] + wid_merge;
                    update->parent->height = compare(hei_merge, h[i]);
                    update->parent->area = smallest_area;
                }

            }
        }
        else
        {
            smallest_area = compare(w[0], wid_merge)*(hei_merge + h[0]);

            for(int i = 0; i < w.size(); i++)
            {
                if(smallest_area >= compare(w[i], wid_merge)*(hei_merge + h[i]))
                {
                    smallest_area = compare(w[i], wid_merge)*(hei_merge + h[i]);

                    update->width = w[i];
                    update->height = h[i];
                    update->area = w[i]*h[i];
                    update->parent->width = compare(w[i], wid_merge);
                    update->parent->height = hei_merge + h[i];
                    update->parent->area = smallest_area;
                }

            }
        }

    }

    else
    {

        Listnode *shape_left = table.get_shape(temp2->left->module);
        Listnode *shape_right = table.get_shape(temp2->right->module);

        if(shape_left != 0)
        {
            w_left = shape_left->width;
            h_left = shape_left->height;
        }

        if(shape_right != 0)
        {
            w_right = shape_right->width;
            h_right = shape_right->height;
        }


        if(shape_left != 0 && shape_right != 0)
        {
            if(update->module == -1)
            {
                double smallest_area = (w_left[0] + w_right[0])*compare(h_left[0], h_right[0]);

                for(int i = 0; i < w_left.size(); i++)
                {
                    for(int j = 0; j < w_right.size(); j++)
                    {
                        if(smallest_area >= (w_left[i] + w_right[j])*compare(h_left[i], h_right[j]))
                        {
                            smallest_area = (w_left[i] + w_right[j])*compare(h_left[i], h_right[j]);

                            update->width = w_left[i] + w_right[j];
                            update->height = compare(h_left[i], h_right[j]);
                            update->area = smallest_area;
                            update->left->width = w_left[i];
                            update->left->height = h_left[i];
                            update->left->area = w_left[i]*h_left[i];
                            update->right->width = w_right[j];
                            update->right->height = h_right[j];
                            update->right->area = w_right[j]*h_right[j];
                        }
                    }
                }
            }
            else
            {
                double smallest_area = compare(w_left[0], w_right[0])*(h_left[0] + h_right[0]);

                for(int i = 0; i < w_left.size(); i++)
                {
                    for(int j = 0; j < w_right.size(); j++)
                    {
                        if(smallest_area >= compare(w_left[i], w_right[j])*(h_left[i] + h_right[j]))
                        {
                            smallest_area = compare(w_left[i], w_right[j])*(h_left[i] + h_right[j]);

                            update->width = compare(w_left[i], w_right[j]);
                            update->height = h_left[i] + h_right[j];
                            update->area = smallest_area;
                            update->left->width = w_left[i];
                            update->left->height = h_left[i];
                            update->left->area = w_left[i]*h_left[i];
                            update->right->width = w_right[j];
                            update->right->height = h_right[j];
                            update->right->area = w_right[j]*h_right[j];
                        }
                    }
                }

            }
        }
        else if(shape_left == 0 && shape_right != 0)
        {
            if(update->module == -1)
            {
                double smallest_area = (update->left->width + w_right[0])*compare(update->left->height, h_right[0]);

                for(int j = 0; j < w_right.size(); j++)
                {
                    if(smallest_area >= (update->left->width + w_right[j])*compare(update->left->height, h_right[j]))
                    {
                        smallest_area = (update->left->width + w_right[j])*compare(update->left->height, h_right[j]);

                        update->width = update->left->width + w_right[j];
                        update->height = compare(update->left->height, h_right[j]);
                        update->area = smallest_area;
                        update->right->width = w_right[j];
                        update->right->height = h_right[j];
                        update->right->area = w_right[j]*h_right[j];
                    }
                }

            }
            else
            {
                double smallest_area = compare(update->left->width, w_right[0])*(update->left->height + h_right[0]);

                for(int j = 0; j < w_right.size(); j++)
                {
                    if(smallest_area >= compare(update->left->width, w_right[j])*(update->left->height + h_right[j]))
                    {
                        smallest_area = compare(update->left->width, w_right[j])*(update->left->height + h_right[j]);

                        update->width = compare(update->left->width, w_right[j]);
                        update->height = update->left->height + h_right[j];
                        update->area = smallest_area;
                        update->right->width = w_right[j];
                        update->right->height = h_right[j];
                        update->right->area = w_right[j]*h_right[j];
                    }
                }


            }
        }
        else if(shape_right == 0 && shape_left != 0)
        {
            if(element->module == -1)
            {
                double smallest_area = (w_left[0] + update->right->width)*compare(h_left[0], update->right->height);

                for(int i = 0; i < w_left.size(); i++)
                {
                    if(smallest_area >= (w_left[i] + update->right->width)*compare(h_left[i], update->right->height))
                    {
                        smallest_area = (w_left[i] + update->right->width)*compare(h_left[i], update->right->height);

                        update->width = w_left[i] + update->right->width;
                        update->height = compare(h_left[i], update->right->height);
                        update->area = smallest_area;
                        update->left->width = w_left[i];
                        update->left->height = h_left[i];
                        update->left->area = w_left[i]*h_left[i];

                    }

                }
            }
            else
            {
                double smallest_area = compare(w_left[0], update->right->width)*(h_left[0] + update->right->height);

                for(int i = 0; i < w_left.size(); i++)
                {

                    if(smallest_area >= compare(w_left[i], update->right->width)*(h_left[i] + update->right->height))
                    {
                        smallest_area = compare(w_left[i], update->right->width)*(h_left[i] + update->right->height);
                        update->width = compare(w_left[i], update->right->width);
                        update->height = h_left[i] + update->right->height;
                        update->area = smallest_area;
                        update->left->width = w_left[i];
                        update->left->height = h_left[i];
                        update->left->area = w_left[i]*h_left[i];

                    }

                }

            }
        }
        else
        {
            if(update->module == -1)
            {
                update->width = (update->left->width > update->right->width) ? update->left->width : update->right->width;
                update->height = update->left->height + update->right->height;
                update->area = update->width*update->height;

            }
            else
            {
                update->width = update->left->width + update->right->width;
                update->height = (update->left->height > update->right->height) ? update->left->height : update->right->height;
                update->area = update->width*update->height;

            }

        }

    }


    if(shape != 0)
        update = update->parent;

    while(update->parent != 0)
    {

        if(update->parent->left == update)
        {
            wid_merge = update->parent->right->width;
            hei_merge = update->parent->right->height;
        }
        else
        {
            wid_merge = update->parent->left->width;
            hei_merge = update->parent->left->height;
        }

        if(update->parent->module == -1)
        {
            update->parent->width = update->width + wid_merge;
            update->parent->height = compare(hei_merge, update->height);
            update->parent->area = update->parent->width*update->parent->height;
        }
        else
        {
            update->parent->width = compare(update->width, wid_merge);
            update->parent->height = hei_merge + update->height;
            update->parent->area = update->parent->width*update->parent->height;
        }

        update = update->parent;

    }

    update = cut;
    Listnode *shape_left = table.get_shape(cut->left->module);
    Listnode *shape_right = table.get_shape(cut->right->module);


    if(shape_left != 0)
    {
        w_left = shape_left->width;
        h_left = shape_left->height;
    }

    if(shape_right != 0)
    {
        w_right = shape_right->width;
        h_right = shape_right->height;
    }


    if(shape_left != 0 && shape_right != 0)
    {
        if(update->module == -1)
        {
            double smallest_area = (w_left[0] + w_right[0])*compare(h_left[0], h_right[0]);

            for(int i = 0; i < w_left.size(); i++)
            {
                for(int j = 0; j < w_right.size(); j++)
                {
                    if(smallest_area >= (w_left[i] + w_right[j])*compare(h_left[i], h_right[j]))
                    {
                        smallest_area = (w_left[i] + w_right[j])*compare(h_left[i], h_right[j]);

                        update->width = w_left[i] + w_right[j];
                        update->height = compare(h_left[i], h_right[j]);
                        update->area = smallest_area;
                        update->left->width = w_left[i];
                        update->left->height = h_left[i];
                        update->left->area = w_left[i]*h_left[i];
                        update->right->width = w_right[j];
                        update->right->height = h_right[j];
                        update->right->area = w_right[j]*h_right[j];
                    }
                }
            }
        }
        else
        {
            double smallest_area = compare(w_left[0], w_right[0])*(h_left[0] + h_right[0]);

            for(int i = 0; i < w_left.size(); i++)
            {
                for(int j = 0; j < w_right.size(); j++)
                {
                    if(smallest_area >= compare(w_left[i], w_right[j])*(h_left[i] + h_right[j]))
                    {
                        smallest_area = compare(w_left[i], w_right[j])*(h_left[i] + h_right[j]);

                        update->width = compare(w_left[i], w_right[j]);
                        update->height = h_left[i] + h_right[j];
                        update->area = smallest_area;
                        update->left->width = w_left[i];
                        update->left->height = h_left[i];
                        update->left->area = w_left[i]*h_left[i];
                        update->right->width = w_right[j];
                        update->right->height = h_right[j];
                        update->right->area = w_right[j]*h_right[j];
                    }
                }
            }

        }
    }
    else if(shape_left == 0)
    {
        if(update->module == -1)
        {
            double smallest_area = (update->left->width + w_right[0])*compare(update->left->height, h_right[0]);

            for(int j = 0; j < w_right.size(); j++)
            {
                if(smallest_area >= (update->left->width + w_right[j])*compare(update->left->height, h_right[j]))
                {
                    smallest_area = (update->left->width + w_right[j])*compare(update->left->height, h_right[j]);

                    update->width = update->left->width + w_right[j];
                    update->height = compare(update->left->height, h_right[j]);
                    update->area = smallest_area;
                    update->right->width = w_right[j];
                    update->right->height = h_right[j];
                    update->right->area = w_right[j]*h_right[j];
                }
            }

        }
        else
        {
            double smallest_area = compare(update->left->width, w_right[0])*(update->left->height + h_right[0]);

            for(int j = 0; j < w_right.size(); j++)
            {
                if(smallest_area >= compare(update->left->width, w_right[j])*(update->left->height + h_right[j]))
                {
                    smallest_area = compare(update->left->width, w_right[j])*(update->left->height + h_right[j]);

                    update->width = compare(update->left->width, w_right[j]);
                    update->height = update->left->height + h_right[j];
                    update->area = smallest_area;
                    update->right->width = w_right[j];
                    update->right->height = h_right[j];
                    update->right->area = w_right[j]*h_right[j];
                }
            }


        }
    }
    else if(shape_right == 0)
    {
        if(element->module == -1)
        {
            double smallest_area = (w_left[0] + update->right->width)*compare(h_left[0], update->right->height);

            for(int i = 0; i < w_left.size(); i++)
            {
                if(smallest_area >= (w_left[i] + update->right->width)*compare(h_left[i], update->right->height))
                {
                    smallest_area = (w_left[i] + update->right->width)*compare(h_left[i], update->right->height);

                    update->width = w_left[i] + update->right->width;
                    update->height = compare(h_left[i], update->right->height);
                    update->area = smallest_area;
                    update->left->width = w_left[i];
                    update->left->height = h_left[i];
                    update->left->area = w_left[i]*h_left[i];

                }

            }
        }
        else
        {
            double smallest_area = compare(w_left[0], update->right->width)*(h_left[0] + update->right->height);

            for(int i = 0; i < w_left.size(); i++)
            {

                if(smallest_area >= compare(w_left[i], update->right->width)*(h_left[i] + update->right->height))
                {
                    smallest_area = compare(w_left[i], update->right->width)*(h_left[i] + update->right->height);
                    update->width = compare(w_left[i], update->right->width);
                    update->height = h_left[i] + update->right->height;
                    update->area = smallest_area;
                    update->left->width = w_left[i];
                    update->left->height = h_left[i];
                    update->left->area = w_left[i]*h_left[i];

                }

            }

        }
    }
    else
    {
        if(update->module == -1)
        {
            update->width = (update->left->width > update->right->width) ? update->left->width : update->right->width;
            update->height = update->left->height + update->right->height;
            update->area = update->width*update->height;

        }
        else
        {
            update->width = update->left->width + update->right->width;
            update->height = (update->left->height > update->right->height) ? update->left->height : update->right->height;
            update->area = update->width*update->height;

        }

    }

    while(update->parent != 0)
    {

        if(update->parent->left == update)
        {
            wid_merge = update->parent->right->width;
            hei_merge = update->parent->right->height;
        }
        else
        {
            wid_merge = update->parent->left->width;
            hei_merge = update->parent->left->height;
        }

        if(update->parent->module == -1)
        {
            update->parent->width = update->width + wid_merge;
            update->parent->height = compare(hei_merge, update->height);
            update->parent->area = update->parent->width*update->parent->height;
        }
        else
        {
            update->parent->width = compare(update->width, wid_merge);
            update->parent->height = hei_merge + update->height;
            update->parent->area = update->parent->width*update->parent->height;
        }

        update = update->parent;

    }

}

double slicingtree::compare(double num1, double num2)
{
    if(num1>=num2)
        return num1;

    else
        return num2;
}
