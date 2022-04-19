#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "GetListings.h"
#include "avltree.h"

using namespace std;

int main()
{ 
  vector<Listing> l;
  l=GetListings();

  //Tree for the whole listings
  AVLTree fullTree(l);
  //Tree for look up
  map<string, AVLTree> treeLookup;

  for (vector<Listing>:: iterator it= l.begin(); it!=l.end() ; it++)
  {
    if( treeLookup.count(it->neighborhood) == 0)//not found
    {
      vector<Listing> newListing;
      newListing.push_back(*it);
      AVLTree newTree = AVLTree(newListing);
      treeLookup[it->neighborhood] = newTree;
    }
    else{
      treeLookup[it->neighborhood].insertListing(*it);
    }
  }

  

  //cout<<treeLookup["Kensington"].head->right->list.rating;


  //test example for search function
  vector<Listing> b;  
  b=search(treeLookup["Kensington"].head,0.0,8.0,10,300);
  for (vector<Listing>:: iterator it= b.begin(); it!=b.end() ; it++)
  {
    cout<<it->rating<<endl;
  }
  
  return 0;

}