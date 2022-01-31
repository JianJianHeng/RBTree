// 红黑树头文件
#include<iostream>
#include<queue>
#include<vector>
#include<string>
using namespace std;

enum color_set{RED,BLACK,NIL};

// 红黑树数据结构
// 节点数据结构
template<class T>
struct RBTreeNode
{
    color_set color;
    T key;
    RBTreeNode<T> * left;
    RBTreeNode<T> * right;
    RBTreeNode<T> * parent;
};



// RB树数据结构
template<class T>
class RBTree
{
    private:
        RBTreeNode<T> * root;// 根节点
        RBTreeNode<T> * nil;// nil节点
    public:
        RBTree()
        {
            // 初始化nil节点
            nil=new RBTreeNode<T>;
            nil->color=BLACK;
            // 一开始root为nil节点
            root=nil;
        }
        ~RBTree(){delete root;delete nil;}
        RBTreeNode<T> * get_root(){return root;}
        RBTreeNode<T> * get_nil(){return nil;}

        bool is_empty();// 判断是否是空树
        void layer_traversal(ostream &os);// 层次遍历输出结果
        int get_bh();// 获得红黑树的黑高
        RBTreeNode<T> * find(T key);// 查找红黑树中等于key的节点

        void left_rotate(RBTreeNode<T>* x,void(* f)(RBTreeNode<T>* a)=NULL);// 左旋
        void right_rotate(RBTreeNode<T>* x,void(* f)(RBTreeNode<T>* a)=NULL);// 右旋
        void RB_insert(RBTreeNode<T>* z,void(* f)(RBTreeNode<T>* a)=NULL);// 插入节点
        void RB_insert_fixup(RBTreeNode<T>* z,void(* f)(RBTreeNode<T>* a)=NULL); // 插入后调整红黑树

        void RB_transplant(RBTreeNode<T>* u,RBTreeNode<T>* v);// 用v节点替代z节点
        RBTreeNode<T>* Tree_minmum(RBTreeNode<T>* z);// 寻找节点最左下节点
        RBTreeNode<T>* RB_delete(RBTreeNode<T>* z,void(* f)(RBTreeNode<T>* a)=NULL);// 删除节点
        void RB_delete_fixup(RBTreeNode<T>* x,void(* f)(RBTreeNode<T>* a)=NULL); // 删除后调整红黑树
};



// 判断是否是空树
template<class T>
bool RBTree<T>:: is_empty()
{
    return root==nil;
}

// 左旋,假定x->right不为nil
template<class T>
void RBTree<T>:: left_rotate(RBTreeNode<T>* x,void(* f)(RBTreeNode<T>* a))
{
    RBTreeNode<T> *y=x->right;
    
    x->right=y->left;
    if (y->left!=nil)
        y->left->parent=x;

    y->parent=x->parent;
    if (x->parent==nil)// 若x是根节点，则将根节点指向y
        root=y;
    else if(x==x->parent->left)
        x->parent->left=y;
    else
        x->parent->right=y;

    y->left=x;
    x->parent=y;

    // 保持扩展数据结构
    if(f!=NULL)
    {
        // 先x后y
        f(x);
        f(y);
    }
}

// 右旋,假定x->left不为nil
template<class T>
void RBTree<T>:: right_rotate(RBTreeNode<T>* x,void(* f)(RBTreeNode<T>* a))
{
    RBTreeNode<T> *y=x->left;
    
    x->left=y->right;
    if (y->right!=nil)
        y->right->parent=x;

    y->parent=x->parent;
    if (x->parent==nil)// 若x是根节点，则将根节点指向y
        root=y;
    else if(x==x->parent->left)
        x->parent->left=y;
    else
        x->parent->right=y;

    y->right=x;
    x->parent=y;

    // 保持扩展数据结构
    if(f!=NULL)
    {
        // 先x后y
        f(x);
        f(y);
    }
}

// 红黑树插入
template<class T>
void RBTree<T>:: RB_insert(RBTreeNode<T>* z,void(* f)(RBTreeNode<T>* a))
{
    RBTreeNode<T> *y=nil;
    RBTreeNode<T> *x=root;

    // 找到插入位置
    while (x!=nil)
    {
        y=x;
        if (z->key<x->key)
            x=x->left;
        else
            x=x->right;
    }

    z->parent=y;
    z->left=nil;
    z->right=nil;

    // 若y是nil，则插入节点为根节点
    if (y==nil)
    {
        root=z;
        nil->left=z;
    }else if(z->key<y->key)
        y->left=z;
    else
        y->right=z;

    z->color=RED;

    // 维护扩展数据结构的性质
    if(f!=NULL)
    {
        RBTreeNode<T> *p=z->parent;
        // 向上不断调整直到nil节点
        while (p!=nil)
        {
            f(p);
            p=p->parent;
        }
    }
    // 调整红黑树
    RB_insert_fixup(z,f);
}


