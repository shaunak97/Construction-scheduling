#include <iostream>
// #include "Timer.h"
#include <fstream>
#include <sys/time.h>
#include <cstddef>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
using namespace std;





//#include "rbt.h"

//healper function for find prev
void findbuild1(rbtnode *root,int jobid,rbtnode* last, int* flag)
{
    if(*flag==1)
        return;
    if (root==NULL)
        return;

    findbuild1(root->left,jobid,last,flag);
    if(root->jobid ==jobid){
        if(last==NULL)
            return;
        heapnode* temp2 = last->twin;
        fout<<"("<<last->jobid<<","<<temp2->exec_time<<","<<temp2->total_time<<")"<<endl;
        *flag = 1;
    }
    last = root;
    findbuild1(root->right,jobid,last,flag);
}

//healper function for nextnode
void findbuild2(rbtnode *root,int jobid, int* flag){
    if(*flag==1)
        return;
    if (root==NULL)
        return;

    findbuild2(root->left,jobid,flag);
    if(root->jobid >jobid){
        heapnode* temp2 = root->twin;
        fout<<"("<<temp2->jobID<<","<<temp2->exec_time<<","<<temp2->total_time<<")"<<endl;
        *flag=1;
        return;
    }
    findbuild2(root->right,jobid,flag);
}



// find smallest node larger than given building number
// find smallest node larger than given building number
void rbt::nextnode(int jobid){
    int flag=0;
    findbuild2(root,jobid,&flag);

    if(flag!=1)
        fout<<"(0,0,0)"<<endl;

}

// find largest node less than given jobid
void rbt::prevnode(int jobid){
    int flag=0;


    findbuild1(root,jobid,NULL,&flag);

    if(flag==0)
        fout<<"(0,0,0)"<<endl;

}



// helper function to find a node in the tree
rbtnode* findnodeHelper(rbtnode* root,int jobid){
    if(root==NULL)
        return NULL;
    else if(root->jobid == jobid)
        return root;
    else if(jobid < root->jobid)
        return findnodeHelper(root->left,jobid);
    else
        return findnodeHelper(root->right,jobid);
}

//method to find a node in the tree
rbtnode* rbt::findnode(int jobid){

    rbtnode* temp = findnodeHelper(root,jobid);
    if(temp==NULL)
        fout<<"(0,0,0)\n";
    else{
        heapnode* temp2 = temp->twin;
        fout<<"("<<temp2->jobID<<","<<temp2->exec_time<<","<<temp2->total_time<<")"<<endl;
    }
    return temp;
}


// recursive function to do in order traversal in range low->high
void inorderHelper(rbtnode *root,int low,int high,int *flag)
{
    if (root==NULL)
        return;
    if(root->jobid > low)
        inorderHelper(root->left,low,high,flag);
    if(root->jobid >= low && root->jobid <=high){
        if(*flag==0)
          *flag=1;
        else
          fout<<",";
        fout <<"("<< root->jobid <<",";

        heapnode* temp = root->twin;
        fout<<temp->exec_time<<","<<temp->total_time<<")";
    }
    if(root->jobid <high)
        inorderHelper(root->right,low,high,flag);
}


// function to insert a new node into the structure
rbtnode* rbtinsert(rbtnode* root, rbtnode *ptr){
    if (root == NULL)
       return ptr;

    if (ptr->jobid < root->jobid){
        root->left  = rbtinsert(root->left, ptr);
        root->left->parent = root;
    }
    else if (ptr->jobid > root->jobid){
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
rbtnode* rbt::insert(const int &jobid){
    rbtnode *ptr = new rbtnode(jobid);
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
         p->jobid=y->jobid;
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

