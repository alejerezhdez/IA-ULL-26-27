#include "mapa.h"
#include "estado.h"
#ifndef BUSQUEDA_H
#define BUSQUEDA_H

class Search {
  public:
    Search();
    Search(Mapa mapa);

    void BusquedaA();
    void expandir(const Estado& estado);
    double eval(const Estado& estado);
  private:
    Mapa map_;
    std::vector<Estado> cerrados_;
    std::vector<Estado> abiertos_;
};

#endif