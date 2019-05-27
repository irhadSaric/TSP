#ifndef LISTA_H
#define LISTA_H

template <typename Tip>
class Lista {
  struct Cvor {
    Tip element;
    Cvor *sljedeci,*prethodni;
    Cvor(Tip element, Cvor *sljedeci,Cvor *prethodni):element{element},sljedeci{sljedeci},prethodni{prethodni}{}
  };
  public:
   class Iterator {
     Cvor *trenutni;
     public:
     Iterator(Cvor *trenutni = nullptr):trenutni{trenutni}{}
     Iterator operator++(int) {
       Iterator stari = *this;
       trenutni = trenutni->sljedeci;
       return stari;
     }
     Iterator &operator++() {
       trenutni = trenutni->sljedeci;
       return *this;
     }
     Iterator operator--(int) {
       Iterator stari = *this;
       trenutni = trenutni->prethodni;
       return stari;
     }
     Iterator &operator--() {
       trenutni = trenutni->prethodni;
       return *this;
     }
     Tip &operator*() { return trenutni->element; }
     Cvor* getTrenutni() { return trenutni; }
     bool operator==(const Iterator it) { return trenutni == it.trenutni; }
     bool operator!=(const Iterator it) { return trenutni != it.trenutni; }
     friend class Lista<Tip>;
   };
  private:
  int velicina;
  Cvor *head, *tail;
  public:
      Lista();
      Lista(const Lista<Tip>& kopija);
      ~Lista();
      Lista<Tip> &operator=(Lista<Tip> &kopija);
      bool Prazna() { return velicina==0;}
      void DodajNaPocetak(Tip element) { UmetniIspredCvora(Begin(),element); }
      void DodajNaKraj(Tip element) { UmetniIspredCvora(End(),element); }
      void IzbrisiSaPocetka() { IzbrisiCvor(Begin()); }
      void IzbrisiSaKraja() { IzbrisiCvor(--End()); }
      Iterator UmetniIspredCvora(Iterator it, Tip element);
      Iterator IzbrisiCvor(Iterator it);
      void ObradiSveElemente(void (*akcija)(Tip&));
      Iterator Begin() const { return Iterator(head->sljedeci); }
      Iterator End() const { return Iterator(tail); }
      void Clear();
      int getVelicina() { return velicina; }
      void redPogubljenja(int n, int m);
      void Zamijeni_Grane(Iterator i, Iterator j);
};

#include "lista.cpp"

#endif // LISTA_H

