#include "KL_E24066242.h"

using namespace std;

struct Node{
  string name;
  bool group;//two groups
  bool temp_group;
  map<Node*,string>all_edges;
  int external;
  int internal;
  int d_value;
};

KL::KL(char* benchmark){
  cost = 0;
  read_file(benchmark);
}
KL::~KL(){

  for(auto iter=all_nodes.begin();iter!=all_nodes.end();++iter){
    delete iter->second;
  }
}

void KL::read_file(char* benchmark){
  fstream nets_file,nodes_file;
  string str = benchmark;

  nets_file.open(str+".nets",ios::in);
  nodes_file.open(str+".nodes",ios::in);

  nodes_file>>str;
  numnodes = stoi(str.substr(9));//read in the total number of nodes
  int counter = 0;

  while(nodes_file>>str){
    //initialize the new node
    Node* n = new Node;
    n->name = str;
    n->external = 0;
    n->internal = 0;

    if(counter<numnodes/2){
      n->group = 0;//first group(first half)
      n->temp_group = 0;
      group0.push_back(n);
      temp_group0.push_back(n);
    }
    else{
      n->group = 1;//second group(last half)
      n->temp_group = 1;
      group1.push_back(n);
      temp_group1.push_back(n);
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

    n1->all_edges[n2]=str;
    n2->all_edges[n1]=str;

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
}

int KL::find_pair_to_swap(Node**n0,Node**n1){
  int max_cost = INT_MIN;
  list<Node*>::iterator swapped_iter0,swapped_iter1;

  for(auto iter0=temp_group0.begin();iter0!=temp_group0.end();++iter0){
    //cout<<iter0->second->name<<endl;
    for(auto iter1=temp_group1.begin();iter1!=temp_group1.end();++iter1){
      (*iter0)->d_value=(*iter0)->external-(*iter0)->internal;
      (*iter1)->d_value=(*iter1)->external-(*iter1)->internal;

      int g = (*iter0)->d_value+(*iter1)->d_value;

      if((*iter0)->all_edges.find(*iter1)!=(*iter0)->all_edges.end()){
        g-=2;//if two nodes are connected
      }
      if(g>max_cost){
        swapped_iter0 = iter0;
        swapped_iter1 = iter1;
        max_cost=g;
      }
    }
  }
  *n0=*swapped_iter0;
  *n1=*swapped_iter1;
  cout<<"swap "<<(*n0)->name<<" and "<<(*n1)->name<<endl;
  update_Dvalue(*n0,*n1);
  temp_group0.erase(swapped_iter0);
  temp_group1.erase(swapped_iter1);
  //lock_pair(swapped_iter0,swapped_iter1);//actually just erase the swapped nodes in temp_gruop list
  return max_cost;
}

void KL::lock_pair(list<Node*>::iterator iter0,list<Node*>::iterator iter1){
  update_temp_Dvalue(*iter0,*iter1);
  temp_group0.erase(iter0);
  temp_group1.erase(iter1);
}
void KL::update_temp_Dvalue(Node*n0,Node*n1){
  n0->d_value=-n0->d_value;
  n1->d_value=-n1->d_value;
  for(auto iter=n0->all_edges.begin();iter!=n0->all_edges.end();++iter){
    if(iter->first==n1){n0->d_value+=2;continue;}
    if(iter->first->temp_group==n0->temp_group){
      iter->first->d_value+=2;
    }
    else{
      iter->first->d_value-=2;
    }
  }
  for(auto iter=n1->all_edges.begin();iter!=n1->all_edges.end();++iter){
    if(iter->first==n0){n1->d_value+=2;continue;}
    if(iter->first->temp_group==n1->temp_group){
      iter->first->d_value+=2;
    }
    else{
      iter->first->d_value-=2;
    }
  }
n0->temp_group=!(n0->temp_group);
n1->temp_group=!(n1->temp_group);
}

void KL::update_Dvalue(Node*n0,Node*n1){
  swap(n0->external,n0->internal);
  swap(n1->external,n1->internal);

  for(auto iter=n0->all_edges.begin();iter!=n0->all_edges.end();++iter){
    Node* n=iter->first;
    if(n==n1){
      n0->external+=1;
      n0->internal-=1;
      continue;
    }
    if(n->group==n0->group){
      ++n->external;
      --n->internal;
    }
    else{
      --n->external;
      ++n->internal;
    }
  }
  for(auto iter=n1->all_edges.begin();iter!=n1->all_edges.end();++iter){
    Node* n=iter->first;
    if(n==n0){
      n1->external+=1;
      n1->internal-=1;
      continue;
    }
    if(n->group==n1->group){
      ++n->external;
      --n->internal;
    }
    else{
      --n->external;
      ++n->internal;
    }
  }
  n0->group=!(n0->group);
  n1->group=!(n1->group);
}

bool KL::max_cost(){
  cout<<"first_cutsize:"<<cost<<endl;
  vector<int>g;
  vector<pair<Node*,Node*>>candidates;
  for(int i=0;i< numnodes/2;++i){
    Node* n0;
    Node* n1;
    int max_cost = find_pair_to_swap(&n0,&n1);
    //cout<<n1->name;
    cout<<"stage "<<i+1<<" max cost :"<<max_cost<<endl;
    g.push_back(max_cost);
    candidates.push_back(make_pair(n0,n1));
  }
  int max_partial_sum = 0;
  int sum = 0;
  int index=0;
  for(int i=0;i<g.size();++i){
    sum+=g.at(i);
    if(sum>max_partial_sum){
      max_partial_sum=sum;
      index=i+1;
    }
  }
  if(max_partial_sum>0){
    cout<<"cost reduce: "<<max_partial_sum<<endl;
    cout<<endl;
    cost -= max_partial_sum;
    for(int i=0;i<index;++i){
      //cout<<candidates.at(i).first<<endl;
      cout<<"actually swap: "<<candidates.at(i).first->name<<' '<<candidates.at(i).second->name<<endl;
      update_Dvalue(candidates.at(i).first,candidates.at(i).second);
      swap(*(candidates.at(i).first),*(candidates.at(i).second));
    }
    return 1;
  }
  else{return 0;}
}

void KL::update_temp(){
  for(auto iter = all_nodes.begin();iter!=all_nodes.end();++iter){
    iter->second->d_value=iter->second->external - iter->second->internal;
    iter->second->temp_group=iter->second->group;
  }
  temp_group0=group0;
  temp_group1=group1;
}

void KL::check(){
  for(auto iter = all_nodes.begin();iter!=all_nodes.end();++iter){
    Node* n=iter->second;
    cout<<n->name<<' '<<n->internal<<' '<<n->external<<endl;
  }
}

void KL::result(){
  cout<<"final_cut_size: "<<cost<<endl;
}
