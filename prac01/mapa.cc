#include "mapa.h"
#include "fstream"
#include <sstream>

Mapa::Mapa() :
  num_filas_(1), num_columnas_(1), estado_inicial_(1,1), estado_final_(1,1) {}

Mapa::Mapa(int filas, int columnas, std::string fichero_entrada) {
  num_filas_ = filas;
  num_columnas_ = columnas;
  std::ifstream input_file(fichero_entrada);
  std::string linea;
  int contadorfilas{1}, contadorcolumnas{1};
  while (std::getline(input_file, linea)) {
    std::istringstream ss(linea);
    std::vector<int> fila;
    int valor_celda;
    contadorcolumnas = 1;
    while (ss >> valor_celda) {
      if (valor_celda == 0) {
        estado_inicial_.first = contadorfilas;
        estado_inicial_.second = contadorcolumnas;
      } else if (valor_celda == 10) {
        estado_final_.first = contadorfilas;
        estado_final_.second = contadorcolumnas;
      }
      fila.push_back(valor_celda);
      contadorcolumnas++;
    }
    if (!fila.empty()) {
      celdas_.push_back(fila);
    }
    contadorfilas++;
  }
}
