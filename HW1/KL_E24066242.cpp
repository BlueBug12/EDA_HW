#include "KL_E24066242.h"

using namespace std;

struct Node{
  string name;
  bool group;//two groups
  bool temp_group;
  map<Node*,pair<vector<string>,int>>all_edges;
  int external;
  int internal;
  int d_value;
};

KL::KL(const string benchmark){
  cost = 0;
  read_file(benchmark);
}

KL::~KL(){
  for(auto iter=all_nodes.begin();iter!=all_nodes.end();++iter){
    delete iter->second;
  }
}

void KL::read_file(const string filename){
  fstream nets_file,nodes_file;
  nets_file.open(filename+".nets",ios::in);
  nodes_file.open(filename+".nodes",ios::in);
  string str;
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
  while (nets_file>>str){
    nets_file>>name1>>name2;

    Node* n1 = all_nodes[name1];
    Node* n2 = all_nodes[name2];
    if(n1->all_edges.find(n2)==n1->all_edges.end()){
      //weighted edge that has not existed before
      vector<string>str_list{str};
      pair<vector<string>,int>p(str_list,1);
      n1->all_edges[n2]=p;
      n2->all_edges[n1]=p;
    }
    else{
      n1->all_edges[n2].first.push_back(str);
      n2->all_edges[n1].first.push_back(str);
      ++(n1->all_edges[n2].second);
      ++(n2->all_edges[n1].second);
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
  first_cost=cost;
  cout<<"first_cutsize:"<<first_cost<<endl;
}

void KL::write_file(const string filename,const double runtime){
  fstream outfile;
  outfile.open(filename+".out",ios::out);
  string str;
  outfile<<"runtime : "<<runtime<<" s"<<endl;
  outfile<<"first_cutsize : "<<first_cost<<endl;
  outfile<<"final_cutsize : "<<cost<<endl<<endl;
  vector<int>result;
  for(auto iter = group0.begin();iter!=group0.end();++iter){
    result.push_back(stoi((*iter)->name.substr(1)));
  }
  sort(result.begin(),result.end());
  outfile<<"G1 :"<<endl;
  for(int i=0;i<result.size();++i){
    outfile<<'o'<<result.at(i)<<' ';
  }
  outfile<<';'<<endl<<endl;
  result.clear();
  vector<int>nets;
  for(auto iter = group1.begin();iter!=group1.end();++iter){
    result.push_back(stoi((*iter)->name.substr(1)));
    for(auto iter2=(*iter)->all_edges.begin();iter2!=(*iter)->all_edges.end();++iter2){
      if(iter2->first->group!=(*iter)->group){
        for(int i=0;i<iter2->second.first.size();++i){
          nets.push_back(stoi(iter2->second.first.at(i).substr(1)));
        }
      }
    }
  }

  sort(result.begin(),result.end());
  outfile<<"G2 :"<<endl;
  for(int i=0;i<result.size();++i){
    outfile<<'o'<<result.at(i)<<' ';
  }
  outfile<<';'<<endl<<endl<<"cutset :"<<endl;;

  sort(nets.begin(),nets.end());
  for(int i=0;i<nets.size();++i){
    outfile<<'n'<<nets.at(i)<<' ';
  }
  outfile<<';'<<endl;
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
    if(new_1<old_1){
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

int KL::find_pair_to_swap(Node**n0,Node**n1){
  int max_cost = INT_MIN;
  list<Node*>::iterator swapped_iter0,swapped_iter1;
  for(auto iter0=temp_group0.begin();iter0!=temp_group0.end();++iter0){
    for(auto iter1=temp_group1.begin();iter1!=temp_group1.end();++iter1){

      int g = (*iter0)->d_value+(*iter1)->d_value;
      int test =g;
      auto target=(*iter0)->all_edges.find(*iter1);
      if(target!=(*iter0)->all_edges.end()){
        g-=2*(target->second.second);//if two nodes are connected
      }
      //cout<<"g for "<<(*iter0)->name<<(*iter1)->name<<' '<<g<<" pre "<<test<<endl;
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
  pre_swap(*n0,*n1);

  temp_group0.erase(swapped_iter0);
  temp_group1.erase(swapped_iter1);
  return max_cost;
}

void KL::pre_swap(Node*n0,Node*n1){
  //don't change external and internal value,just update d_value temporarily
  n0->d_value=-n0->d_value;
  n1->d_value=-n1->d_value;
  //update the d_value and group of nodes that connected with n0
  for(auto iter=n0->all_edges.begin();iter!=n0->all_edges.end();++iter){
    if(iter->first->temp_group==n0->temp_group){
      iter->first->d_value+=2*(iter->second.second);
    }
    else{
      iter->first->d_value-=2*(iter->second.second);
    }
  }
  //update the d_value and group of nodes that connected with n1
  for(auto iter=n1->all_edges.begin();iter!=n1->all_edges.end();++iter){
    if(iter->first->temp_group==n1->temp_group){
      iter->first->d_value+=2*(iter->second.second);
    }
    else{
      iter->first->d_value-=2*(iter->second.second);
    }
  }
n0->temp_group=!(n0->temp_group);
n1->temp_group=!(n1->temp_group);
}

void KL::swap_nodes(Node*n0,Node*n1){
  swap(n0->external,n0->internal);
  swap(n1->external,n1->internal);

  for(auto iter=n0->all_edges.begin();iter!=n0->all_edges.end();++iter){
    Node* n=iter->first;
    if(n==n1){
      n0->external+=iter->second.second;
      n0->internal-=iter->second.second;
      continue;
    }
    if(n->group==n0->group){
      n->external+=iter->second.second;
      n->internal-=iter->second.second;
    }
    else{
      n->external-=iter->second.second;
      n->internal+=iter->second.second;
    }
  }
  for(auto iter=n1->all_edges.begin();iter!=n1->all_edges.end();++iter){
    Node* n=iter->first;
    if(n==n0){
      n1->external+=iter->second.second;
      n1->internal-=iter->second.second;
      continue;
    }
    if(n->group==n1->group){
      n->external+=iter->second.second;
      n->internal-=iter->second.second;
    }
    else{
      n->external-=iter->second.second;
      n->internal+=iter->second.second;
    }
  }
  n0->group=!(n0->group);
  n1->group=!(n1->group);

  for(auto iter = group0.begin();iter!=group0.end();++iter){
    if(*iter==n0){
      *iter=n1;
      break;
    }
  }

  for(auto iter = group1.begin();iter!=group1.end();++iter){
    if(*iter==n1){
      *iter=n0;
      break;
    }
  }
}

bool KL::max_cost(){

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
    cost -= max_partial_sum;
    for(int i=0;i<index;++i){
      swap_nodes(candidates.at(i).first,candidates.at(i).second);
    }
    return 1;
  }
  else{return 0;}
}

void KL::update_new_group(){
  for(auto iter = all_nodes.begin();iter!=all_nodes.end();++iter){
    iter->second->d_value=iter->second->external - iter->second->internal;
    iter->second->temp_group=iter->second->group;
  }
  temp_group0=group0;
  temp_group1=group1;
}

void KL::result(){
  cout<<"final_cut_size: "<<cost<<endl;
}
