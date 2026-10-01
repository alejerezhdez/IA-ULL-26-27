#include "Search.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <utility>

Search::Search() :
  map_(Mapa()) {}

Search::Search(Mapa map) :
  map_(map) {}

void Search::BusquedaA(const std::string& fichero_traza, const std::string& fichero_camino) {
  
  // Limpiamos las listas de cerrados y abiertos
  abiertos_.clear();
  cerrados_.clear();

  // Definimos el estado final e inicial
  Estado inicial = map_.get_inicial();
  Estado final = map_.get_final();

  // Abrimos el fichero que contiene el mapa
  std::ofstream traza(fichero_traza);

  // Establecemos la g del nodo inicial a 0 y evaluamos sus vecinos
  inicial.set_g(0);
  inicial.set_f(eval(inicial));

  // Incluimos a abiertos el nodo inicial
  abiertos_.push_back(inicial);

  // Imprimimos iteracion por pantalla y en el archivo
  int it = 0;
  imprimir_iteracion(it, std::cout);
  imprimir_iteracion(it, traza);

  while (!abiertos_.empty()) {
    // Sacamos el de menor f
    size_t mejor = 0;
    for (size_t i = 1; i < abiertos_.size(); ++i) {
      if (abiertos_[i].get_f() < abiertos_[mejor].get_f()) {
        mejor = i;
      }
    }

    // Incluimos el mejor nodo a cerrados y lo eliminamos de abiertos
    cerrados_.push_back(abiertos_[mejor]);
    abiertos_.erase(abiertos_.begin() + mejor);

    // Establecemos el ultimo nodo revisado como el actual
    const Estado& actual = cerrados_.back();

    // Miramos si es el nodo final
    if (actual.get_coord() == final.get_coord()) {
      ++it;
      imprimir_iteracion(it, std::cout);
      imprimir_iteracion(it, traza);

      // Reconstruimos el camino
      std::vector<std::pair<int, int>> camino;
      for (const Estado* p = &actual; p != nullptr; p = p->get_padre()) {
        camino.push_back(p->get_coord());
        // Recorremos los nodos hacia atras y los incluimos en el vector camino (Se incluye al reves)
      }
      std::reverse(camino.begin(), camino.end());

      // Funcion de imprimir camino
      auto imprimir_camino = [&](std::ostream& os) { // [&] se usa para que las variables locales se puedan utilizar
        os << "Camino: ";
        for (size_t i = 0; i < camino.size(); ++i) {
          os << (i ? " -> " : "") << "(" << camino[i].first + 1
             << "," << camino[i].second + 1 << ")";
        }
        os << "\nCoste: " << actual.get_g() << std::endl;
      };

      imprimir_camino(std::cout);
      imprimir_camino(traza);

      // Imprimir el mapa 
      std::ofstream sal(fichero_camino);
      for (int i = 0; i < map_.get_filas(); ++i) {
        for (int j = 0; j < map_.get_cols(); ++j) {
          bool en_camino = std::find(camino.begin(), camino.end(), std::make_pair(i, j)) != camino.end();
          if (en_camino) {
            sal << "\033[34m" << " *" << "\033[0m";
            std::cout << "\033[34m" << " *" << "\033[0m";
          } else {
            if (map_.at(i,j) != -1) {
            sal << " " << map_.at(i, j);
            std::cout << " " << map_.at(i, j);
            } else {
                sal << "\033[31m" << map_.at(i, j) << "\033[0m";
                std::cout << "\033[31m" << map_.at(i, j) << "\033[0m";
            }
          }
          sal << (j + 1 < map_.get_cols() ? " " : "");
          std::cout << (j + 1 < map_.get_cols() ? " " : "");
        }
        sal << "\n";
        std::cout << "\n";
      }
      return;
    }

    // Expandir si no es el final
    expandir(actual);

    ++it;
    imprimir_iteracion(it, std::cout);
    imprimir_iteracion(it, traza);
  }

  std::cout << "No hay camino" << std::endl;
  traza << "No hay camino" << std::endl;
}

void Search::expandir(const Estado& estado) {


  int x = estado.get_coord().first;
  int y = estado.get_coord().second;
  std::pair<int, int> destino = map_.get_final().get_coord();

  // Hacemos 2 vectores para recorrer a la vez y obtener los vecinos (I, Arriba, D, Abajo)
  const int dx[] = {-1, 0, 1, 0};
  const int dy[] = {0, 1, 0, -1};

  // Recorremos los vectores para sacar las coordenadas de todos los vecinos
  for (int i = 0; i < 4; ++i) {
    int nx = x + dx[i], ny = y + dy[i];

    // Comprobamos todos los casos en los que un vecino no es util o accesible
    if (nx < 0 || nx >= map_.get_filas() || ny < 0 || ny >= map_.get_cols()) continue;
    if (map_.at(nx, ny) == -1) continue;

    // Generamos el objeto v del tipo estado
    Estado v(nx, ny, &estado);

    // Obtiene el coste de la casilla ( si es la final usa 2 y no 10)
    double coste = map_.at(nx, ny);
    if (std::make_pair(nx, ny) == destino) coste = 2;

    // Calculamos la f del estado
    double g = estado.get_g() + coste;
    v.set_g(g);
    v.set_f(g + eval(v));

    // Saltar si ya está en cerrados_
    bool repetido = false;
    for (const Estado& c : cerrados_) {
      if (c.get_coord() == v.get_coord()) { repetido = true; break; }
    }
    if (repetido) continue;

    // Si ya está en abiertos_: reemplazar si el nuevo g es menor, si no ignorar
    auto it = std::find_if(abiertos_.begin(), abiertos_.end(), [&](const Estado& a) { return a.get_coord() == v.get_coord(); });
    if (it != abiertos_.end()) {
      if (v.get_g() < it->get_g()) *it = v;
      continue;
    }

    abiertos_.push_back(v);
  }
}

double Search::eval(const Estado& estado) {
  int x_i = estado.get_coord().first;
  int y_i = estado.get_coord().second;

  int x_z = map_.get_final().get_coord().first;
  int y_z = map_.get_final().get_coord().second;

  return 2 * (std::abs(x_z - x_i) + std::abs(y_z - y_i));
}

void Search::imprimir_iteracion(int it, std::ostream& os) {
  os << "Iteracion " << it << "\n-----------\nAbiertos = ";
  for (size_t i = 0; i < abiertos_.size(); ++i) {
    os << (i ? ", " : "") << "(" << abiertos_[i].get_coord().first + 1
       << "," << abiertos_[i].get_coord().second + 1 << ")";
  }
  os << "\nCerrados = ";
  bool primero = true;
  for (const Estado& c : cerrados_) {
    os << (primero ? "" : ", ") << "(" << c.get_coord().first + 1
       << "," << c.get_coord().second + 1 << ")";
    primero = false;
  }
  os << "\n------------------------\n";
}
