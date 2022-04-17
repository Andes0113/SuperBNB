#ifndef Avltree_hpp
#define Avltree_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include "Listing.hpp"
#endif





class Node
{
public:
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

Node *newNode(Listing list) {
  Node *node = new Node();
  node->list=list;
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



///// what is return value 
Listing search(Node *root,double rating1,double rating2) {
    // If root is NULL
    if (root == NULL)
        return ;
    // If found, return 
    if ( rating1 < root->list.rating )
        search(root->left, rating1, rating2);
     
    /* if root's data lies in range,
    then prints root's data */
    if ( rating1 <= root->list.rating && rating2 >= root->list.rating )
        return root->list;
     
    /* recursively call the right subtree */
   search(root->right, rating1, rating2);
}


///// Search by multiple criterions
Listing searchByPrice(Node *root,double price1, double price2) {


}
