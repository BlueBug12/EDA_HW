#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <climits>

using namespace std;
struct edge;
struct node{
  string logic;
  edge* parent;
  edge* left_child;
  edge* right_child;
};

struct edge{
  string name;
  node* from;
  node* to;
};

class Tree{
public:
  Tree(string pattern_name,vector<string>i_ports,string o_port){
    name=pattern_name;
    edge* temp;
    for(int i=0;i<i_ports.size();++i){
      temp=new edge;
      temp->name=i_ports.at(i);
      temp->from=NULL;
      temp->to=NULL;
      leaves.push_back(temp);
      ports[i_ports.at(i)]=temp;
    }
    root=new edge;
    root->name=o_port;
    root->from=NULL;
    root->to=NULL;
    ports[o_port]=root;
    cost=0;
    size=0;
  }
  ~Tree(){
    node* start=root->from;
    delete root;
    queue<node*>q;
    q.push(start);
    while(!q.empty()){
      start=q.front();
      q.pop();
      if(start->left_child!=NULL){
        if(start->left_child->from!=NULL){
          q.push(start->left_child->from);
        }
        delete start->left_child;
      }

      if(start->right_child!=NULL){
        if(start->right_child->from!=NULL){
          q.push(start->right_child->from);
        }
        delete start->right_child;
      }
      delete start;
    }
  }



  bool rotate(node** n1,node**n2){

    if((*n1)->parent->to==NULL){return 0;}
    if((*n2)->parent->to==NULL){return 0;}
    *n1=(*n1)->parent->to;
    *n2=(*n2)->parent->to;
    if((*n1)->logic=="NOT"){return 0;}
    swap((*n1)->left_child,(*n1)->right_child);
    return 1;
  }

  vector<edge*> matching(node* n){
    vector<edge*>ans;

    matching(n,root->from,0,ans);
    if(ans.size()!=leaves.size()){
      ans.clear();
    }
    return ans;
  }

  bool matching(node* n1,node* n2,bool left,vector<edge*>&ans){

    int flag=1;

    if(n1->logic!=n2->logic){
      if(left&&rotate(&n1,&n2)){flag=0;}
      else{return 0;}

    }


    if(n1->left_child!=NULL&&n2->left_child!=NULL){
      if(n1->right_child!=NULL&&n2->right_child!=NULL&&(n1->left_child->from==NULL&&n2->right_child->from==NULL||
      n1->right_child->from==NULL&&n2->left_child->from==NULL)){
        if(n1->logic!="NOT"){swap(n1->left_child,n1->right_child);}
      }

      if(n2->left_child->from==NULL){ans.push_back(n1->left_child);}
      else if(n1->left_child->from!=NULL){
        matching(n1->left_child->from,n2->left_child->from,flag,ans);

      }
      else{return 0;}

    }

    if(n1->right_child!=NULL&&n2->right_child!=NULL){
      if(n2->right_child->from==NULL){if(flag){ans.push_back(n1->right_child);}}
      else if(n1->right_child->from!=NULL){
        matching(n1->right_child->from,n2->right_child->from,0,ans);
      }
      else{return 0;}

    }

  }

  edge* root_edge(){return root;}
  void build_port(string port,node* n,bool from){
    edge* e;
    auto it = ports.find(port);//map<string,node*>::iterator
    if (it == ports.end()){//new port name
      e=new edge;
      e->name=port;
      if(from){
        e->from=n;
        e->to=NULL;
        n->parent=e;
      }
      else{
        e->from=NULL;
        e->to=n;
        if(n->left_child==NULL)
          n->left_child=e;
        else
          n->right_child=e;
      }
      ports[port]=e;
    }

    else{
      e=it->second;
      if(from){
        e->from=n;
        n->parent=e;
      }
      else{
        e->to=n;
        if(n->left_child==NULL)
          n->left_child=e;
        else
          n->right_child=e;
      }
      ports.erase(it);
    }
  }

