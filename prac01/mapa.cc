#include "mapa.h"
#include <fstream>
#include <sstream>

Mapa::Mapa() :
  num_filas_(1), num_columnas_(1), estado_inicial_(1,1), estado_final_(1,1) {}

// Metodo que lee el fichero y crea el mapa donde se movera el robot
Mapa::Mapa(std::string fichero_entrada) {
  std::ifstream input_file(fichero_entrada);
  std::string linea;
  int contadorfilas{0}, contadorcolumnas{0};
  while (std::getline(input_file, linea)) {
    std::istringstream ss(linea);
    std::vector<int> fila;
    int valor_celda;
    contadorcolumnas = 0;
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
  num_filas_ = celdas_.size();
  num_columnas_ = celdas_.empty() ? 0 : celdas_[0].size();
}

const Estado Mapa::get_inicial() {
  return Estado(estado_inicial_.first, estado_inicial_.second, nullptr);
}

const Estado Mapa::get_final() {
  return Estado(estado_final_.first, estado_final_.second, nullptr);
}

const int Mapa::get_filas() {
  return num_filas_;
}

const int Mapa::get_cols() {
  return num_columnas_;
}

const int Mapa::at(int x, int y) {
  return celdas_[x][y];
}