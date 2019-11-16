#include "KL_E24066242.h"

using namespace std;
struct Node;
typedef pair<string,Node*>edge;//edge name and connected node
struct Node{
  string name;
  bool group;//two groups
  bool locked;
  vector<edge*>all_edges;//using vector like adjacent list
  int external;
  int internal;
};

KL::KL(char* benchmark){
  cost = 0;
  read_file(benchmark);
}
KL::~KL(){
  for(auto iter=all_nodes.begin();iter!=all_nodes.end();++iter){
    for(int i=0;i<iter->second->all_edges.size();++i){
      delete iter->second->all_edges.at(i);
    }
    delete iter->second;
  }
}

void KL::read_file(char* benchmark){
  fstream nets_file,nodes_file;
  string str = benchmark;

  nets_file.open(str+".nets",ios::in);
  nodes_file.open(str+".nodes",ios::in);

  nodes_file>>str;
  //cout<<str.substr(9)<<endl;//read in
  numnodes = stoi(str.substr(9));//read in the total number of nodes
  int counter = 0;

  while(nodes_file>>str){
    //initialize the new node
    Node* n = new Node;
    n->name = str;
    n->lucked = 0;
    n->external = 0;
    n->internal = 0;
    if(counter<numnodes/2){
      n->group = 0;//first group(first half)
      group0[str] = n;
    }
    else{
      n->group = 1;//second group(last half)
      group1[str] = n;
    }
    all_nodes[str] = n;
    ++counter;
  }

  string name1,name2;
  nets_file>>str;//avoid read in first line(numnets)
  while (nets_file>>str) {
    nets_file>>name1>>name2;

    Node* n1 = all_nodes[name1];
    Node* n2 = all_nodes[name2];

    edge* e1 = new edge(str,n2);
    edge* e2 = new edge(str,n1);
    n1->all_edges.push_back(e1);
    n2->all_edges.push_back(e2);

    if(n2->group==n1->group){
      ++(n1->internal);
      ++(n2->internal);
    }
    else{
      ++(n1->external);
      ++(n2->external);
      ++cost;
    }
  }
  cout<<"first_cutsize:"<<cost<<endl;
}

void KL::find_pair_to_swap(){
  int max_cost=-1;
  Node* n0 = group0.begin()->second;
  Node* n1 = group1.begin()->second;
  for(auto iter0=group0.begin();iter0!=group0.end();++iter0){
    //cout<<iter0->second->name<<endl;
    for(auto iter1=group1.begin();iter1!=group1.end();++iter1){
      int g = iter0->second->external+iter1->second->external
             -iter0->second->internal-iter1->second->internal;//Da + Db
      for(int i=0;i<iter0->second->all_edges.size();++i){

        if(iter0->second->all_edges.at(i)->second==iter1->second){
          //if two nodes are connected
          g-=2;
          break;
        }
      }

      if(g>=max_cost||(g==max_cost&&stoi(iter1->second->name.substr(1))<=stoi(n1->name.substr(1))
        &&stoi(iter0->second->name.substr(1))<=stoi(n0->name.substr(1)))){

        n0=iter0->second;
        n1=iter1->second;
        //cout<<n1->name<<' '<<n0->name<<' '<<g<<endl;
        max_cost=g;
      }
    }
  }
  cout<<max_cost<<endl;

}

void KL::swap_pair(map<string, Node*>::iterator iter1,map<string, Node*>::iterator iter2){
  move_out(iter1->second);
  move_out(iter2->second);
  swap(*iter1,*iter2);//to be confirmed
}

void KL::move_out(Node*n){
  n->locked = 1;
  for(int i=0;i<n->all_edges.size();++i){
    //update new D' value
    if(n->all_edges.at(i)->seond->group==n->group){
      ++(n->all_edges.at(i)->second->external);
      --(n->all_edges.at(i)->second->internal);
    }
    else{
      --(n->all_edges.at(i)->second->external);
      ++(n->all_edges.at(i)->second->internal);
    }
  }
}