// 红黑树插入的调整
template<class T>
void RBTree<T>::RB_insert_fixup(RBTreeNode<T>* z,void(* f)(RBTreeNode<T>* a))
{
    while (z->parent->color==RED)
    {
        // case 1 2 3
        if (z->parent==z->parent->parent->left)
        {
            RBTreeNode<T> *y=z->parent->parent->right;//z节点的叔叔
                if (y->color==RED)
                {// case 1:z的叔叔是红色
                    cout<<"1 ";
                    
                    // 将z的父亲和叔叔节点染黑，祖父节点染红
                    z->parent->color=BLACK;
                    y->color=BLACK;
                    z->parent->parent->color=RED;
                    z=z->parent->parent;
                }
                else 
                {
                    if(z==z->parent->right)
                    {
                        // case 2:z的叔叔为黑色，且z为父亲节点的右孩子
                        cout<<"2 ";

                        // 左旋z的父亲节点，转换为case3
                        z=z->parent;
                        left_rotate(z,f);
                    }
                    // case 3:z的叔叔为红色，且z为父亲节点的左孩子
                    cout<<"3 ";

                    // 将父亲节点染黑，祖父节点染红，右旋祖父节点
                    z->parent->color=BLACK;
                    z->parent->parent->color=RED;
                    right_rotate(z->parent->parent,f);
                }    
        }
        else    // case 4 5 6
        {
            RBTreeNode<T> *y=z->parent->parent->left;//z节点的叔叔
            if (y->color==RED)
            {// case 4:z的叔叔是红色
                cout<<"4 ";
                
                // 将z的父亲和叔叔节点染黑，祖父节点染红
                z->parent->color=BLACK;
                y->color=BLACK;
                z->parent->parent->color=RED;
                z=z->parent->parent;
            }
            else 
            {
                if(z==z->parent->left)
                {
                    // case 5:z的叔叔为黑色，且z为父亲节点的左孩子
                    cout<<"5 ";

                    // 右旋z的父亲节点，转换为case3
                    z=z->parent;
                    right_rotate(z,f);
                }
                // case 6:z的叔叔为红色，且z为父亲节点的右孩子
                cout<<"6 ";

                // 将父亲节点染黑，祖父节点染红，左旋祖父节点
                z->parent->color=BLACK;
                z->parent->parent->color=RED;
                left_rotate(z->parent->parent,f);
            }    
        }
        cout<<endl;
    }
    
    // 将根节点染黑
    root->color=BLACK;
}

// 层次遍历红黑树
template<class T>
void RBTree<T>::layer_traversal(ostream &os)
{
    if (root==nil)
        return;
    
    queue<RBTreeNode<T>*> que;
    que.push(root);

    while (!que.empty())
    {
        RBTreeNode<T>* q=que.front();
        string color;
        switch (q->color)
        {
        case RED:
            color="RED";
            break;
        case BLACK:
            color="BLACK";
            break;
        default:
            break;
        }

        if (q==nil)
            os<<"NIL"<<endl;
        else
            os<<q->key<<","<<color<<endl;

        que.pop();

        if (q!=nil)
            que.push(q->left);
        if (q!=nil)
            que.push(q->right);
    }
}

// 获得红黑树的黑高
template<class T>
int RBTree<T>:: get_bh()
{
    if (root==nil)
        return 0;

    RBTreeNode<T> *x=root;
    int bh=0;

    while (x!=nil)
    {
        if (x->color==BLACK)
            bh++;
        x=x->left;
    }
    
    return bh;
}

// 查找红黑树中等于key的节点
template<class T>
RBTreeNode<T> * RBTree<T>::find(T key)
{
    if (is_empty())
        return NULL;
    
    RBTreeNode<T> *x=root;
    
    while (x!=nil)
    {
        if (x->key==key)
            break;
        else if(key<x->key)
            x=x->left;
        else
            x=x->right;
    }
    
    if (x==nil)
        return NULL;
    else
        return x;
}

// 用v节点替代z节点
template<class T>
void RBTree<T>::RB_transplant(RBTreeNode<T>* u,RBTreeNode<T>* v)
{
    // 若u节点时根节点，直接让v成为根节点
    if(u->parent==nil)
        root=v;
    else if(u==u->parent->left) // 左孩子
        u->parent->left=v;
    else
        u->parent->right=v;

    v->parent=u->parent;
}

// 寻找节点最左下节点
template<class T>
RBTreeNode<T>* RBTree<T>::Tree_minmum(RBTreeNode<T>* z)
{
    RBTreeNode<T> *p=z;
    while (p->left!=nil)
        p=p->left;
    
    return p;
}

