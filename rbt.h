//Defining the two colors of RBT
enum color {RED, BLACK};

struct rbtnode{

    int bnum;
    bool color;
    rbtnode *left, *right, *parent;
    heapnode* twin; //Pointer connecting corresponding elements of RBT and Min heap

    rbtnode(int bnum){
       this->bnum = bnum;
       parent=left=right=NULL;
    }

};


// class of redblack tree
class rbt{

private:
    rbtnode *root;
protected:

    void rotateleft(rbtnode *&, rbtnode *&); // rotate left operation of rbt
    void rotateright(rbtnode *&, rbtnode *&); // rotate right operation of rbt
    void fixtree(rbtnode *&, rbtnode *&); // fix the tree to regain rbt properties
public:
    rbt(); // constructor
    rbtnode* insert(const int &n); // insert new node into the tree
    rbtnode* findnode(int bnum); // find a node in the tree
    void nextnode(int bnum); // find the next lowest node wrt inorder traversal
    void prevnode(int bnum); // find the previous largest node wrt inorder traversal

    void inorder(int, int); // find a range of building numbers in range low,high

    void deletenode(rbtnode*); // delete a node from tree

    void fixviolation(rbtnode*); // fix violation caused due to delete
    void rightrotate(rbtnode *p); // rotate right after delete
    void leftrotate(rbtnode *p); // rotate left after delete

};

//constructor
rbt::rbt(){
    root = NULL;
}

