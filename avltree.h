#include <map>
#include <stack>
struct Node
{
    Listing list;
    Node* left=nullptr;
    Node* right=nullptr;
    int height;
};

int height(Node *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

Node *newNode(Listing newlist) {
  Node *node = new Node();
  node->list=newlist;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  return (node);
}

// Rotate right
Node *rightRotate(Node *y) {
  Node *x = y->left;
  Node *T2 = x->right;
  x->right = y;
  y->left = T2;
  y->height = std::max(height(y->left),
          height(y->right)) + 1;
  x->height = std::max(height(x->left),
          height(x->right)) + 1;
  return x;
}

// Rotate left
Node *leftRotate(Node *x) {
  Node *y = x->right;
  Node *T2 = y->left;
  y->left = x;
  x->right = T2;
  x->height = std::max(height(x->left),
          height(x->right)) +
        1;
  y->height = std::max(height(y->left),
          height(y->right)) +
        1;
  return y;
}

int getBalanceFactor(Node *N) {
  if (N == NULL)
    return 0;
  return height(N->left) -
       height(N->right);
}

Node *insertNode(Node *node, Listing newlist) {
  // Find the correct postion and insert the node
  if (node == NULL)
    return (newNode(newlist));

  if (newlist.rating < node->list.rating)
    node->left = insertNode(node->left,newlist);
  else if (newlist.rating > node->list.rating)
    node->right = insertNode(node->right, newlist);
  else
    {
      return node;}

  node->height = 1 + std::max(height(node->left),
               height(node->right));
  int balanceFactor = getBalanceFactor(node);
  if (balanceFactor > 1) {
    if (newlist.rating < node->left->list.rating) {
      return rightRotate(node);
    } else if (newlist.rating > node->left->list.rating) {
      node->left = leftRotate(node->left);
      return rightRotate(node);
    }
  }
  if (balanceFactor < -1) {
    if (newlist.rating > node->right->list.rating) {
      return leftRotate(node);
    } else if (newlist.rating < node->right->list.rating) {
      node->right = rightRotate(node->right);
      return leftRotate(node);
    }
  }
  return node;
}


std::vector<Listing> search(Node *root,double rating1,double rating2, double price1, double price2) {
    std::stack<Node*> s;
    Node *curr=root;
    std::vector<Listing> newListing;

    while (curr != NULL || s.empty() == false)
    {
        /* Reach the left most Node of the
           curr Node */
        while (curr !=  NULL)
        {
            /* place pointer to a tree node on
               the stack before traversing
              the node's left subtree */
            s.push(curr);
            curr = curr->left;
        }

        /* Current must be NULL at this point */
        curr = s.top();
        s.pop();
        if(curr->list.rating>=rating1&&curr->list.rating<=rating2&&
        curr->list.price>=price1&&curr->list.price<=price2){
          newListing.push_back(curr->list);
        }
 
        /* we have visited the node and its
           left subtree.  Now, it's right
           subtree's turn */
        curr = curr->right;
 
    }
    return newListing;
}



std::map<std::string,Node*> createMap( std::vector<Listing> l){
    std::map<std::string,Node*> mymap;
    for (std::vector<Listing>::iterator  it = l.begin(); it!=l.end() ; it++)
    {
      mymap.insert(std::make_pair(it->neighborhood, nullptr));        
    }
    return mymap;
 }



Node* buildTree(std::vector<Listing> L, Node* head){
  for(int i = 0; i < L.size(); i++){
    head = insertNode(head, L.at(i));
  }
  return head;
}

class AVLTree{
  public:
  Node* head;
  AVLTree(vector<Listing> l){
    head=nullptr;
    head = buildTree(l, head);
  }
  AVLTree(){
    head=nullptr;
  };
  void insertListing(Listing l){
    head = insertNode(head, l);
  }
};

