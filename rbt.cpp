#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <cstddef>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
using namespace std;




void findbuild1(rbtnode *root,int bnum,rbtnode* last, int* flag)
{
    if(*flag==1)
        return;
    if (root==NULL)
        return;

    findbuild1(root->left,bnum,last,flag);
    if(root->bnum ==bnum){
        if(last==NULL)
            return;
        heapnode* temp2 = last->twin;
        fout<<"("<<last->bnum<<","<<temp2->exec_time<<","<<temp2->total_time<<")"<<endl;
        *flag = 1;
    }
    last = root;
    findbuild1(root->right,bnum,last,flag);
}


void findbuild2(rbtnode *root,int bnum, int* flag){
    if(*flag==1)
        return;
    if (root==NULL)
        return;

    findbuild2(root->left,bnum,flag);
    if(root->bnum >bnum){
        heapnode* temp2 = root->twin;
        fout<<"("<<temp2->bnum<<","<<temp2->exec_time<<","<<temp2->total_time<<")"<<endl;
        *flag=1;
        return;
    }
    findbuild2(root->right,bnum,flag);
}



// find smallest node larger than given building number
// find smallest node larger than given building number
void rbt::nextnode(int bnum){
    int flag=0;
    findbuild2(root,bnum,&flag);

    if(flag!=1)
        fout<<"(0,0,0)"<<endl;

}

// find largest node less than given bnum
void rbt::prevnode(int bnum){
    int flag=0;


    findbuild1(root,bnum,NULL,&flag);

    if(flag==0)
        fout<<"(0,0,0)"<<endl;

}



// helper function to find a node in the tree
rbtnode* findnodeHelper(rbtnode* root,int bnum){
    if(root==NULL)
        return NULL;
    else if(root->bnum == bnum)
        return root;
    else if(bnum < root->bnum)
        return findnodeHelper(root->left,bnum);
    else
        return findnodeHelper(root->right,bnum);
}

//method to find a node in the tree
rbtnode* rbt::findnode(int bnum){

    rbtnode* temp = findnodeHelper(root,bnum);
    if(temp==NULL)
        fout<<"(0,0,0)\n";
    else{
        heapnode* temp2 = temp->twin;
        fout<<"("<<temp2->bnum<<","<<temp2->exec_time<<","<<temp2->total_time<<")"<<endl;
    }
    return temp;
}


// recursive function to do in order traversal in range low->high
void inorderHelper(rbtnode *root,int low,int high,int *flag)
{
    if (root==NULL)
        return;
    if(root->bnum > low)
        inorderHelper(root->left,low,high,flag);
    if(root->bnum >= low && root->bnum <=high){
        if(*flag==0)
          *flag=1;
        else
          fout<<",";
        fout <<"("<< root->bnum <<",";

        heapnode* temp = root->twin;
        fout<<temp->exec_time<<","<<temp->total_time<<")";
    }
    if(root->bnum <high)
        inorderHelper(root->right,low,high,flag);
}


// function to insert a new node into the structure
rbtnode* rbtinsert(rbtnode* root, rbtnode *ptr){
    if (root == NULL)
       return ptr;

    if (ptr->bnum < root->bnum){
        root->left  = rbtinsert(root->left, ptr);
        root->left->parent = root;
    }
    else if (ptr->bnum > root->bnum){
        root->right = rbtinsert(root->right, ptr);
        root->right->parent = root;
    }
    return root;
    // fixing of properties is done from the rbt method
}

 //do a rotate left operation
void rbt::rotateleft(rbtnode *&root, rbtnode *&ptr){
    rbtnode *ptr_right = ptr->right;

    ptr->right = ptr_right->left;

    if (ptr->right != NULL)
        ptr->right->parent = ptr;

    ptr_right->parent = ptr->parent;

    if (ptr->parent == NULL)
        root = ptr_right;

    else if (ptr == ptr->parent->left)
        ptr->parent->left = ptr_right;

    else
        ptr->parent->right = ptr_right;
    ptr->parent = ptr_right;
    ptr_right->left = ptr;
}

// do a rotate right operation
void rbt::rotateright(rbtnode *&root, rbtnode *&ptr){
    rbtnode *ptr_left = ptr->left;
    ptr->left = ptr_left->right;

    if (ptr->left != NULL)
        ptr->left->parent = ptr;

    ptr_left->parent = ptr->parent;

    if (ptr->parent == NULL)
        root = ptr_left;

    else if (ptr == ptr->parent->left)
        ptr->parent->left = ptr_left;

    else
        ptr->parent->right = ptr_left;

    ptr_left->right = ptr;
    ptr->parent = ptr_left;
}


