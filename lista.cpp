#include "lista.h"
#ifndef LISTA_CPP
#define LISTA_CPP

template <typename Tip>
Lista<Tip>::Lista() :velicina(0) {
  head = new Cvor(Tip{}, nullptr,nullptr);
  tail = new Cvor(Tip{}, nullptr, head);
  head->sljedeci = tail;
}

template <typename Tip>
typename Lista<Tip>::Iterator Lista<Tip>::UmetniIspredCvora(Iterator it, Tip element) {
  it.trenutni->prethodni = it.trenutni->prethodni->sljedeci = new Cvor(element,it.trenutni,it.trenutni->prethodni);
  velicina++;
  it--;
  return it;
}

template <typename Tip>
typename Lista<Tip>::Iterator Lista<Tip>::IzbrisiCvor(Iterator it) {
  if(it.trenutni==nullptr)
    throw "Nemoguce je izbrisati";
  it.trenutni->prethodni->sljedeci = it.trenutni->sljedeci;
  it.trenutni->sljedeci->prethodni = it.trenutni->prethodni;
  Iterator it2 (it.trenutni->sljedeci);
  delete it.trenutni;
  velicina--;
  return it2;
}

template <typename Tip>
void Lista<Tip>::Clear() {
  Iterator it = Begin();
  while(it!=End())
    it = IzbrisiCvor(it);
}

template <typename Tip>
Lista<Tip>::~Lista() {
  Clear();
  delete head;
  delete tail;
}

template <typename Tip>
Lista<Tip>::Lista(const Lista<Tip>& kopija):velicina(0) {
  head = new Cvor(Tip{}, nullptr,nullptr);
  tail = new Cvor(Tip{}, nullptr, head);
  head->sljedeci = tail;
  for(Iterator it = kopija.Begin();it!=kopija.End();it++)
    DodajNaKraj(it.trenutni->element);
}

template <typename Tip>
Lista<Tip>& Lista<Tip>:: operator=(Lista<Tip> &kopija) {
  if(this==&kopija)
    return *this;
  this->Clear();
  for(Iterator it = kopija.Begin();it!=kopija.End();it++)
    DodajNaKraj(it.trenutni->element);
  return *this;
}

template <typename Tip>
void Lista<Tip>::ObradiSveElemente(void (*akcija)(Tip&)) {
  for(Iterator it = Begin();it!=End();it++)
    akcija(it.trenutni->element);
}

template <typename Tip>
void Lista<Tip>::Zamijeni_Grane(Iterator i, Iterator j){
  Lista<int>::Iterator iplus1(i.getTrenutni()->sljedeci);
  Lista<int>::Iterator jplus1(j.getTrenutni()->sljedeci);
  i.trenutni->sljedeci = j.trenutni;
  iplus1.trenutni->sljedeci = jplus1.trenutni;
  j.trenutni->sljedeci = iplus1.trenutni;

//  cout << "I: " << *i << endl;
//  cout << "I sljedeci: " << i.trenutni->sljedeci->element << endl;
//  ++i;
//  cout << "I+1 sljedeci: " << i.trenutni->sljedeci->element << endl;
//  i.trenutni->sljedeci = j.trenutni;
//  ++i;
//  j++;
//  i.trenutni->sljedeci = j.trenutni;
//  --j;
//  j.trenutni->sljedeci = i.trenutni;
//  ++j;
}

#endif // LISTA_CPP

