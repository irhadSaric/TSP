#ifndef NETEZINSKI_GRAF_H
#define NETEZINSKI_GRAF_H

#include <vector>
#include <queue>
#include <algorithm>
#include <stack>
using namespace std;

class Ruta {
  int n;
  vector <vector <int> > lista_susjedstva;

public:
  Ruta(int broj_cvorova);
  void dodajGranu(int i, int j);
  void obrisiGranu(int i, int j);
  bool imaLiCiklusa(int pocetni);
  vector<int> vratiTuru();
  vector<int> vratiTuru(int pocetni);
};

Ruta::Ruta(int broj_cvorova){
  n = broj_cvorova;
  lista_susjedstva.resize(n);
}

void Ruta::dodajGranu(int i, int j) {
  if(i>=n || j>=n)
    throw "Cvor ne postoji";
  lista_susjedstva[i].push_back(j);
  lista_susjedstva[j].push_back(i);
}

void Ruta::obrisiGranu(int i, int j) {
  if(i>=n || j>=n)
    throw "Cvor ne postoji";

  for(auto it = lista_susjedstva[i].begin();it!=lista_susjedstva[i].end();it++)
      if(*it == j) {
        lista_susjedstva[i].erase(it);
        break;
      }

  for(auto it = lista_susjedstva[j].begin();it!=lista_susjedstva[j].end();it++)
      if(*it == i) {
        lista_susjedstva[j].erase(it);
        break;
      }

}

bool Ruta::imaLiCiklusa(int pocetni) {
    vector<int> ciklus;
    vector<bool> posjeceni(n,false);
    vector<int> dodaoJe(n,-1);
    queue<int> red;
    red.push(pocetni);
    dodaoJe[pocetni] = -2;
    while(!red.empty()) {
        int trenutni = red.front();
        red.pop();
        if(!posjeceni[trenutni]) {
            posjeceni[trenutni] = true;
            for(int i=0;i<lista_susjedstva[trenutni].size();i++)
                if(!posjeceni[lista_susjedstva[trenutni][i]]) {
                  int trenutniPocetak = trenutni;
                  if(dodaoJe[lista_susjedstva[trenutni][i]]!=-1) {
                    return true;
                  }
                  red.push(lista_susjedstva[trenutni][i]);
                  dodaoJe[lista_susjedstva[trenutni][i]] = trenutni;
                }

        }
    }
    return false;
}

vector<int> Ruta::vratiTuru() {

  int pocetakTure;
  for(int i=0;i<n;i++)
      if(lista_susjedstva[i].size() == 1) {
        pocetakTure = i;
        break;
      }
  vector<int> ruta;
  ruta.push_back(pocetakTure);
  vector<bool> posjeceni(n,false);
  posjeceni[pocetakTure] = true;
  int trenutni = pocetakTure;
  for(int i=0;i<n-1;i++) {
    if(!posjeceni[lista_susjedstva[trenutni][0]])
        trenutni = lista_susjedstva[trenutni][0];
    else
        trenutni = lista_susjedstva[trenutni][1];
    ruta.push_back(trenutni);
    posjeceni[trenutni] = true;
  }
  cout << endl;
  return ruta;
}

vector<int> Ruta::vratiTuru(int pocetni) {

  int pocetakTure;
  for(int i=0;i<n;i++)
      if(lista_susjedstva[i].size() == 1) {
        pocetakTure = i;
        break;
      }
  pocetakTure = pocetni;
  vector<int> ruta;
  ruta.push_back(pocetakTure);
  vector<bool> posjeceni(n,false);
  posjeceni[pocetakTure] = true;
  int trenutni = pocetakTure;
  for(int i=0;i<n-1;i++) {
    if(!posjeceni[lista_susjedstva[trenutni][0]])
        trenutni = lista_susjedstva[trenutni][0];
    else
        trenutni = lista_susjedstva[trenutni][1];
    ruta.push_back(trenutni);
    posjeceni[trenutni] = true;
  }
  cout << endl;
  return ruta;
}

#endif // NETEZINSKI_GRAF_H