// function fixes rbt violations caused by bst insertion
void rbt::fixtree(rbtnode *&root, rbtnode *&ptr){
    rbtnode *parent_ptr = NULL;
    rbtnode *grand_parent_ptr = NULL;

    while ((ptr != root) && (ptr->color != BLACK) && (ptr->parent->color == RED)){

        grand_parent_ptr = ptr->parent->parent;
        parent_ptr = ptr->parent;

        // when X=L
        if (parent_ptr == grand_parent_ptr->left){

            rbtnode *uncle_ptr = grand_parent_ptr->right;

            // when r = red ie uncle red
            if (uncle_ptr != NULL && uncle_ptr->color == RED){
                parent_ptr->color = BLACK;
                grand_parent_ptr->color = RED;
                uncle_ptr->color = BLACK;
                ptr = grand_parent_ptr;
            }

            else{
                // when Y = R
                if (ptr == parent_ptr->right){
                    rotateleft(root, parent_ptr);
                    ptr = parent_ptr;
                    parent_ptr = ptr->parent;
                }

                // when Y = L
                rotateright(root, grand_parent_ptr);
                {
                    bool clr;
                    clr = parent_ptr->color;
                    parent_ptr->color = grand_parent_ptr->color;
                    grand_parent_ptr->color = clr;
                }
                ptr = parent_ptr;
            }
        }

        // when X = R
        else{
            rbtnode *uncle_ptr = grand_parent_ptr->left;

            //when r=red
            if ((uncle_ptr != NULL) && (uncle_ptr->color == RED)){
                parent_ptr->color = BLACK;
                uncle_ptr->color = BLACK;
                grand_parent_ptr->color = RED;
                ptr = grand_parent_ptr;
            }
            else{
                //when Y = L
                if (ptr == parent_ptr->left){
                    rotateright(root, parent_ptr);
                    ptr = parent_ptr;
                    parent_ptr = ptr->parent;
                }
                //when Y = R
                rotateleft(root, grand_parent_ptr);
                {
                    bool clr;
                    clr = parent_ptr->color;
                    parent_ptr->color = grand_parent_ptr->color;
                    grand_parent_ptr->color = clr;
                }

                ptr = parent_ptr;
            }
        }
    }
    root->color = BLACK;
}


// rbt method to insert a new node
rbtnode* rbt::insert(const int &bnum){
    rbtnode *ptr = new rbtnode(bnum);
    rbtnode* bk = ptr;
   // bst insert
    root = rbtinsert(root, ptr);

    return bk;
    //violations are fixed from the main program
}

// Function in order traversal in range low to high

void rbt::inorder(int low,int high){
    int flag=0;
    inorderHelper(root,low,high,&flag);
    if(flag==0)
      fout<<"(0,0,0)";
    fout<<endl;
}




rbtnode* successor(rbtnode *node)
{
      rbtnode *k=NULL;
     if(node->left!=NULL)
     {
         k=node->left;
         while(k->right!=NULL)
              k=k->right;
     }
     else
     {
         k=node->right;
         while(k->left!=NULL)
              k=k->left;
     }
     return k;

}


void rbt::deletenode(rbtnode* p){

     p=root;
     rbtnode *k=NULL,*y=NULL, *q=NULL;


     if(p->left==NULL||p->right==NULL)
          y=p;
     else
          y=successor(p);

     if(y->left!=NULL)
          q=y->left;
     else
     {
          if(y->right!=NULL)
               q=y->right;
          else
               q=NULL;
     }
     if(q!=NULL)
          q->parent=y->parent;
     if(y->parent==NULL)
          root=q;
     else
     {
         if(y==y->parent->left)
            y->parent->left=q;
         else
            y->parent->right=q;
     }
     if(y!=p)
     {
         p->color=y->color;
         p->bnum=y->bnum;
         p->twin = y->twin;

         struct heapnode* temp;
         temp = p->twin;
         temp->twin = p;

     }
     if(k!=NULL)
         fixviolation(q);

}

//left rotate after delete
void rbt::leftrotate(rbtnode *p)
{
     if(p->right==NULL)
           return ;
     else
     {
           rbtnode *y=p->right;
           if(y->left!=NULL)
           {
                  p->right=y->left;
                  y->left->parent=p;
           }
           else
                  p->right=NULL;
           if(p->parent!=NULL)
                y->parent=p->parent;
           if(p->parent==NULL)
                root=y;
           else
           {
               if(p==p->parent->left)
                       p->parent->left=y;
               else
                       p->parent->right=y;
           }
           y->left=p;
           p->parent=y;
     }
}
//right rotation after delete
void rbt::rightrotate(rbtnode *p)
{
     if(p->left==NULL)
          return ;
     else
     {
         rbtnode *y=p->left;
         if(y->right!=NULL)
         {
                  p->left=y->right;
                  y->right->parent=p;
         }
         else
                 p->left=NULL;
         if(p->parent!=NULL)
                 y->parent=p->parent;
         if(p->parent==NULL)
               root=y;
         else
         {
             if(p==p->parent->left)
                   p->parent->left=y;
             else
                   p->parent->right=y;
         }
         y->right=p;
         p->parent=y;
     }
}


//function to fix violation after delete
void rbt::fixviolation(rbtnode *p)
{
    rbtnode *s;
    while(p!=root&&p->color==BLACK){
          if(p->parent->left==p){
                  s=p->parent->right;
                  if(s->color==RED){
                         s->color=BLACK;
                         p->parent->color=RED;
                         leftrotate(p->parent);
                         s=p->parent->right;
                  }
                  if(s->right->color==BLACK&&s->left->color==BLACK){
                         s->color=RED;
                         p=p->parent;
                  }
                  else{
                      if(s->right->color==BLACK){
                             s->left->color=BLACK;
                             s->color=RED;
                             rightrotate(s);
                             s=p->parent->right;
                      }
                      s->color=p->parent->color;
                      p->parent->color=BLACK;
                      s->right->color=BLACK;
                      leftrotate(p->parent);
                      p=root;
                  }
          }
          else{
                  s=p->parent->left;
                  if(s->color==RED){
                        s->color=BLACK;
                        p->parent->color=RED;
                        rightrotate(p->parent);
                        s=p->parent->left;
                  }
                  if(s->left->color==BLACK&&s->right->color==BLACK){
                        s->color=RED;
                        p=p->parent;
                  }
                  else{
                        if(s->left->color==BLACK){
                              s->right->color=BLACK;
                              s->color=RED;
                              leftrotate(s);
                              s=p->parent->left;
                        }
                        s->color=p->parent->color;
                        p->parent->color=BLACK;
                        s->left->color=BLACK;
                        rightrotate(p->parent);
                        p=root;
                  }
          }
       p->color=BLACK;
       root->color=BLACK;
    }
}

