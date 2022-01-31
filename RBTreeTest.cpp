#include<iostream>
#include<fstream>
#include "RedBlackTree.h"

using namespace std;

typedef float Type;

int main()
{   
    int num;
    ifstream infile;
    infile.open("insert1.txt",ios::in);
    ofstream outfile;
    outfile.open("LOT.txt");
    
    // 读取插入节点总数
    infile>>num;

    // 创建红黑树
    RBTree<Type> *tree=new RBTree<Type>;

    // 插入节点
    for (int i = 0; i < num; i++)
    {
        RBTreeNode<Type> *node=new RBTreeNode<Type>;
        infile>>node->key;
        tree->RB_insert(node);
    }
    
    tree->layer_traversal(cout);
    tree->layer_traversal(outfile);

    cout<<"Black Height:"<<tree->get_bh()<<endl;
    cout<<"Is Empty:"<<tree->is_empty()<<endl;


    // 查找节点
    RBTreeNode<Type> *n1=new RBTreeNode<Type>;
    n1=tree->find(16.3);
    if (n1!=NULL)
        cout<<"n1:"<<n1->key<<endl;
    else
        cout<<"Can't find n1!"<<endl;
    
    
    // 删除节点
    Type del_node;
    cout<<"Enter a node to delete:"<<endl;
    while (cin>>del_node)
    {
        cout<<"--------------------------------"<<endl;
        RBTreeNode<Type> *dn=new RBTreeNode<Type>;
        dn=tree->find(del_node);
        tree->RB_delete(dn);
        tree->layer_traversal(cout);
        cout<<"Enter a node to delete:"<<endl;
    }
    

    

    return 0;
}