  void add_node(string o_port,string i_port1,string i_port2,string name){
    node* n=new node;
    n->logic=name;
    n->parent=NULL;
    n->left_child=NULL;
    n->right_child=NULL;
    build_port(o_port,n,1);
    build_port(i_port1,n,0);

    if(i_port2!="")
      build_port(i_port2,n,0);
    else
      n->right_child=NULL;

    ++size;
  }

  void set_cost(string c){cost=stoi(c);}
  int get_cost(){return cost;}

  void print_name(){cout<<name<<endl;}

private:
  edge* root;
  vector<edge*>leaves;
  int cost;
  int size;
  string name;
  map<string,edge*>ports;
};

int mincost(edge* top,vector<Tree*>&all_patterns){
  if(top->from==NULL){return 0;}
  int cost=INT_MAX;
  int newcost;
  vector<edge*>next;
  for(int i=0;i<all_patterns.size();++i){
    next=all_patterns.at(i)->matching(top->from);
    if(next.size()!=0){
      newcost=all_patterns.at(i)->get_cost();

      for(int j=0;j<next.size();++j){
        newcost+=mincost(next.at(j),all_patterns);
      }
      if(newcost<cost){
        cost=newcost;
      }
    }
  }
  return cost;
}

int main(int argc, char *argv[]){

  fstream file;
  string pattern_file=argv[1];
  string circuit_file=argv[2];
  vector<Tree*>all_patterns;
  file.open(pattern_file+".v",ios::in);
  if(!file){
    cout<<"Wrong file name!"<<endl;
    exit(1);
  }

  string str;
  while(file>>str){
    //build all patterns
    if(str=="module"){
      map<string,node*>ports;
      vector<string>i_ports;
      string name;
      file>>name;
      //cout<<name<<endl;
      file>>str;


      while(file>>str,str!="output"){//input ports
        i_ports.push_back(str.substr(0,str.find(',',0)));
      }

      //output ports
      file>>str;
      string o_port=str.substr(0,str.find(')',0));
      Tree* t=new Tree(name,i_ports,o_port);

      while(file>>str,str.size()<8){
        string logic=str;
        string port1,port2,port3;

        file>>str;
        port1=str.substr(str.find('(')+1,str.size()-str.find('(')-2);

        if(logic=="NOT"){
          file>>str;
          port2=str.substr(0,str.size()-2);
          port3="";
        }
        else{
          file>>str;
          port2=str.substr(0,str.size()-1);
          file>>str;
          port3=str.substr(0,str.size()-2);
        }
        t->add_node(port1,port2,port3,logic);
      }
      t->set_cost(str.substr(str.find('=')+1,str.size()-str.find('=')-2));
      file>>str;
      all_patterns.push_back(t);
    }

  }
  file.close();

  file.open(circuit_file+".v",ios::in);
  if(!file){
    cout<<"Wrong file name!"<<endl;
    exit(1);
  }


  file>>str;
  map<string,node*>ports;
  vector<string>i_ports;
  string name;
  file>>name;
  file>>str;

  //read target circuit
  while(file>>str,str!="output"){//input ports
    i_ports.push_back(str.substr(0,str.find(',',0)));
  }

  //output ports
  file>>str;
  string o_port=str.substr(0,str.find(')',0));
  Tree* target=new Tree(name,i_ports,o_port);
  file>>str;
  getline(file,str);

  while(file>>str,str!="endmodule"){
    string logic=str;
    string port1,port2,port3;

    file>>str;
    port1=str.substr(str.find('(')+1,str.size()-str.find('(')-2);

    if(logic=="NOT"){
      file>>str;
      port2=str.substr(0,str.size()-2);
      port3="";
    }
    else{
      file>>str;
      port2=str.substr(0,str.size()-1);
      file>>str;
      port3=str.substr(0,str.size()-2);
    }
    target->add_node(port1,port2,port3,logic);
  }

  cout<<"Mincost of "<<circuit_file<<" : "<<mincost(target->root_edge(),all_patterns)<<endl;
  target->~Tree();
  for(int i=0;i<all_patterns.size();++i){
    all_patterns.at(i)->~Tree();
  }
  return 0;
}
