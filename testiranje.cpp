#include <iostream>
#include <vector>
#include <cstdio>
#include <ctime>

#include "dinamicko.h"
#include "graf.h"
#include "lista.h"

using namespace std;

int main() {
  Graf G("uruguay.txt");

  clock_t start;
  double duration;
  start = clock();

  vector<int> tura = turaNS(G.udaljenosti, 0);

  duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
  //_2OptSaPocetnom(G.udaljenosti,tura);
  ispisiTuru(tura);
  cout << endl;
  cout<<duzinaTure(G.udaljenosti, tura) << " , " << duration << "s" << endl;

  start = clock();
  vector<int> turagrana = G.vratiTuraNajmanjaGrana();
  duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
  ispisiTuru(turagrana);
  cout << duzinaTure(G.udaljenosti, turagrana) << " , " << duration << "s" << endl;

  start = clock();
  vector<int> mojaTura = G._Greedy(G.udaljenosti, 0);
  duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
  ispisiTuru(mojaTura);
  cout << duzinaTure(G.udaljenosti, mojaTura) << " , " << duration << "s" << endl;

  /*vector<int> zekatura = G.turaClarkeWright();
  for(int i = 0 ; i < zekatura.size(); i++)
  {
      cout << zekatura[i] << " ";
  }
  cout << endl;*/
  //ispisiTuru(tura);
  //_2OptSaPocetnom(G.udaljenosti,tura);
  //cout<<endl<<duzinaTure(G.udaljenosti, tura) << endl;
  //TwoOptSaRandom(G.udaljenosti);
  //cout<<duzinaTure(G.udaljenosti, tura);

//  long int zbir = 0;
//  for(int i=0;i<50;i++) {
//    vector<int> tura = _2OptSaRandom(G.udaljenosti);
//    //cout<<"Duzina konacne ture: "<<duzinaTure(G.udaljenosti,tura)<<endl;
//    zbir += duzinaTure(G.udaljenosti,tura);
//  }
//  cout<<zbir/50;

  /*long int zbir = 0;
  for(int i=0;i<1;i++) {
    vector<int> tura = _3OptSaRandom(G.udaljenosti);
    //cout<<"Duzina konacne ture: "<<duzinaTure(G.udaljenosti,tura)<<endl;
    zbir += duzinaTure(G.udaljenosti,tura);
  }
  cout<<zbir;
  int n = G.udaljenosti.size();
  vector<vector<int>> state(n);
    for(auto& neighbors : state)
        neighbors = vector<int>((1 << n) - 1, INT_MAX);
  cout<<tsp(G.udaljenosti,0,1,state);*/
}