// 删除节点
template<class T>
RBTreeNode<T>* RBTree<T>::RB_delete(RBTreeNode<T>* z,void(* f)(RBTreeNode<T>* a))
{
    RBTreeNode<T> *y=z;// y为替代z被删除的节点
    RBTreeNode<T> *x;// x用于记录树中y原本的位置
    color_set ori_corlor=y->color;// 记录待删除节点的颜色

    // 若是case1、case2情况，直接用孩子节点替代
    if(z->left==nil)
    {
        x=z->right;
        RB_transplant(z,z->right);
    }
    else if(z->right==nil)
    {
        x=z->left;
        RB_transplant(z,z->left);
    }
    else
    {// case3:寻找z节点中序遍历的后继节点
        y=Tree_minmum(z->right);
        ori_corlor=y->color;
        
        x=y->right;

        // 若y的父亲节点是z,这里x->p必须指向y，因为x有可能是nil节点
        if(y->parent==z)
            x->parent=y;
        else
        {// 用y的右孩子替代y的位置
            RB_transplant(y,y->right);
            y->right=z->right;
            y->right->parent=y;
        }

        RB_transplant(z,y);// 用y替代z
        y->left=z->left;
        y->left->parent=y;
        y->color=z->color;
    }
    
    // 维护扩展数据结构的性质
    if(f!=NULL)
    {
        RBTreeNode<T> *p=x->parent;
        // 向上不断调整直到nil节点
        while (p!=nil)
        {
            f(p);
            p=p->parent;
        }
    }

    // 若z的颜色为黑色才对x节点执行调整
    if(ori_corlor==BLACK)
        RB_delete_fixup(x,f);
}

// 删除后调整红黑树
template<class T>
void RBTree<T>::RB_delete_fixup(RBTreeNode<T>* x,void(* f)(RBTreeNode<T>* a))
{
    while (x!=root&&x->color==BLACK)
    {
        if(x==x->parent->left)
        {// case1、2、3、4:x是父节点的左孩子
            RBTreeNode<T> *w=x->parent->right;// w是x的兄弟节点
            if(w->color==RED)
            {// case 1:若w为红
                cout<<1<<endl;

                // 将w涂黑，父节点涂红，左旋父节点，转换成case2、3、4
                w->color=BLACK;
                x->parent->color=RED;
                left_rotate(x->parent,f);
                w=x->parent->right;// w指向新的兄弟节点
            }
            if(w->left->color==BLACK&&w->right->color==BLACK)
            {// case2:若w为黑且w的两个孩子均为黑
                cout<<2<<endl;

                // 将w涂红，将x指向x的父节点，问题上移
                w->color=RED;
                x=x->parent;
            }else 
            {   if(w->right->color==BLACK)
                {// case3:若w为黑且w的左孩子为红，右孩子为黑 
                    cout<<3<<endl;

                    // 将w的左孩子涂黑，w涂红，右旋w，将问题转成case4
                    w->left->color=BLACK;
                    w->color=RED;
                    right_rotate(w,f);
                    w=x->parent->right;// w指向新的兄弟节点
                }
                // case4:若w为黑且w的右孩子为红，左孩子可黑可红
                cout<<4<<endl;

                // x父节点的颜色由w继承，w的黑色由右孩子继承，x父节点继承x多余的一层黑色，左旋x的父节点，完成调整
                w->color=x->parent->color;
                x->parent->color=BLACK;
                w->right->color=BLACK;
                left_rotate(x->parent,f);
                x=root;// x指向根节点用于跳出循环，且避免右旋后导致根节点为红的情况
            }
        }
        else
        {// case5、6、7、8:x是父节点的右孩子
            RBTreeNode<T> *w=x->parent->left;// w是x的兄弟节点
            if(w->color==RED)
            {// case 5:若w为红
                cout<<5<<endl;

                // 将w涂黑，父节点涂红，右旋父节点，转换成case2、3、4
                w->color=BLACK;
                x->parent->color=RED;
                right_rotate(x->parent,f);
                w=x->parent->left;// w指向新的兄弟节点
            }
            if(w->left->color==BLACK&&w->right->color==BLACK)
            {// case6:若w为黑且w的两个孩子均为黑
                cout<<6<<endl;

                // 将w涂红，将x指向x的父节点，问题上移
                w->color=RED;
                x=x->parent;
            }else 
            {   if(w->left->color==BLACK)
                {// case7:若w为黑且w的右孩子为红，左孩子为黑 
                    cout<<7<<endl;

                    // 将w的右孩子涂黑，w涂红，左旋w，将问题转成case8
                    w->right->color=BLACK;
                    w->color=RED;
                    left_rotate(w,f);
                    w=x->parent->left;// w指向新的兄弟节点
                }
                // case8:若w为黑且w的左孩子为红，右孩子可黑可红
                cout<<8<<endl;

                // x父节点的颜色由w继承，w的黑色由左孩子继承，x父节点继承x多余的一层黑色，右旋x的父节点，完成调整
                w->color=x->parent->color;
                x->parent->color=BLACK;
                w->left->color=BLACK;
                right_rotate(x->parent,f);
                x=root;// x指向根节点用于跳出循环，且避免右旋后导致根节点为红的情况
            }
        }
    }

    // 将x涂黑
    x->color=BLACK;
}