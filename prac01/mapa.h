#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include <iostream>
#include <utility>
#include <string>
#include "estado.h"

class Mapa {
  public:

    // Constructor por defecto
    Mapa();

    // Constructor
    Mapa(std::string fichero_entrada);

    // Getters
    const Estado get_inicial();
    const Estado get_final();

    const int get_filas();
    const int get_cols();

    // Metodo para acceder a una celda
    const int at(int x, int y);

  private:
    int num_filas_;
    int num_columnas_;
    std::vector<std::vector<int>> celdas_;
    std::pair<int, int> estado_inicial_;
    std::pair<int, int> estado_final_;
};

#endif