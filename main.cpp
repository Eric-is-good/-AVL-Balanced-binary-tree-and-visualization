#include "AvlTree.hpp"
#include "show_tree.h"


int main()
{
    AvlTree<int> tree;
    //BTree t_to_show;  //show
    
    int value;
    int tmp;
    cout << "请输入整数建立二叉树(-1结束)：" << endl;
    while (cin >> value)
    {
        if (value == -1)
            break;
        tree.Insert(tree.root, value);
    }
    show_structure(tree);

    while (1) {
        cout << "请输入要删除的结点：";
        cin >> tmp;
        tree.Delete(tree.root, tmp);
        show_structure(tree);

        cout << "请输入要插入的结点：";
        cin >> tmp;
        tree.Insert(tree.root, tmp);
        show_structure(tree);
    }
    

}