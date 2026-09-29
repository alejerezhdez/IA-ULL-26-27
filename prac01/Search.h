#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include <vector>
#include <list>
#include <string>
#include <ostream>
#include "mapa.h"
#include "estado.h"

class Search {
  public:
    Search();
    Search(Mapa mapa);

    void BusquedaA(const std::string& fichero_traza = "traza.txt", const std::string& fichero_camino = "camino.txt");
    void expandir(const Estado& estado);
    double eval(const Estado& estado);
    void imprimir_iteracion(int it, std::ostream& os);

  private:
    Mapa map_;
    std::list<Estado> cerrados_;
    std::vector<Estado> abiertos_;
};

#endif