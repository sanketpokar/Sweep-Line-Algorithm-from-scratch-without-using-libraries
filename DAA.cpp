#include<iostream>
#include<utility>
#include<vector>
#include<unistd.h>
#include<fstream>
#include<ctime>
using namespace std;

//Sanket Pokar 2018B3A70356H
//Dhruvikaa Ahuja 2018B3A70916H
//Manu Priyam Mittal 2018B3A71051H
//Ishita Menon 2018B5A70013H

float sweepposition;

class segment;

///This is the class to store points
///
///Stores the x-coordinates, y-coordinates of the line and also stores the associated segments(if the segment passes, is the upper end point, or the lower end point)
class Point
{
    public:
    float x,y;
    vector<segment *> upper;      //vector of segments where the point is upper end point
    vector<segment *> middle;     //
    vector<segment *> lower;      //vector of segments where the point is lower end point
};


///This function figures out if one point is above in precedence or not.
///
///Takes input as point objects, figures out if the first Point object is above another.
int islessthan(Point *p1, Point *p2)
{
    float x1 = p1->x;
    float y1 = p1->y;
    float x2 = p2->x;
    float y2 = p2->y;
    if(y1>y2)
    {
        return 1;
    }
    else if(y1==y2)
    {
        if(x1<x2)
        {
            return 1;
        }
    }
    return 0;
}

int isitequal(Point *p1, Point *p2)
{
    float x1 = p1->x;
    float y1 = p1->y;
    float x2 = p2->x;
    float y2 = p2->y;
    if(x1==x2 && y1==y2)
    {
        return 1;
    }
    return 0;
}




///class to store segments
///
///Stores upper endpoint and lower endpoint pointers to point objects.
class segment
{
    public:
    Point *p1;       //upper endpoint
    Point *p2;       //lower endpoint
};




float findslope(segment *s)
{
    float x1 = s->p1->x;
    float y1 = s->p1->y;
    float x2 = s->p2->x;
    float y2 = s->p2->y;
    return (y2-y1)/(x2-x1);
}

bool isitequal(segment *s1, segment *s2)
{
    if(s1==NULL || s2==NULL)
    {
        return true;
    }
    if(isitequal(s1->p1, s2->p1) && isitequal(s1->p2,s2->p2))
    {
        return true;
    }
    return false;
}

