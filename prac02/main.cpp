#include <iostream>
#include "instance.h"
#include "genetic_algorithm.h"

int main (int argc, char* argv[]) {
  std::string fichero;
  int tamano_pob, tamano_tor, presupuesto, semilla;
  double pc, pm;
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

  std::cout << "Leyendo fichero: " << fichero << std::endl;
  Instance instancia(fichero);
  std::cout << "Instancia creada" << std::endl;
  GeneticAlgorithm algoritmo(instancia, tamano_pob, pc, pm, tamano_tor, presupuesto, semilla);

  std::vector<int> mejor_caso = algoritmo.run();

  for(int i{0}; i < mejor_caso.size(); i++) {
    std::cout << mejor_caso[i] + 1 << " ";
     
  }
  std::cout << std::endl;
  std::cout << "Mejor makespan: " << instancia.cmax(mejor_caso) << std::endl;
  return 0;
}