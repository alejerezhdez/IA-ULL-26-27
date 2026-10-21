#include <iostream>
#include <filesystem>
#include "instance.h"
#include "genetic_algorithm.h"

int main (int argc, char* argv[]) {
  std::string fichero;
  int tamano_pob, tamano_tor, presupuesto, semilla;
  double pc, pm;
  bool permutacion, use_neh;
  if (argc <= 1) {
    std::cout << "Error, uso incorrecto del programa, para un uso correcto: ./programa archivo_instancia tamaño_poblacion probabilidadcruce probabilidadmutacion tamañotorneo presupuestoevaluaciones semilla" << std::endl;
    return 1;
  }
  fichero = argv[1];
  tamano_pob = std::stoi(argv[2]);
  pc = std::stod(argv[3]);
  pm = std::stod(argv[4]);
  tamano_tor = std::stoi(argv[5]);
  presupuesto = std::stoi(argv[6]);
  semilla = std::stoi(argv[7]);
  use_neh = (std::stoi(argv[8]) == 1);
  std::string escribirpermutacion = "-v";
  if (argc == 10 && argv[9] == escribirpermutacion) {
    permutacion = true;
  } else  {
    permutacion = false;
  }

  Instance instancia(fichero);
  GeneticAlgorithm algoritmo(instancia, tamano_pob, pc, pm, tamano_tor, presupuesto, semilla, use_neh);

  std::vector<int> mejor_caso = algoritmo.run();

  if(permutacion) {
    for(int i{0}; i < mejor_caso.size(); i++) {
      std::cout << mejor_caso[i] + 1 << " ";
    }
    std::cout << std::endl;
  }

  int cmax = instancia.cmax(mejor_caso);
  int upper_bound = instancia.get_upper_bound();

  double rpd = 100.0 * (cmax - upper_bound) / upper_bound;

  std::string nombre = std::filesystem::path(fichero).stem().string();

  std::cout << nombre << " " << semilla << " " << cmax << std::endl;
  return 0;
}