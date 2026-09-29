#include <iostream>
#include <string>

#include "mapa.h"
#include "Search.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << " <fichero_mapa> [fichero_traza] [fichero_camino]\n";
    return 1;
  }

  std::string fichero_mapa = argv[1];
  std::string fichero_traza = (argc > 2) ? argv[2] : "traza.txt";
  std::string fichero_camino = (argc > 3) ? argv[3] : "camino.txt";

  Mapa mapa(fichero_mapa);
  Search busqueda(mapa);
  busqueda.BusquedaA(fichero_traza, fichero_camino);

  return 0;
}