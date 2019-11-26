#include "KL_E24066242.h"

using namespace std;

struct Node{
  string name;
  bool group;//two groups
  bool temp_group;
  map<Node*,int>all_edges;
  int external;
  int internal;
  int d_value;
};

KL::KL(char* benchmark){
  cost = 0;
  read_file(benchmark);
  //header0 = temp_group0.begin();
  //header1 = temp_group1.begin();
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
    n->d_value=0;

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
    if(n1->all_edges.find(n2)==n1->all_edges.end()){
      //weighted edge that has not existed before
      n1->all_edges[n2]=1;
      n2->all_edges[n1]=1;
    }
    else{
      ++(n1->all_edges[n2]);
      ++(n2->all_edges[n1]);
    }

    if(n2->group==n1->group){
      ++(n1->internal); --(n1->d_value);
      ++(n2->internal); --(n2->d_value);
    }
    else{
      ++(n1->external); ++(n1->d_value);
      ++(n2->external); ++(n2->d_value);
      ++cost;
    }
  }
}

pair<it,it> KL::priority(it new0,it new1, it old0, it old1){
  int new_0 = stoi(((*new0)->name).substr(1));
  int new_1 = stoi(((*new1)->name).substr(1));
  int old_0 = stoi(((*old0)->name).substr(1));
  int old_1 = stoi(((*old1)->name).substr(1));
  pair<it,it>p;

  if(new_0<old_0){
    p.first=new0;
    p.second=new1;
  }
  else if(new_0>old_0){
    p.first=old0;
    p.second=old1;
  }
  else{
    if(new_0<old_0){
      p.first=new0;
      p.second=new1;
    }
    else{
      p.first=old0;
      p.second=old1;
    }
  }
  return p;
}
void KL::f(){
  int num=0;
  int cut1=0;
  int cut2=0;
  for(auto iter = group0.begin();iter!=group0.end();++iter){
    cut1+=(*iter)->external;
  }

  for(auto iter = group1.begin();iter!=group1.end();++iter){
    cut2+=(*iter)->external;
  }

  cout<<"cut1: "<<cut1<<" cut2: "<<cut2<<' '<<"cost: "<<cost<<endl;
}
int KL::find_pair_to_swap(Node**n0,Node**n1){
  //check();
  int max_cost = INT_MIN;
  list<Node*>::iterator swapped_iter0,swapped_iter1;
  //if(temp_group0.size()!=3){cout<<"fuckkkkkkkkkkkkkkkk"<<endl;}
  for(auto iter0=temp_group0.begin();iter0!=temp_group0.end();++iter0){
    for(auto iter1=temp_group1.begin();iter1!=temp_group1.end();++iter1){

      int g = (*iter0)->d_value+(*iter1)->d_value;
      auto target=(*iter0)->all_edges.find(*iter1);
      if(target!=(*iter0)->all_edges.end()){
        g-=2*(target->second);//if two nodes are connected
      }
      //cout<<"g for "<<(*iter0)->name<<(*iter1)->name<<' '<<g<<endl;
      if(g==max_cost){
        //cout<<"compare"<<(*iter0)->name<<(*iter1)->name<<' '<<(*swapped_iter0)->name<<(*swapped_iter1)->name<<endl;
        pair<it,it>p=priority(iter0,iter1,swapped_iter0,swapped_iter1);
        swapped_iter0 = p.first;
        swapped_iter1 = p.second;
      }
      else if(g>max_cost){
        swapped_iter0 = iter0;
        swapped_iter1 = iter1;
        max_cost=g;
      }
      else{
        //just don't swap
      }
    }
  }
  *n0=*swapped_iter0;
  *n1=*swapped_iter1;
  cout<<"swap "<<(*n0)->name<<" and "<<(*n1)->name<<endl;
  cout<<"max cost reduce: "<<max_cost<<endl;
  update_temp_Dvalue(*n0,*n1);
  //check();
  //check(1);
  temp_group0.erase(swapped_iter0);
  temp_group1.erase(swapped_iter1);
  return max_cost;
}

void KL::update_temp_Dvalue(Node*n0,Node*n1){
  //don't change external and internal value,just update d_value temporarily
  n0->d_value=-n0->d_value;
  n1->d_value=-n1->d_value;
  //update the d_value and group of nodes that connected with n0
  for(auto iter=n0->all_edges.begin();iter!=n0->all_edges.end();++iter){
    //if(iter->first==n1){n0->d_value+=2;continue;}
    if(iter->first->temp_group==n0->temp_group){
      iter->first->d_value+=2*(iter->second);
    }
    else{
      iter->first->d_value-=2*(iter->second);
    }
  }
  //update the d_value and group of nodes that connected with n1
  for(auto iter=n1->all_edges.begin();iter!=n1->all_edges.end();++iter){
    //if(iter->first==n0){n1->d_value+=2;continue;}
    if(iter->first->temp_group==n1->temp_group){
      iter->first->d_value+=2*(iter->second);
    }
    else{
      iter->first->d_value-=2*(iter->second);
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
      n0->external+=iter->second;
      n0->internal-=iter->second;
      continue;
    }
    if(n->group==n0->group){
      n->external+=iter->second;
      n->internal-=iter->second;
    }
    else{
      n->external-=iter->second;
      n->internal+=iter->second;
    }
  }
  for(auto iter=n1->all_edges.begin();iter!=n1->all_edges.end();++iter){
    Node* n=iter->first;
    if(n==n0){
      n1->external+=iter->second;
      n1->internal-=iter->second;
      continue;
    }
    if(n->group==n1->group){
      n->external+=iter->second;
      n->internal-=iter->second;
    }
    else{
      n->external-=iter->second;
      n->internal+=iter->second;
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

void KL::check(bool temp){
  /*
  for(auto iter = all_nodes.begin();iter!=all_nodes.end();++iter){
    Node* n=iter->second;
    cout<<n->name<<' '<<n->internal<<' '<<n->external<<endl;
  }*/
  if(!temp){
    cout<<"Group0: "<<endl;
    for(auto iter = group0.begin();iter!=group0.end();++iter){
      cout<<(*iter)->name<<' '<<(*iter)->group<<' '<<(*iter)->internal<<' '<<(*iter)->external<<' '<<(*iter)->d_value<<endl;
    }
    cout<<"Group1 "<<endl;
    for(auto iter = group1.begin();iter!=group1.end();++iter){
      cout<<(*iter)->name<<' '<<(*iter)->group<<' '<<(*iter)->internal<<' '<<(*iter)->external<<' '<<(*iter)->d_value<<endl;
    }
  }
  else{
    cout<<"temp_Group0: "<<endl;
    for(auto iter = temp_group0.begin();iter!=temp_group0.end();++iter){
      cout<<(*iter)->name<<' '<<(*iter)->temp_group<<' '<<(*iter)->internal<<' '<<(*iter)->external<<' '<<(*iter)->d_value<<endl;
    }
    cout<<"temp_Group1 "<<endl;
    for(auto iter = temp_group1.begin();iter!=temp_group1.end();++iter){
      cout<<(*iter)->name<<' '<<(*iter)->temp_group<<' '<<(*iter)->internal<<' '<<(*iter)->external<<' '<<(*iter)->d_value<<endl;
    }
  }
}

void KL::result(){
  cout<<"final_cut_size: "<<cost<<endl;
}
