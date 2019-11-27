#include <iostream>
#include <string>
#include <ctime>
#include "KL_E24066242.h"
using namespace std;

int main(int argc,char **argv){
  clock_t start, stop;
  start = clock();
  KL kl(argv[1]);

  //kl.check();
  kl.f();
  cout<<endl;
  while(kl.max_cost()){
    kl.update_new_group();
    kl.f();
    cout<<endl;
  }



  //kl.check(0);
  //kl.check();



  /*
  while(kl.max_cost()){
    kl.update_temp();
    kl.check(0);
    //kl.check();
    cout<<endl;
  }*/
  kl.result();
  stop = clock();
  cout << "runtime: "<<double(stop - start) / CLOCKS_PER_SEC <<" s"<<endl;
}