int islessthan(segment *s1, segment *s2)
{
    float x1 = s1->p1->x;
    float y1 = s1->p1->y;
    float m1 = findslope(s1);
    float p1intercept;
    if(m1!=0)
    {
    p1intercept = x1+((sweepposition-y1)/m1);
    }
    else
    {
    p1intercept = x1;
    }
    float x2 = s2->p1->x;
    float y2 = s2->p1->y;
    float m2 = findslope(s2);
    float p2intercept;
    if(m2!=0)
    {
    p2intercept = x2+((sweepposition-y2)/m2);
    }
    else
    {
    p2intercept = x2;
    }

    if(p1intercept<p2intercept)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


float findintercept(segment* s)
{
    float slope = findslope(s);
    if(slope==0)
    {
        return s->p1->x;
    }
    return ((sweepposition-(s->p1->y))/slope)+(s->p1->x);
}






enum Color {RED, BLACK};

///Node to store segment objects in a RED BLACK tree.
struct segNode
{
    segment* data;
    bool color;
    segNode *left, *right, *parent;
    segNode(segment* data)
    {
       this->data = data;
       left = right = parent = NULL;
       this->color = RED;
    }
    bool isOnLeft()
    {
    return this==parent->left;
    }
    void moveDown(segNode *nParent) 
    {
        if (parent != NULL) {
        if (isOnLeft()) 
        {
        parent->left = nParent;
        } 
        else 
        {
        parent->right = nParent;
        }
    }
    nParent->parent = parent;
    parent = nParent;
    }

    segNode *sibling()
    {
        if(parent == NULL)
        {
            return NULL;
        }
        else if(parent->right == this)
        {
            return parent->left;
        }
        else
        {
            return parent->right;
        }
    }

    bool hasRedChild()
    {
        if(left!=NULL && left->color==RED)
        {
            return true;
        }
        if(right!=NULL && right->color==RED)
        {
            return true;
        }
        return false;
    }
};




///Class to store a status Red Black Tree
class statusRBTree
{
private:
    segNode *root;
protected:
    void rotateLeft(segNode *&, segNode *&);
    void rotateRight(segNode *&, segNode *&);
    void fixViolation(segNode *&, segNode *&);
public:
    statusRBTree() { root = NULL; }
    void insert(segment *n);
    void leftRotate(segNode *x);
    void rightRotate(segNode *x);
    segNode *BSTreplace(segNode *x);
    void fixDoubleBlack(segNode *x);
    void deleteNode(segNode *v); 
    segNode* leftneighbor(Point *p);
    segNode* rightneighbor(Point *p);
    segNode* predecessor(segNode *x);
    segNode* successor(segNode *x);
    segNode *dosearch(segNode *x, segment *s)
    {
    if(x==NULL || isitequal(x->data,s))
    {
        return x;
    }
    if(islessthan(x->data,s))
    {
        return dosearch(x->right, s);
    }
    return dosearch(x->left, s);
    }

    segNode* search(segment *s)
    {
    return dosearch(root, s);
    }
};

///Help to RB Insert
segNode* BSTInsert(segNode* root, segNode *pt)
{
    if (root == NULL)
       return pt;
 
 
    if (islessthan(pt->data, root->data))
    {
        root->left  = BSTInsert(root->left, pt);
        root->left->parent = root;
    }
    else
    {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }
    return root;
}

///Rotations of RB  tree
void statusRBTree::rotateLeft(segNode *&root, segNode *&pt)
{
    segNode *pt_right = pt->right;
 
    pt->right = pt_right->left;
 
    if (pt->right != NULL)
        pt->right->parent = pt;
 
    pt_right->parent = pt->parent;
 
    if (pt->parent == NULL)
        root = pt_right;
 
    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;
 
    else
        pt->parent->right = pt_right;
 
    pt_right->left = pt;
    pt->parent = pt_right;
}
 

///Rotations of RB Tree
void statusRBTree::rotateRight(segNode *&root, segNode *&pt)
{
    segNode *pt_left = pt->left;
 
    pt->left = pt_left->right;
 
    if (pt->left != NULL)
        pt->left->parent = pt;
 
    pt_left->parent = pt->parent;
 
    if (pt->parent == NULL)
        root = pt_left;
 
    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;
 
    else
        pt->parent->right = pt_left;
 
    pt_left->right = pt;
    pt->parent = pt_left;
}

///Fixes insertion RB tree rule violations
void statusRBTree::fixViolation(segNode *&root, segNode *&pt)
{
    segNode *parent_pt = NULL;
    segNode *grand_parent_pt = NULL;
 
    while ((pt != root) && (pt->color != BLACK) &&
           (pt->parent->color == RED))
    {
 
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;
        if (parent_pt == grand_parent_pt->left)
        {
 
            segNode *uncle_pt = grand_parent_pt->right;
 
            if (uncle_pt != NULL && uncle_pt->color ==
                                                   RED)
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
 
            else
            {
                if (pt == parent_pt->right)
                {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateRight(root, grand_parent_pt);
                swap(parent_pt->color,
                           grand_parent_pt->color);
                pt = parent_pt;
            }
        }
        else
        {
            segNode *uncle_pt = grand_parent_pt->left;
 
            if ((uncle_pt != NULL) && (uncle_pt->color ==
                                                    RED))
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else
            {
                if (pt == parent_pt->left)
                {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->color,
                         grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }
 
    root->color = BLACK;
}

///Insertions in RB tree
void statusRBTree::insert(segment *data)
{
    segNode *pt = new segNode(data);
    root = BSTInsert(root, pt);
    fixViolation(root, pt);
}


///Rotations in RB tree
void statusRBTree::leftRotate(segNode *x) 
{

    segNode *nParent = x->right;
    if (x == root)
      root = nParent;
 
    x->moveDown(nParent);
    x->right = nParent->left;
    if (nParent->left != NULL)
      nParent->left->parent = x;

    nParent->left = x;
  }
 
 ///Rotations in RB tree
  void statusRBTree::rightRotate(segNode *x) 
  {
    segNode *nParent = x->left;
 
    if (x == root)
      root = nParent;
 
    x->moveDown(nParent);
 
    x->left = nParent->right;
    if (nParent->right != NULL)
      nParent->right->parent = x;
 
    nParent->right = x;
  }


///Helper function
segNode *statusRBTree::BSTreplace(segNode *x) 
{
    if (x->left != NULL and x->right != NULL)
    {
        x = x->right;
        while(x->left!=NULL)
        {
            x = x->left;
        }
        return x;
    }
 
    if (x->left == NULL and x->right == NULL)
      return NULL; 
 
    if (x->left != NULL)
      return x->left;
    else
      return x->right;
}

///Swaps nodes
void swapvalues(segNode *u, segNode *v)
{
    segment *temp = u->data;
    u->data = v->data;
    v->data = temp;
}

///Fixes double black violation in RB tree
  void statusRBTree::fixDoubleBlack(segNode *x) {
    if (x == root)
      return;
 
    segNode *sibling = x->sibling();
    segNode *parent = x->parent;
    if (sibling == NULL) {
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == RED) {
        parent->color = RED;
        sibling->color = BLACK;
        if (sibling->isOnLeft()) {
          rightRotate(parent);
        } else {
          leftRotate(parent);
        }
        fixDoubleBlack(x);
      } else {
        if (sibling->hasRedChild()) {
          // at least 1 red children
          if (sibling->left != NULL and sibling->left->color == RED) {
            if (sibling->isOnLeft()) {
              // left left
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rightRotate(parent);
            } else {
              // right left
              sibling->left->color = parent->color;
              rightRotate(sibling);
              leftRotate(parent);
            }
          } else {
            if (sibling->isOnLeft()) {
              // left right
              sibling->right->color = parent->color;
              leftRotate(sibling);
              rightRotate(parent);
            } else {
              // right right
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              leftRotate(parent);
            }
          }
          parent->color = BLACK;
        } else {
          // 2 black children
          sibling->color = RED;
          if (parent->color == BLACK)
            fixDoubleBlack(parent);
          else
            parent->color = BLACK;
        }
      }
    }
  }

///Successor in RB Tree
segNode* statusRBTree::successor(segNode *x)
{
    segNode *ahead = NULL;
    segNode *current = root;
    if(!root)
    {
        return NULL;
    }
    while(current && !isitequal(current->data, x->data))
    {   

        if(islessthan(x->data, current->data))
        {
            ahead = current;
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    if(current && current->right)
    {
        segNode* temp = current->right;
        while(temp->left)
        {
            temp = temp->left;
        }
        return temp;
    }
    return ahead;
}

///Predecessor in RB tree
segNode* statusRBTree::predecessor(segNode *x)
{
    segNode *prev = NULL;
    segNode *current = root;
    if(!root)
    {
        return NULL;
    }
    while(current && !isitequal(current->data, x->data))
    {
        if(islessthan(x->data, current->data))
        {
            current = current->left;
        }
        else
        {
            prev = current;
            current = current->right;
        }
    }
    if(current && current->left)
    {
        segNode *ans = current->left;
        while(ans->right)
        {
            ans = ans->right;
        }
        return ans;
    }
    return prev;
}


///Deletes nodes.
void statusRBTree::deleteNode(segNode *v) 
{
    if(v==NULL)
    {
        return;
    }
    segNode *u = BSTreplace(v);
 
    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
    segNode *parent = v->parent;
 
    if (u == NULL) {
      if (v == root) 
      {
        root = NULL;
      } else {
        if (uvBlack) {
          fixDoubleBlack(v);
        } else {
          if (v->sibling() != NULL)
            v->sibling()->color = RED;
        }
 
        if (v->isOnLeft()) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      }
      delete v;
      return;
    }
 
    if (v->left == NULL || v->right == NULL) {
      if (v == root) {
        v->data = u->data;
        v->left = v->right = NULL;
        delete u;
      } else {
        if (v->isOnLeft()) {
          parent->left = u;
        } else {
          parent->right = u;
        }
        delete v;
        u->parent = parent;
        if (uvBlack) {
          fixDoubleBlack(u);
        } else {
          u->color = BLACK;
        }
      }
      return;
    }
 
    swapvalues(u, v);
    deleteNode(u);
  }



































///Class to make nodes for event RB tree.
class Node
{
    public:
        Point* data;
        bool color;
        Node *left, *right, *parent;
        Node(Point* data)
        {
        this->data = data;
        left = right = parent = NULL;
        this->color = RED;
        }

    bool isOnLeft()
    {
    return this==parent->left;
    }
    void moveDown(Node *nParent) 
    {
        if (parent != NULL) {
        if (isOnLeft()) 
        {
        parent->left = nParent;
        } 
        else 
        {
        parent->right = nParent;
        }
    }
    nParent->parent = parent;
    parent = nParent;
    }

    Node *sibling()
    {
        if(parent == NULL)
        {
            return NULL;
        }
        else if(parent->right == this)
        {
            return parent->left;
        }
        else
        {
            return parent->right;
        }
    }

    bool hasRedChild()
    {
        if(left!=NULL && left->color==RED)
        {
            return true;
        }
        if(right!=NULL && right->color==RED)
        {
            return true;
        }
        return false;
    }
};




///Event RB tree.
class eventRBTree
{
private:
    Node *root;
protected:
    void rotateLeft(Node *&, Node *&);
    void rotateRight(Node *&, Node *&);
    void fixViolation(Node *&, Node *&);
public:
    eventRBTree() { root = NULL; }
    void insert(Point *n);
    bool isitempty();
    void fixDoubleBlack(Node *x);
    void deleteNode(Node *x);
    Node *successor(Node *x);
    Node *search(float x, float y);
    void leftRotate(Node *x);
    void rightRotate(Node *x);
    Node *BSTreplace(Node* x);
    Point *pop();
    void print();
    
};

///Helper function to insert in Rb tree
Node* BSTInsert(Node* root, Node *pt)
{
    if (root == NULL)
       return pt;
 
 
    if (islessthan(pt->data, root->data))
    {
        root->left  = BSTInsert(root->left, pt);
        root->left->parent = root;
    }
    else
    {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }
    return root;
}

Node *dosearch(Node *temp, float x, float y)
{
    
    Point *p = new Point;
    p->x = x;
    p->y = y;

    if(temp==NULL || isitequal(temp->data, p))
    {
        delete p;
        return temp;
    }
    else if(islessthan(temp->data,p))
    {
        delete p;
        return dosearch(temp->right, x, y);
    }
    else
    {
        delete p;
        return dosearch(temp->left, x, y);
    }
}

///Searches in event rb tree
Node* eventRBTree::search(float x, float y)
{
    if(root==NULL)
    {
        return root;
    }
    return dosearch(root, x, y);
}


///Rotations in RB tree
void eventRBTree::rotateLeft(Node *&root, Node *&pt)
{
    Node *pt_right = pt->right;
 
    pt->right = pt_right->left;
 
    if (pt->right != NULL)
        pt->right->parent = pt;
 
    pt_right->parent = pt->parent;
 
    if (pt->parent == NULL)
        root = pt_right;
 
    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;
 
    else
        pt->parent->right = pt_right;
 
    pt_right->left = pt;
    pt->parent = pt_right;
}
 
///Rotations in a RB tree
void eventRBTree::rotateRight(Node *&root, Node *&pt)
{
    Node *pt_left = pt->left;
 
    pt->left = pt_left->right;
 
    if (pt->left != NULL)
        pt->left->parent = pt;
 
    pt_left->parent = pt->parent;
 
    if (pt->parent == NULL)
        root = pt_left;
 
    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;
 
    else
        pt->parent->right = pt_left;
 
    pt_left->right = pt;
    pt->parent = pt_left;
}

///Insertion violation fixation
void eventRBTree::fixViolation(Node *&root, Node *&pt)
{
    Node *parent_pt = NULL;
    Node *grand_parent_pt = NULL;
 
    while ((pt != root) && (pt->color != BLACK) &&
           (pt->parent->color == RED))
    {
 
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;
        if (parent_pt == grand_parent_pt->left)
        {
 
            Node *uncle_pt = grand_parent_pt->right;
 
            if (uncle_pt != NULL && uncle_pt->color ==
                                                   RED)
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
 
            else
            {
                if (pt == parent_pt->right)
                {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateRight(root, grand_parent_pt);
                swap(parent_pt->color,
                           grand_parent_pt->color);
                pt = parent_pt;
            }
        }
        else
        {
            Node *uncle_pt = grand_parent_pt->left;
 
            if ((uncle_pt != NULL) && (uncle_pt->color ==
                                                    RED))
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else
            {
                if (pt == parent_pt->left)
                {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->color,
                         grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }
 
    root->color = BLACK;
}

///Rotations in a RB tree.
void eventRBTree::leftRotate(Node *x) {
    Node *nParent = x->right;
 
    if (x == root)
      root = nParent;
 
    x->moveDown(nParent);
 
    x->right = nParent->left;
    if (nParent->left != NULL)
      nParent->left->parent = x;
 
    nParent->left = x;
  }
 ///Rotations in a RB Tree
  void eventRBTree::rightRotate(Node *x) 
  {
    Node *nParent = x->left;
 
    if (x == root)
      root = nParent;
 
    x->moveDown(nParent);
 
    x->left = nParent->right;
    if (nParent->right != NULL)
      nParent->right->parent = x;
 
    nParent->right = x;
  }

///Insertion in a RB tree
void eventRBTree::insert(Point *data)
{
    Node *pt = new Node(data);
    root = BSTInsert(root, pt);
    fixViolation(root, pt);
}

///Returns yes if event tree is empty
bool eventRBTree::isitempty()
{
    if(root==NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}





///Helper function 
Node *eventRBTree::BSTreplace(Node *x) 
{
    if (x->left != NULL and x->right != NULL)
    {
        x = x->right;
        while(x->left!=NULL)
        {
            x = x->left;
        }
        return x;
    }
 
    if (x->left == NULL and x->right == NULL)
      return NULL; 
 
    if (x->left != NULL)
      return x->left;
    else
      return x->right;
}


void swapvalues(Node *u, Node *v)
{
    Point *temp = u->data;
    u->data = v->data;
    v->data = temp;
}

///Fixes double black in RB deletion
  void eventRBTree::fixDoubleBlack(Node *x) {
    if (x == root)
      return;
 
    Node *sibling = x->sibling(), *parent = x->parent;
    if (sibling == NULL) {
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == RED) {
        parent->color = RED;
        sibling->color = BLACK;
        if (sibling->isOnLeft()) {
          rightRotate(parent);
        } else {
          leftRotate(parent);
        }
        fixDoubleBlack(x);
      } else {
        if (sibling->hasRedChild()) {
          if (sibling->left != NULL and sibling->left->color == RED) {
            if (sibling->isOnLeft()) {
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rightRotate(parent);
            } else {
              sibling->left->color = parent->color;
              rightRotate(sibling);
              leftRotate(parent);
            }
          } else {
            if (sibling->isOnLeft()) {
              sibling->right->color = parent->color;
              leftRotate(sibling);
              rightRotate(parent);
            } else {
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              leftRotate(parent);
            }
          }
          parent->color = BLACK;
        } else {
          sibling->color = RED;
          if (parent->color == BLACK)
            fixDoubleBlack(parent);
          else
            parent->color = BLACK;
        }
      }
    }
  }

///Deletes node
void eventRBTree::deleteNode(Node *v) 
{
    Node *u = BSTreplace(v);
 
    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
    Node *parent = v->parent;
 
    if (u == NULL) {
      if (v == root) 
      {
        root = NULL;
      } else {
        if (uvBlack) {
          fixDoubleBlack(v);
        } else {
          if (v->sibling() != NULL)
            v->sibling()->color = RED;
        }
 
        if (v->isOnLeft()) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      }
      delete v;
      return;
    }
 
    if (v->left == NULL || v->right == NULL) {
      if (v == root) {
        v->data = u->data;
        v->left = v->right = NULL;
        delete u;
      } else {
        if (v->isOnLeft()) {
          parent->left = u;
        } else {
          parent->right = u;
        }
        delete v;
        u->parent = parent;
        if (uvBlack) {
          fixDoubleBlack(u);
        } else {
          u->color = BLACK;
        }
      }
      return;
    }
 
    swapvalues(u, v);
    deleteNode(u);
  }




///Pops from event tree
Point* eventRBTree::pop()
{
    Node *temp = root;
    while(temp->left)
    {
        temp = temp->left;
    }
    Point *p = temp->data;
    deleteNode(temp);
    return p;
}



int findupperpoint(Point* p1, Point* p2)
{
    if(p1->y>p2->y)
    {
        return 1;
    }
    else if(p1->y==p2->y)
    {
        if(p1->x<p2->x)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    return 2;
}


eventRBTree eventtree;
statusRBTree statustree;

void handleverticalevent(segment* s1, segment* s2)
{
        float x = s1->p1->x;
        float m = findslope(s2);
        float x1 = s2->p1->x;
        float y1 = s2->p1->y;
        float y = y1+(m*(x-x1));
        float yup = s1->p1->y;
        float ydown = s1->p2->y;
        if(y>yup || y<ydown)
        {
            return;
        }

        if(s2->p1->x>x && s2->p2->x>x)
        {
            return;
        }
        if(s2->p1->x<x && s2->p2->x<x)
        {
            return;
        }
        Point *p = new Point;
        p->x = x;
        p->y = y;
        if(eventtree.search(x,y) == NULL)
        {
            p->middle.push_back(s1);
            p->middle.push_back(s2);
            eventtree.insert(p);
        }
        else
        {
             Node *n = eventtree.search(x,y);
            int f1=0;
            int f2=0;
            vector<segment *> upper = n->data->upper;
            vector<segment *> lower = n->data->lower;
            vector<segment *> middle = n->data->middle;
            for(int i=0; i<upper.size(); i++)
            {
                if(isitequal(upper[i], s1))
                {
                    f1 = 1;
                }
                if(isitequal(upper[i],s2))
                {
                    f2 = 1;
                }
            }

            for(int i=0; i<lower.size(); i++)
            {
                if(isitequal(lower[i], s1))
                {
                    f1 = 1;
                }
                if(isitequal(lower[i],s2))
                {
                    f2 = 1;
                }
            }


            for(int i=0; i<middle.size(); i++)
            {
                if(isitequal(middle[i], s1))
                {
                    f1 = 1;
                }
                if(isitequal(middle[i],s2))
                {
                    f2 = 1;
                }
            }

            if(!f1)
            {
                n->data->middle.push_back(s1);
            }
            if(!f2)
            {
                n->data->middle.push_back(s2);
            }
        }
}

int isitvertical(segment* s1, segment* s2)
{
    float x1 = s1->p1->x;
    float x2 = s1->p2->x;

    float x3 = s2->p1->x;
    float x4 = s2->p2->x;
    if(x1==x2 && x3==x4)
    {
        return 3;
    }
    if(x1==x2)
    {
        return 1;
    }
    if(x3==x4)
    {
        return 2;
    }
    return 0;
}


void findnewevent(segment *s1, segment *s2, float cx, Point *pold)
{
    float x1 = s1->p1->x;
    float y1 = s1->p1->y;
    float m1 = findslope(s1);

    float x2 = s2->p1->x;
    float y2 = s2->p1->y;
    float m2 = findslope(s2);

    int v = isitvertical(s1, s2);


    
    if(v)
    {
        if(v==1)
        {
            handleverticalevent(s1,s2);
        }
        if(v==2)
        {
            handleverticalevent(s2,s1);
        }
        return;
    }

    if(m1==m2 || v==3)
    {
        return;
    }
    else
    {
        Point *p = new Point;
        float x = ((y2-(m2*x2))-(y1-(m1*x1)))/(m1-m2);
        float y = (m1*(y2-(m2*x2))-(m2*(y1-(m1*x1))))/(m1-m2);
        p->x = x;
        p->y = y;   
        if(y>sweepposition || (y==sweepposition && x<cx))
        {
            return;
        }

        if(y<s1->p2->y || y<s2->p2->y)
        {
            return;
        }
        if(y>s1->p1->y || y>s2->p1->y)
        {
            return;
        }

        if(eventtree.search(x,y)==NULL)
        {
            p->middle.push_back(s1);
            p->middle.push_back(s2);
            eventtree.insert(p);
        }
        else
        {
            Node *n = eventtree.search(x,y);
            int f1=0;
            int f2=0;
            vector<segment *> upper = n->data->upper;
            vector<segment *> lower = n->data->lower;
            vector<segment *> middle = n->data->middle;
            for(int i=0; i<upper.size(); i++)
            {
                if(isitequal(upper[i], s1))
                {
                    f1 = 1;
                }
                if(isitequal(upper[i],s2))
                {
                    f2 = 1;
                }
            }

            for(int i=0; i<lower.size(); i++)
            {
                if(isitequal(lower[i], s1))
                {
                    f1 = 1;
                }
                if(isitequal(lower[i],s2))
                {
                    f2 = 1;
                }
            }

            for(int i=0; i<middle.size(); i++)
            {
                if(isitequal(middle[i], s1))
                {
                    f1 = 1;
                }
                if(isitequal(middle[i],s2))
                {
                    f2 = 1;
                }
            }

            if(f1!=1)
            {
                n->data->middle.push_back(s1);
            }
            if(f2!=1)
            {
                n->data->middle.push_back(s2);
            }
        }
    }
    
}



int main()
{
    int n;
    cin>>n;

    vector<segment *> segments;
    vector<Point *> points;
    clock_t start;
    start = clock();


    ofstream visualizer;
    visualizer.open("seg.txt");

    int maxtemp  = INT_MIN;
    for(int i=0; i<n; i++)
    {
        float x1, y1, x2, y2;
        cin>>x1>>y1>>x2>>y2;
        visualizer<<x1<<" "<<x2<<" "<<y1<<" "<<y2;
        visualizer<<endl;

        if(y1>maxtemp)
        {
            maxtemp = y1;
        }
        if(y2>maxtemp)
        {
            maxtemp = y2;
        }

        Point *p1;
        Point *p2;
        if(eventtree.search(x1,y1))
        {
            p1 = eventtree.search(x1,y1)->data;
        }
        else
        {
        p1 = new Point;
        p1->x = x1;
        p1->y = y1;
        }


        if(eventtree.search(x2,y2))
        {
            p2 = eventtree.search(x2,y2)->data;
        }
        else
        {
            p2 = new Point;
            p2->x = x2; 
            p2->y = y2;
        }


        segment *s = new segment;
        
       /*  if(eventtree.search(p1))
        {
            p1 = eventtree.search(p1);
        } */
        /* if(eventtree.search(p2))
        {
            p2 = eventtree.search(p2);
        } */


        int temp = findupperpoint(p1, p2);   //returns 1 if p1 is the upper end point else 0

        
        if(temp==1)
        {
            s->p1 = p1;
            s->p2 = p2;
            p1->upper.push_back(s);
            p2->lower.push_back(s);
        }
        else
        {
            s->p1 = p2;
            s->p2 = p1;
            p1->lower.push_back(s);
            p2->upper.push_back(s);
        }
        segments.push_back(s);

        if(eventtree.search(p1->x, p1->y)==NULL)
        {
        eventtree.insert(p1);
        }
        if(eventtree.search(p2->x, p2->y)==NULL)
        {
        eventtree.insert(p2);
        }
    }


    sweepposition = maxtemp;

    ofstream myfile;
    myfile.open ("report.html");
    myfile << "<!DOCTYPE html><html><head><title>Report</title></head><body>"; 
    myfile<<"<p>"<<"Input: "<<n<<" segments"<<"</p>";
    myfile<<"<br>";


    while(!eventtree.isitempty())
    {
        Point *p = eventtree.pop();
        
        //statustree.inorder();
        //handle event pint

        vector<segment *> up;
        up = p->upper;
        vector<segment *> lp;
        lp = p->lower;
        vector<segment *> cp;
        cp = p->middle;


        if(up.size()+lp.size()+cp.size()>1)
        {
            cout<<"Intersection Point: x = "<<p->x<<"  &y = "<<p->y;
            myfile<<"<p>"<<"Intersection Point: x = "<<p->x<<"  &y = "<<p->y<<"</p>";
            cout<<endl;
            cout<<"Intersecting Segments: ";
            myfile<<"<p>"<<"Intersecting Segments: "<<"</p>";
            for(int i=0; i<up.size(); i++)
            {
                cout<<"("<<up[i]->p1->x<<", "<<up[i]->p1->y<<")";
                myfile<<"<p>"<<"("<<up[i]->p1->x<<", "<<up[i]->p1->y<<")";
                cout<<" to ";
                myfile<<"to ";
                cout<<"("<<up[i]->p2->x<<", "<<up[i]->p2->y<<")  ";
                myfile<<"<p>"<<"("<<up[i]->p2->x<<", "<<up[i]->p2->y<<")";
            }

            for(int i=0; i<lp.size(); i++)
            {
                cout<<"("<<lp[i]->p1->x<<", "<<lp[i]->p1->y<<")";
                myfile<<"<p>"<<"("<<lp[i]->p1->x<<", "<<lp[i]->p1->y<<")";
                cout<<" to ";
                myfile<<"to";
                cout<<"("<<lp[i]->p2->x<<", "<<lp[i]->p2->y<<")  ";
                myfile<<"("<<lp[i]->p2->x<<", "<<lp[i]->p2->y<<")  ";

                cout<<"     ";
            }

            for(int i=0; i<cp.size(); i++)
            {
                cout<<"("<<cp[i]->p1->x<<", "<<cp[i]->p1->y<<")";
                myfile<<"<p>"<<"("<<cp[i]->p1->x<<", "<<cp[i]->p1->y<<")";
                cout<<" to ";
                myfile<<"to";
                cout<<"("<<cp[i]->p2->x<<", "<<cp[i]->p2->y<<")  ";
                myfile<<"("<<cp[i]->p2->x<<", "<<cp[i]->p2->y<<")  ";
            }


            myfile<<"<br>";
            myfile<<"<br>";
        }
        
        sweepposition = (p->y)+0.00001;
        
        for(int i=0; i<cp.size();i++)
        {
            statustree.deleteNode(statustree.search(cp[i]));
        }
        for(int i=0; i<lp.size(); i++)
        {
            statustree.deleteNode(statustree.search(lp[i]));
        }

        sweepposition = (p->y)-0.01;

        for(int i=0; i<cp.size(); i++)
        {
            statustree.insert(cp[i]);
        }
        for(int i=0; i<up.size(); i++)
        {
            statustree.insert(up[i]);
        }


        if(up.size()+cp.size()==0)
        {   
            float leftmost_intercept = INT_MAX;
            float rightmost_intercept = INT_MIN;
            segment *rightmost = NULL;
            segment *leftmost = NULL;
            sweepposition = (p->y)+0.00001;
            for(int i=0; i<lp.size(); i++)
            {
                if(findintercept(lp[i])<leftmost_intercept)
                {
                    leftmost_intercept = findintercept(lp[i]);
                    leftmost = lp[i];
                }

                if(findintercept(lp[i])>rightmost_intercept)
                {
                    rightmost_intercept = findintercept(lp[i]);
                    rightmost = lp[i];
                }
            }

            if(leftmost!=NULL && rightmost!=NULL)
            {
                segNode *leftmost_node = statustree.search(leftmost);
                segNode *sl = NULL;
                if(leftmost_node != NULL)
                {
                    sl = statustree.predecessor(leftmost_node);
                }
                segNode *rightmost_node = statustree.search(rightmost);
                segNode *sr = NULL;
                if(rightmost_node != NULL)
                {
                    sr = statustree.successor(rightmost_node);
                }
                if(sl!=NULL && sr!=NULL)
                {
                    sweepposition = p->y;
                findnewevent(sl->data, sr->data,p->x, p);
                }
            }
        }
        else
        {
            float leftmost_intercept = INT_MAX;
            float rightmost_intercept = INT_MIN;
            segment *leftmost = NULL;
            segment *rightmost = NULL;
            cout<<endl;
            sweepposition = (p->y)-0.001;
            for(int i=0; i<up.size(); i++) 
            {

                if(findintercept(up[i])<leftmost_intercept)
                {
                    leftmost_intercept = findintercept(up[i]);
                    leftmost = up[i];
                }


                if(findintercept(up[i])>rightmost_intercept)
                {
                    rightmost_intercept = findintercept(up[i]);
                    rightmost = up[i];
                }
            }   
            for(int i=0; i<cp.size(); i++)
            {
                if(findintercept(cp[i])<leftmost_intercept)
                {
                    leftmost_intercept = findintercept(cp[i]);
                    leftmost = cp[i];
                }


                if(findintercept(cp[i])>rightmost_intercept)
                {
                    rightmost_intercept = findintercept(cp[i]);
                    rightmost = cp[i];
                }
            }

            if(leftmost!=NULL)
            {
                segNode* leftmost_node = statustree.search(leftmost);
                segNode* sl;
                if(leftmost_node!=NULL)
                {
                    sl = statustree.predecessor(leftmost_node);
                }
                sweepposition = p->y;
                if(sl!= NULL && leftmost_node!=NULL)
                {   
                    sweepposition = p->y;
                    findnewevent(sl->data, leftmost_node->data, p->x, p);
                }
            }
            if(rightmost!=NULL)
            { 
                segNode* rightmost_node = statustree.search(rightmost);
                segNode *sr;
                if(rightmost_node!=NULL)
                {
                    sr = statustree.successor(rightmost_node);
                }
                sweepposition = p->y;


                if(sr!=NULL && rightmost_node!=NULL)
                {
                sweepposition = p->y;
                findnewevent(sr->data, rightmost_node->data, p->x, p);
                } 
            }
        }
    }
    clock_t time_req;
    time_req = clock()-start;
    cout<<(double)time_req/CLOCKS_PER_SEC<<" seconds";
    myfile<<"<p>"<<"Time required:  "<<(double)time_req/CLOCKS_PER_SEC<<" seconds"<<"</p>";
    return 0;
}