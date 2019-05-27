#ifndef GRAF_H
#define GRAF_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <climits>
#include <ctime>
#include <functional>
#include <queue>

#include "lista.h"
#include "netezinski_graf.h"

using namespace std;

struct Grad
{
    int indeks;
    double latitude;
    double longitude;
    Grad(int indeks, double latitude, double longitude):indeks(indeks), latitude(latitude), longitude(longitude) {}
};

class Graf
{

    struct Grana
    {
        int tezina;
        int usteda;
        int pocetak;
        int kraj;
        Grana(int tezina, int pocetak, int kraj):tezina(tezina),pocetak(pocetak),kraj(kraj){}
        Grana(int tezina, int pocetak, int kraj, double usteda):tezina(tezina),pocetak(pocetak),kraj(kraj), usteda(usteda){}
        friend bool operator <(const Grana& prva, const Grana& druga)
        {
            return prva.tezina < druga.tezina;
        }
    };

public:
    int n;
    vector <Grad> gradovi;
    vector <vector <int> > udaljenosti;
    vector<Grana> grane;

    int udaljenost(double x1, double y1, double x2, double y2)
    {
        return round(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
    }

    Graf(string filename);

    vector<int> vratiTuraNajmanjaGrana();
    bool provjeriCiklus(vector<Grana>& tura);
    int turaNajmanjaGrana();
    vector<int> _Greedy(vector<vector<int>> &udaljenosti, int pocetni);
};

Graf::Graf(string filename)
{
    ifstream unos(filename);
    while(!unos.eof())
    {
        int indeks;
        double latitude;
        double longitude;
        unos>>indeks;
        unos.get();
        unos>>latitude;
        unos.get();
        unos>>longitude;
        unos.get();
        gradovi.push_back(Grad(indeks,latitude,longitude));
    }
    unos.close();
    n = gradovi.size();
    udaljenosti.resize(n);
    for(int i=0; i<n; i++)
        udaljenosti[i].resize(n);
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
        {
            udaljenosti[i][j] = udaljenost(gradovi[i].latitude,gradovi[i].longitude,gradovi[j].latitude,gradovi[j].longitude);
            if(i<j)
                grane.push_back(Grana(udaljenosti[i][j],i,j));
        }
}

int duzinaTure(vector<vector<int>> &udaljenosti, vector<int>& tura)
{
    int duzina_ture = 0;
    int n = tura.size();

    for(int i=0; i < n; i++)
        duzina_ture += udaljenosti[tura[i]][tura[(i+1)%n]];

    return duzina_ture;
}

int duzinaTureL(vector<vector<int>> &udaljenosti, Lista<int>& tura)
{
    int duzina_ture = 0;
    int n = tura.getVelicina();
    Lista<int>::Iterator it(tura.Begin());
    while (it != tura.End())
    {
        Lista<int>::Iterator itplus1(it.getTrenutni()->sljedeci);
        //cout << "Uso: " << *it << " " << *itplus1 << " " << udaljenosti[*it][*(itplus1)] << endl;
        duzina_ture += udaljenosti[*it][*itplus1%n];
        it++;
    }

    return duzina_ture;
}

void ispisiTuru(vector<int>& tura)
{
    for(int i=0; i<tura.size(); i++)
        cout<<tura[i]<<" ";
    cout<<endl;
}

void ispisiTuru(const vector<int>& tura)
{
    for(int i=0; i<tura.size(); i++)
        cout<<tura[i]<<" ";
    cout<<endl;
}

bool provjeriTuru(vector<int>& tura)
{
    int n = tura.size();
    vector<bool> posjecen(n,false);
    if(tura.size() != n)
        return false;
    else
    {
        for(int i=0; i<n; i++)
        {
            if(tura[i]<0 || tura[i]>=n || posjecen[tura[i]] == true)
                return false;
            else
                posjecen[tura[i]] = true;
        }
    }
    return true;
}

bool Graf::provjeriCiklus(vector<Grana>& tura)
{
    vector<int> cvorovi;
    Grana grana = tura[0];
    cvorovi.push_back(grana.pocetak);
    cvorovi.push_back(grana.kraj);
    for(int j = 1; j < tura.size(); j++)
    {
        grana = tura[j];
        for(int i = 0; i < cvorovi.size(); i++)
        {
            if (cvorovi[i] == grana.pocetak || cvorovi[i] == grana.kraj) return true;
        }
    }
    return false;
}

int turaNajbliziSusjed(vector<vector<int>> &udaljenosti, int pocetni)
{

    vector<int> tura;
    int n = udaljenosti.size();
    vector<bool> posjecen(n,false);
    posjecen[pocetni] = true;
    tura.push_back(pocetni);
    for(int i=0; i<n-1; i++)
    {
        int trenutni = tura[tura.size()-1];
        int udaljenostNajblizeg = INT_MAX;
        int najblizi;
        for(int j=0; j<n; j++)
        {
            if(!posjecen[j] && udaljenosti[trenutni][j] < udaljenostNajblizeg)
            {
                udaljenostNajblizeg = udaljenosti[trenutni][j];
                najblizi = j;
            }
        }
        posjecen[najblizi] = true;
        tura.push_back(najblizi);
    }
    if(provjeriTuru(tura))
        return duzinaTure(udaljenosti,tura);
    else
        throw "Greska";
}

vector<int> turaNS(vector<vector<int>> &udaljenosti, int pocetni)
{
    vector<int> tura;
    int n = udaljenosti.size();
    vector<bool> posjecen(n,false);
    posjecen[pocetni] = true;
    tura.push_back(pocetni);
    for(int i=0; i<n-1; i++)
    {
        int trenutni = tura[tura.size()-1];
        int udaljenostNajblizeg = INT_MAX;
        int najblizi;
        for(int j=0; j<n; j++)
        {
            if(!posjecen[j] && udaljenosti[trenutni][j] < udaljenostNajblizeg)
            {
                udaljenostNajblizeg = udaljenosti[trenutni][j];
                najblizi = j;
            }
        }
        posjecen[najblizi] = true;
        tura.push_back(najblizi);
    }
    if(provjeriTuru(tura))
        return tura;
    else
        throw "Greska";
}

int Graf::turaNajmanjaGrana()
{
    Ruta ruta(n);
    sort(grane.begin(),grane.end());
    vector<int> stepeni(n,0);
    int trenutna = 0;
    for(int i=0; i<n-1; i++)
    {
        while(true)
        {
            Grana trenutnaGrana = grane[trenutna];
            int pocetak = trenutnaGrana.pocetak;
            int kraj = trenutnaGrana.kraj;
            if(stepeni[pocetak] == 2 || stepeni[kraj] == 2)
            {
                trenutna++;
                continue;
            }
            ruta.dodajGranu(pocetak,kraj);
            if(ruta.imaLiCiklusa(pocetak))
            {
                ruta.obrisiGranu(pocetak,kraj);
                trenutna++;
                continue;
            }
            else
            {
                stepeni[pocetak]++;
                stepeni[kraj]++;
                trenutna++;
                break;
            }

        }
    }

    vector<int> tura = ruta.vratiTuru();
    if(provjeriTuru(tura))
        return duzinaTure(udaljenosti,tura);
    else
        throw "Greska";
}

vector<int> Graf::vratiTuraNajmanjaGrana()
{
    Ruta ruta(n);
    sort(grane.begin(),grane.end());
    vector<int> stepeni(n,0);
    int trenutna = 0;
    for(int i=0; i<n-1; i++)
    {
        while(true)
        {
            Grana trenutnaGrana = grane[trenutna];
            int pocetak = trenutnaGrana.pocetak;
            int kraj = trenutnaGrana.kraj;
            if(stepeni[pocetak] == 2 || stepeni[kraj] == 2)
            {
                trenutna++;
                continue;
            }
            ruta.dodajGranu(pocetak,kraj);
            if(ruta.imaLiCiklusa(pocetak))
            {
                ruta.obrisiGranu(pocetak,kraj);
                trenutna++;
                continue;
            }
            else
            {
                stepeni[pocetak]++;
                stepeni[kraj]++;
                trenutna++;
                break;
            }

        }
    }

    vector<int> tura = ruta.vratiTuru();
    if(provjeriTuru(tura))
        return tura;
    else
        throw "Greska";
}

void Zamijeni_Grane(vector<int> &tura, int i, int j)
{
    for (int k = 0; k < (j - i) / 2; k++)
    {
        int pomocna = tura[i + 1 + k];
        tura[i + 1 + k] = tura[j - k];
        tura[j - k] = pomocna;
    }
}

void Zamijeni_Grane(vector<int> &tura, int i, int j, int k, int tip)
{
    vector<int> nova_tura = tura;

    if (tip == 3)
    {
        for (int t = i + 1; t <= i + k - j; t++)
            nova_tura[t] = tura[k - t + i + 1];
        for (int t = i + k - j + 1; t <= k; t++)
            nova_tura[t] = tura[t - k + j];
    }
    else if (tip == 2)
    {
        for (int t = i + 1; t <= i + k - j; t++)
            nova_tura[t] = tura[j + t - i];
        for (int t = i + k - j + 1; t <= k; t++)
            nova_tura[t] = tura[t - k + j];
    }
    else if (tip == 1)
    {
        for (int t = i + 1; t <= j; t++)
            nova_tura[t] = tura[j + i + 1 - t];
        for (int t = j + 1; t <= k; t++)
            nova_tura[t] = tura[k + j + 1 - t];
    }
    else if (tip == 0)
    {
        for (int t = i + 1; t <= i + k - j; t++)
            nova_tura[t] = tura[j + t - i];
        for (int t = i + k - j + 1; t <= k; t++)
            nova_tura[t] = tura[i + k + 1 - t];
    }
    tura = nova_tura;
}

vector<int> _2OptSaRandom(vector<vector<int>> &udaljenosti)
{
    int broj_cvorova = udaljenosti.size();
    vector<int> tura(broj_cvorova);
    for(int i=0; i<broj_cvorova; i++)
        tura[i] =  i;
    random_shuffle(tura.begin(),tura.end());

    //cout<<"Duzina pocetne ture: "<<duzinaTure(udaljenosti,tura)<<endl;
    while (true)
    {
        bool nadjena_bolja_tura = false;

        for (int i = 0; i < broj_cvorova - 1; i++)
            for (int j = i + 2; j < broj_cvorova; j++)
            {
                if (udaljenosti[tura[i]][tura[i + 1]] + udaljenosti[tura[j]][tura[(j + 1) % broj_cvorova]]
                        > (udaljenosti[tura[i]][tura[j]] + udaljenosti[tura[i + 1]][tura[(j + 1) % broj_cvorova]]))
                {
                    Zamijeni_Grane(tura, i, j);
                    nadjena_bolja_tura = true;
                }
            }
        if (!nadjena_bolja_tura)
            break;
    }
    return tura;
}

void ispisTure(const Lista<int>& turaL){
    Lista<int>::Iterator ispis(turaL.Begin());
        while(ispis != turaL.End()){
            cout << *ispis << " ";
            ispis++;
        }
    cout << endl;
}

Lista<int> TwoOptSaRandom(vector<vector<int>> &udaljenosti)
{
    int broj_cvorova = udaljenosti.size();
    vector<int> tura(broj_cvorova);
    for(int i=0; i<broj_cvorova; i++)
        tura[i] =  i;
    random_shuffle(tura.begin(),tura.end());

    Lista<int> turaL;
    for(int i=0; i<broj_cvorova; i++)
        turaL.DodajNaKraj(tura[i]);

    ispisTure(turaL);

    //cout<<"Duzina pocetne ture: "<<duzinaTure(udaljenosti,tura)<<endl;
    while (true)
    {
        bool nadjena_bolja_tura = false;

        Lista<int>::Iterator it(turaL.Begin());
        Lista<int>::Iterator jt(++(++(turaL.Begin())));
        Lista<int>::Iterator predzadnji(--(turaL.End()));
        /*for (int i = 0; i < broj_cvorova - 1; i++)
            for (int j = i + 2; j < broj_cvorova; j++)
            {
                if (udaljenosti[tura[i]][tura[i + 1]] + udaljenosti[tura[j]][tura[(j + 1) % broj_cvorova]]
                        > (udaljenosti[tura[i]][tura[j]] + udaljenosti[tura[i + 1]][tura[(j + 1) % broj_cvorova]]))
                {
                    turaL.Zamijeni_Grane(it, jt);
                    //Zamijeni_Grane(tura, i, j);
                    nadjena_bolja_tura = true;
                }
            }*/
        for (it; it != predzadnji; it++)
            for (jt; jt != turaL.End(); jt++)
            {
                Lista<int>::Iterator itplus1(it.getTrenutni()->sljedeci);
                Lista<int>::Iterator jtplus1(jt.getTrenutni()->sljedeci);
                //--it;
                //Lista<int>::Iterator jtplus1 = ++jt;
                //--jt;
                if (udaljenosti[*it][*(itplus1)] + udaljenosti[*jt][*jtplus1 % broj_cvorova]
                        > (udaljenosti[*it][*jt] + udaljenosti[*(itplus1)][*jtplus1 % broj_cvorova]))
                {
                    //cout << "Mijenjam: " << *it << " i " << *jt << endl;
                    turaL.Zamijeni_Grane(it, jt);
                    //Zamijeni_Grane(tura, i, j);
                    nadjena_bolja_tura = true;
                }
            }
        if (!nadjena_bolja_tura)
            break;
    }
    return turaL;
}

vector<int> _2OptSaRandomMax(vector<vector<int>> &udaljenosti)
{
    int broj_cvorova = udaljenosti.size();
    vector<int> tura(broj_cvorova);
    for(int i=0; i<broj_cvorova; i++)
        tura[i] =  i;
    random_shuffle(tura.begin(),tura.end());

    //cout<<"Duzina pocetne ture: "<<duzinaTure(udaljenosti,tura)<<endl;
    while (true)
    {
        int najveca_usteda = 0;
        int indeks_i,indeks_j;

        for (int i = 0; i < broj_cvorova - 1; i++)
            for (int j = i + 2; j < broj_cvorova; j++)
            {
                int usteda = udaljenosti[tura[i]][tura[i + 1]] + udaljenosti[tura[j]][tura[(j + 1) % broj_cvorova]]
                        - (udaljenosti[tura[i]][tura[j]] + udaljenosti[tura[i + 1]][tura[(j + 1) % broj_cvorova]]);

                if (usteda > najveca_usteda)
                {
                    najveca_usteda = usteda;
                    indeks_i = i;
                    indeks_j = j;
                }
            }
        if (najveca_usteda == 0)
            break;
        Zamijeni_Grane(tura, indeks_i, indeks_j);
        cout<<duzinaTure(udaljenosti,tura)<<endl;
    }
    return tura;
}

int _2OptSaPocetnom(vector<vector<int>> &udaljenosti, vector<int> &tura)
{

    int broj_cvorova = tura.size();
    while (true)
    {
        bool nadjena_bolja_tura = false;

        for (int i = 0; i < broj_cvorova - 1; i++)
            for (int j = i + 2; j < broj_cvorova; j++)
            {
                if (udaljenosti[tura[i]][tura[i + 1]] + udaljenosti[tura[j]][tura[(j + 1) % broj_cvorova]]
                        > (udaljenosti[tura[i]][tura[j]] + udaljenosti[tura[i + 1]][tura[(j + 1) % broj_cvorova]]))
                {
                    Zamijeni_Grane(tura, i, j);
                    nadjena_bolja_tura = true;
                }
            }
        if (!nadjena_bolja_tura)
            break;
    }

}

Lista<int> TwoOptSaPocetnom(vector<vector<int>> &udaljenosti, vector<int> tura)
{
    Lista<int> turaL;
    for(int i=0; i<tura.size(); i++)
        turaL.DodajNaKraj(tura[i]);
    cout << "Prije while" << endl;
    ispisTure(turaL);

    cout << "begin: " << turaL.Begin().getTrenutni()->element<<endl;
    while (true)
    {
        bool nadjena_bolja_tura = false;

        Lista<int>::Iterator it(turaL.Begin());
        Lista<int>::Iterator jt(++(++(turaL.Begin())));
        Lista<int>::Iterator predzadnji(--(turaL.End()));
        for (it; it != predzadnji; it++)
            for (jt; jt != turaL.End(); jt++)
            {
                Lista<int>::Iterator itplus1(it.getTrenutni()->sljedeci);
                Lista<int>::Iterator jtplus1(jt.getTrenutni()->sljedeci);
                //--it;
                //Lista<int>::Iterator jtplus1 = ++jt;
                //--jt;
                if (udaljenosti[*it][*(itplus1)] + udaljenosti[*jt][*jtplus1 % tura.size()]
                        > (udaljenosti[*it][*jt] + udaljenosti[*(itplus1)][*jtplus1 % tura.size()]))
                {
                    cout << "Mijenjam: " << *it << " i " << *jt << endl;
                    turaL.Zamijeni_Grane(it, jt);
                    //Zamijeni_Grane(tura, i, j);
                    ispisTure(turaL);
                    nadjena_bolja_tura = true;
                }
            }
        if (!nadjena_bolja_tura)
            break;
    }
    cout << "Krajnja: " << endl;
    ispisTure(turaL);
    return turaL;
}


int _3OptSaPocetnom(vector<vector<int>> &udaljenosti, vector<int> &tura)
{
    int broj_cvorova = tura.size();
    while (true)
    {
        bool nadjena_bolja_tura = false;

        for (int i = 0; i < broj_cvorova - 2; i++)
            for (int j = i + 1; j < broj_cvorova - 1; j++)
                for (int k = j + 1; k < broj_cvorova; k++)
                {
                    if (udaljenosti[tura[i]][tura[i + 1]]
                            + udaljenosti[tura[j]][tura[(j + 1)]]
                            + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                            > udaljenosti[tura[i]][tura[k]]
                            + udaljenosti[tura[i + 1]][tura[(j + 1)]]
                            + udaljenosti[tura[j]][tura[(k + 1) % broj_cvorova]])
                    {
                        Zamijeni_Grane(tura, i, j, k, 3);
                        cout<<duzinaTure(udaljenosti,tura)<<endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                            + udaljenosti[tura[j]][tura[(j + 1)]]
                            + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                            > udaljenosti[tura[i]][tura[j + 1]]
                            + udaljenosti[tura[i + 1]][tura[k]]
                            + udaljenosti[tura[j]][tura[(k + 1) % broj_cvorova]])
                    {
                        Zamijeni_Grane(tura, i, j, k, 2);
                        cout<<duzinaTure(udaljenosti,tura)<<endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                            + udaljenosti[tura[j]][tura[(j + 1)]]
                            + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                            > udaljenosti[tura[i]][tura[j]]
                            + udaljenosti[tura[i + 1]][tura[k]]
                            + udaljenosti[tura[j + 1]][tura[(k + 1) % broj_cvorova]])
                    {
                        Zamijeni_Grane(tura, i, j, k, 1);
                        cout<<duzinaTure(udaljenosti,tura)<<endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                            + udaljenosti[tura[j]][tura[(j + 1)]]
                            + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                            > udaljenosti[tura[i]][tura[j + 1]]
                            + udaljenosti[tura[k]][tura[(j)]]
                            + udaljenosti[tura[i + 1]][tura[(k + 1) % broj_cvorova]])
                    {
                        Zamijeni_Grane(tura, i, j, k, 0);
                        cout<<duzinaTure(udaljenosti,tura)<<endl;
                        nadjena_bolja_tura = true;
                    }

                }
        if (!nadjena_bolja_tura)
            break;
    }
    return duzinaTure(udaljenosti,tura);
}

vector<int> _3OptSaRandom(vector<vector<int>> &udaljenosti)
{
    int broj_cvorova = udaljenosti.size();
    vector<int> tura(broj_cvorova);
    for(int i=0; i<broj_cvorova; i++)
        tura[i] =  i;
    random_shuffle(tura.begin(),tura.end());
    while (true)
    {
        bool nadjena_bolja_tura = false;

        for (int i = 0; i < broj_cvorova - 2; i++)
            for (int j = i + 1; j < broj_cvorova - 1; j++)
                for (int k = j + 1; k < broj_cvorova; k++)
                {
                    if (udaljenosti[tura[i]][tura[i + 1]]
                            + udaljenosti[tura[j]][tura[(j + 1)]]
                            + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                            > udaljenosti[tura[i]][tura[k]]
                            + udaljenosti[tura[i + 1]][tura[(j + 1)]]
                            + udaljenosti[tura[j]][tura[(k + 1) % broj_cvorova]])
                    {
                        Zamijeni_Grane(tura, i, j, k, 3);
                        cout<<duzinaTure(udaljenosti,tura)<<endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                            + udaljenosti[tura[j]][tura[(j + 1)]]
                            + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                            > udaljenosti[tura[i]][tura[j + 1]]
                            + udaljenosti[tura[i + 1]][tura[k]]
                            + udaljenosti[tura[j]][tura[(k + 1) % broj_cvorova]])
                    {
                        Zamijeni_Grane(tura, i, j, k, 2);
                        cout<<duzinaTure(udaljenosti,tura)<<endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                            + udaljenosti[tura[j]][tura[(j + 1)]]
                            + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                            > udaljenosti[tura[i]][tura[j]]
                            + udaljenosti[tura[i + 1]][tura[k]]
                            + udaljenosti[tura[j + 1]][tura[(k + 1) % broj_cvorova]])
                    {
                        Zamijeni_Grane(tura, i, j, k, 1);
                        cout<<duzinaTure(udaljenosti,tura)<<endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                            + udaljenosti[tura[j]][tura[(j + 1)]]
                            + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                            > udaljenosti[tura[i]][tura[j + 1]]
                            + udaljenosti[tura[k]][tura[(j)]]
                            + udaljenosti[tura[i + 1]][tura[(k + 1) % broj_cvorova]])
                    {
                        Zamijeni_Grane(tura, i, j, k, 0);
                        cout<<duzinaTure(udaljenosti,tura)<<endl;
                        nadjena_bolja_tura = true;
                    }

                }
        if (!nadjena_bolja_tura)
            break;
    }
    return tura;
}

int iscrpnaPretraga(vector<vector<int>> &udaljenosti)
{

    int n = udaljenosti.size();
    vector<int> tura(n);
    for(int i=0; i<n; i++)
        tura[i] = i;

    int najkraca_tura = INT_MAX;
    do
    {
        if(duzinaTure(udaljenosti,tura) < najkraca_tura)
            najkraca_tura = duzinaTure(udaljenosti, tura);
    }
    while (next_permutation(tura.begin(),tura.end()));

    return najkraca_tura;

}


vector<int> Graf::_Greedy(vector<vector<int>> &udaljenosti, int pocetni)
{
    auto cmp = [](Grana left, Grana right) { return left.usteda <= right.usteda;};
    std::priority_queue<Grana, std::vector<Grana>, decltype(cmp)> pq(cmp);

    for(int i = 0; i < udaljenosti.size(); i++)
    {
        for(int j = 0; j < udaljenosti.size(); j++)
        {
            if(i != j && i != pocetni && j != pocetni)
                pq.push(Grana(udaljenosti[i][j], i, j, udaljenosti[pocetni][i] + udaljenosti[j][pocetni] - udaljenosti[i][j]));
        }
    }

    vector<int> gradovi;
    for(int i = 0; i < udaljenosti.size(); i++)
    {
        if(i == pocetni) continue;
        gradovi.push_back(i);
    }

    Ruta ruta(n);
    vector<int> stepeni(n,0);
    while(true)
    {
        Grana trenutna(pq.top().tezina, pq.top().pocetak, pq.top().kraj, pq.top().usteda);
        pq.pop();
        if(stepeni[trenutna.pocetak] < 2 && stepeni[trenutna.kraj] < 2)
        {
            ruta.dodajGranu(trenutna.pocetak, trenutna.kraj);
            stepeni[trenutna.pocetak] += 1;
            stepeni[trenutna.kraj] += 1;
            if (ruta.imaLiCiklusa(trenutna.pocetak))
            {
                ruta.obrisiGranu(trenutna.pocetak, trenutna.kraj);
                stepeni[trenutna.pocetak] -= 1;
                stepeni[trenutna.kraj] -= 1;
            }
        }
        if(stepeni[trenutna.pocetak] == 2)
        {
            for(int i = 0; i < gradovi.size(); i++)
            {
                if(gradovi[i] == trenutna.pocetak)
                {
                    gradovi.erase(gradovi.begin() + i);
                    break;
                }
            }
        }
        if(stepeni[trenutna.kraj] == 2)
        {
            for(int i = 0; i < gradovi.size(); i++)
            {
                if(gradovi[i] == trenutna.kraj)
                {
                    gradovi.erase(gradovi.begin() + i);
                    break;
                }
            }
        }
        if(gradovi.size() == 2)
        {
            for(int i = 0; i < stepeni.size(); i++){
                if(stepeni[i] < 2 && i != pocetni)
                {
                    stepeni[i] += 1;
                    stepeni[pocetni] += 1;
                    ruta.dodajGranu(pocetni, i);
                }
            }
            break;
        }
    }

    vector<int> konacnatura = ruta.vratiTuru(pocetni);
    if(provjeriTuru(konacnatura))
        return konacnatura;
    else
        throw("Greska");

}
#endif // GRAF_H
