#include <iostream>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <fstream>
#include "instance.h"
#include "genetic_algorithm.h"
#include "random_search.h"

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
  permutacion = false;
  bool aleatoria = false;
  bool convergencia = false;
  for (int i{9}; i < argc; i++) {
    if (std::string(argv[i]) == "-v") permutacion = true;
    if (std::string(argv[i]) == "-r") aleatoria = true;
    if (std::string(argv[i]) == "-c") convergencia = true;
  }

  Instance instancia(fichero);
  GeneticAlgorithm algoritmo(instancia, tamano_pob, pc, pm, tamano_tor, presupuesto, semilla, use_neh);
  RandomSearch busqueda(instancia, presupuesto, semilla);

  std::clock_t inicio = std::clock();
  std::vector<int> mejor_caso = aleatoria ? busqueda.run() : algoritmo.run();
    double tiempo_cpu = static_cast<double>(std::clock() - inicio) / CLOCKS_PER_SEC;

  if(permutacion) {
    for(int i{0}; i < mejor_caso.size(); i++) {
      std::cout << mejor_caso[i] + 1 << " ";
    }
    std::cout << std::endl;
  }

int cmax = instancia.cmax(mejor_caso);
int upper_bound = instancia.get_upper_bound();
int cmax_neh = instancia.cmax(algoritmo.neh());   // fuera del tiempo medido

std::string nombre = std::filesystem::path(fichero).stem().string();

// Con -c se guarda la curva de convergencia (evaluación, mejor cmax)
if (convergencia) {
  const std::vector<std::pair<int,int>>& h = aleatoria ? busqueda.get_historial() : algoritmo.get_historial();
  std::ofstream conv("conv_" + std::string(aleatoria ? "aleatoria" : "ag") + "_" + nombre + "_" + std::to_string(semilla) + ".csv");
  conv << "evaluaciones,cmax\n";
  for (const auto& par : h) conv << par.first << "," << par.second << "\n";
}

// Añade una fila a resultados.csv (con cabecera si el fichero no existe)
std::string ruta_csv = aleatoria ? "traza_aleatoria.csv" : "traza.csv";
bool existe = std::filesystem::exists(ruta_csv);
std::ofstream csv(ruta_csv, std::ios::app);
if (!existe) {
  csv << "instancia,n,m,cmax,ub,neh,tiempo_cpu,semilla\n";
}

double cmax_minimo = std::min<double>(cmax, cmax_minimo);
csv << nombre << "," << instancia.get_trabajos() << "," << instancia.get_maquinas() << ","
    << cmax << "," << upper_bound << "," << cmax_neh << ","
    << std::fixed << std::setprecision(3) << tiempo_cpu << "," << semilla << "\n";
  
std::cout << nombre << " " << semilla << " " << cmax << std::endl;
return 0;
}