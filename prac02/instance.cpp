#include "instance.h"
#include <fstream>
#include <sstream>
#include <iostream>

Instance::Instance() :
  n_trabajos_(0), n_maquinas_(0), semilla_(0) {}

Instance::Instance(std::string fichero) {
  std::string linea;

  std::ifstream input_file(fichero);
  if (!input_file.is_open()) {
    std::cout << "Error archivo no abierto";
  }
  std::getline(input_file, linea);

  std::getline(input_file, linea);

  std::istringstream iss(linea);
  int n_jobs{0}, n_machines{0}, semilla{0}, upper_bound{0}, lower_bound{0};

  iss >> n_jobs >> n_machines >> semilla >> upper_bound >> lower_bound;

  n_trabajos_ = n_jobs;
  n_maquinas_ = n_machines;
  semilla_ = semilla;
  upper_bound_ = upper_bound;
  lower_bound_ = lower_bound;

  tiempos_.assign(n_trabajos_, std::vector<int>(n_maquinas_));   // [trabajo][maquina]

      std::getline(input_file, linea);   // cabecera "processing times"

  for (int j{0}; j < n_maquinas_; j++) {        // una línea por máquina
    std::getline(input_file, linea);
    std::istringstream lineas_matriz(linea);
    for (int i{0}; i < n_trabajos_; i++) {      // n tiempos por línea
      lineas_matriz >> tiempos_[i][j];
    }
  }  
}

int Instance::cmax(const std::vector<int>& permutacion) const {
  std::vector<std::vector<int>> matriz_aux(n_trabajos_ + 1, std::vector<int>(n_maquinas_ + 1, 0));

  for(int i{1}; i <= get_trabajos(); i++) {
    int trabajo = permutacion[i - 1];
    for (int j{1}; j <= n_maquinas_; j++) {
      matriz_aux[i][j] = std::max(matriz_aux[i - 1][j], matriz_aux[i][j - 1]) + get_tiempo(trabajo, j - 1);
    }
  } 

  return matriz_aux[get_trabajos()][get_maquinas()];
}
