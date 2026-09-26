#include <vector>
#include <iostream>
#include <utility>
#include <string>

class Mapa {
  public:
    Mapa();
    Mapa(int filas, int columnas, std::string fichero_entrada);
  private:
    int num_filas_;
    int num_columnas_;
    std::vector<std::vector<int>> celdas_;
    std::pair<int, int> estado_inicial_;
    std::pair<int, int> estado_final_;
};