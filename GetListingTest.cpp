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
    Node* root=NULL;
    map<string,Node*> a;
    a=createMap(l);
    
    for (map<string,Node*>::iterator  it = a.begin(); it!=a.end() ; it++)
    {
      cout<<it->first<<endl;        
    }
    for (map<string,Node*>::iterator  it = a.begin(); it!=a.end() ; it++)
    {
      it->second=root;       
    }    
    buildTree(l,a);//seems the buildTree function doesn't work
    for (map<string,Node*>::iterator  it = a.begin(); it!=a.end() ; it++)
    {
      cout<<it->second->list.rating<<endl;        
    }
    return 0;

}