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

    // Constructor por defecto
    Search();

    // Constructor por parametros
    Search(Mapa mapa);

    // Metodo principal de la busqueda a*
    void BusquedaA(const std::string& fichero_traza = "traza.txt", const std::string& fichero_camino = "camino.txt");
    
    // Metodo para expandir un estado
    void expandir(const Estado& estado);
    
    // Metodo para evaluar el candidato optimo
    double eval(const Estado& estado);

    // Metodo para imprimir una iteracion de la busqueda
    void imprimir_iteracion(int it, std::ostream& os);

  private:
    Mapa map_;
    std::list<Estado> cerrados_;
    std::vector<Estado> abiertos_;
};

#endif