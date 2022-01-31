// 区间树实现
#include<iostream>
#include<fstream>
#include<algorithm>
#include "RedBlackTree.h"

using namespace std;

typedef int T;


// 区间的结构定义
class Section
{
    private:
        T low;    // 区间低点
        T high;   // 区间高点
        T max;    // 区间树中的最有端点
    public:
        Section(){}
        Section(T _low,T _high){low=_low;high=_high;}
        void setMax(T _max){max=_max;}// 设置max值
        bool operator<(Section &s);// 重载小于运算符
        bool operator==(Section &s);// 重载等于运算符
        T getLow(){return low;}
        T getHigh(){return high;}
        T getMax(){return max;}
        friend istream& operator>>(istream& is,Section& s);// 重载输入运算符
        friend ostream& operator<<(ostream& os,const Section& s);// 重载输出运算符
};

// 维护max的函数
void maintainMAX(RBTreeNode<Section>* x)
{
    T MAX=max(x->left->key.getMax(),x->right->key.getMax());
    MAX=max(MAX,x->key.getHigh());
    x->key.setMax(MAX);
}

// 重载小于运算符
bool Section::operator<(Section &s)
{
    return low<s.low;
}

// 重载等于运算符
bool Section::operator==(Section &s)
{
    return low==s.low&&high==s.high;
}

// 重载输出运算符
ostream& operator<<(ostream& os,const Section& s)
{
    os<<'['<<s.low<<','<<s.high<<']'<<" max:"<<s.max;
    return os;
}

// 重载输入运算符
istream& operator>>(istream& is,Section& s)
{
    is>>s.low;
    is>>s.high;
    return is;
}

// 搜索是否存在重叠区间
// RBTreeNode<Section> * intervalSearch(RBTree<Section> *tree,Section *s)
// {
//     RBTreeNode<Section> *p=tree->get_root();
//     RBTreeNode<Section> *nil=tree->get_nil();

//     // 若不重叠
//     while(p!=nil&&((p->key.getLow()>=s->getHigh())||(p->key.getHigh()<=s->getLow())))
//     {
//         if(p->left!=nil && p->left->key.getMax()>s->getLow())  // 若左子树不为空且左子树的max大于区间的low，搜查左子树
//             p=p->left;
//         else                                                   // 否则搜查右子树
//             p=p->right;
//     }

//     if(p!=nil)
//         return p;
//     else
//         return NULL;
// }

// 检验是否重叠
bool isOverlaped(RBTreeNode<Section> *root,Section *s)
{
    return !(root->key.getLow()>=s->getHigh()||root->key.getHigh()<=s->getLow());
}

// 搜索是否存在重叠区间
void intervalSearch(RBTreeNode<Section> *root,Section *s)
{
    // 若当前节点重叠，则打印结果
    if(isOverlaped(root,s))
        cout<<root->key<<endl;
    
    if(root->left->color!=NIL&&root->left->key.getMax()>=s->getLow())
        intervalSearch(root->left,s);

    if(root->right->color!=NIL&&root->right->key.getMax()>=s->getLow()&&root->key.getLow()<=s->getHigh())
        intervalSearch(root->right,s);
}

int main()
{
    int num;
    ifstream infile;
    infile.open("insert2.txt",ios::in);

    cout<<0<<endl;

    // 读取插入节点总数
    infile>>num;

    cout<<0.1<<endl;

    // 创建红黑树
    RBTree<Section> *tree=new RBTree<Section>;

    // 插入节点
    for (int i = 0; i < num; i++)
    {
        RBTreeNode<Section> *node=new RBTreeNode<Section>;
        infile>>node->key;
        node->key.setMax(node->key.getHigh());
        tree->RB_insert(node,maintainMAX);
    }

    tree->layer_traversal(cout);

    // 寻找重叠的区间
    T low,high;
    cout<<"Input Section:";
    cin>>low>>high;

    Section *s=new Section(low,high);

    intervalSearch(tree->get_root(),s);

    // 删除节点
    Section del_node;
    cout<<"Enter a node to delete:"<<endl;
    while (cin>>del_node)
    {
        cout<<"--------------------------------"<<endl;
        RBTreeNode<Section> *dn=new RBTreeNode<Section>;
        dn=tree->find(del_node);
        tree->RB_delete(dn,maintainMAX);
        tree->layer_traversal(cout);
        cout<<"Enter a node to delete:"<<endl;
    }

    return 0;
}

