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

void Search::BusquedaA(const std::string& fichero_traza,
                       const std::string& fichero_camino) {
  abiertos_.clear();
  cerrados_.clear();

  Estado inicial = map_.get_inicial();
  Estado final = map_.get_final();

  std::ofstream traza(fichero_traza);

  inicial.set_g(0);
  inicial.set_f(eval(inicial));
  abiertos_.push_back(inicial);

  int it = 0;
  imprimir_iteracion(it, std::cout);
  imprimir_iteracion(it, traza);

  while (!abiertos_.empty()) {
    // 1. sacar el de menor f
    size_t mejor = 0;
    for (size_t i = 1; i < abiertos_.size(); ++i) {
      if (abiertos_[i].get_f() < abiertos_[mejor].get_f()) {
        mejor = i;
      }
    }

    cerrados_.push_back(abiertos_[mejor]);
    abiertos_.erase(abiertos_.begin() + mejor);
    const Estado& actual = cerrados_.back();

    // 2. ¿es el final?
    if (actual.get_coord() == final.get_coord()) {
      ++it;
      imprimir_iteracion(it, std::cout);
      imprimir_iteracion(it, traza);

      // reconstruir camino
      std::vector<std::pair<int, int>> camino;
      for (const Estado* p = &actual; p != nullptr; p = p->get_padre()) {
        camino.push_back(p->get_coord());
      }
      std::reverse(camino.begin(), camino.end());

      // camino y coste (pantalla y fichero de traza), coordenadas desde 1
      auto imprimir_camino = [&](std::ostream& os) {
        os << "Camino: ";
        for (size_t i = 0; i < camino.size(); ++i) {
          os << (i ? " -> " : "") << "(" << camino[i].first + 1
             << "," << camino[i].second + 1 << ")";
        }
        os << "\nCoste: " << actual.get_g() << std::endl;
      };
      imprimir_camino(std::cout);
      imprimir_camino(traza);

      // mapa con * en el camino: fichero y pantalla
      std::ofstream sal(fichero_camino);
      for (int i = 0; i < map_.get_filas(); ++i) {
        for (int j = 0; j < map_.get_cols(); ++j) {
          bool en_camino = std::find(camino.begin(), camino.end(),
                                     std::make_pair(i, j)) != camino.end();
          if (en_camino) {
            sal << "*";
            std::cout << "*";
          } else {
            sal << map_.at(i, j);
            std::cout << map_.at(i, j);
          }
          sal << (j + 1 < map_.get_cols() ? " " : "");
          std::cout << (j + 1 < map_.get_cols() ? " " : "");
        }
        sal << "\n";
        std::cout << "\n";
      }
      return;
    }

    // 3. expandir
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

  // orden: arriba, derecha, abajo, izquierda (como en la traza del enunciado)
  const int dx[] = {-1, 0, 1, 0};
  const int dy[] = {0, 1, 0, -1};

  for (int i = 0; i < 4; ++i) {
    int nx = x + dx[i], ny = y + dy[i];

    if (nx < 0 || nx >= map_.get_filas() || ny < 0 || ny >= map_.get_cols()) continue;
    if (map_.at(nx, ny) == -1) continue;

    Estado v(nx, ny, &estado);

    // llegar a la casilla destino cuesta 2 (en el fichero vale 10)
    double coste = map_.at(nx, ny);
    if (std::make_pair(nx, ny) == destino) coste = 2;

    double g = estado.get_g() + coste;
    v.set_g(g);
    v.set_f(g + eval(v));

    // saltar si ya está en cerrados_
    bool repetido = false;
    for (const Estado& c : cerrados_) {
      if (c.get_coord() == v.get_coord()) { repetido = true; break; }
    }
    if (repetido) continue;

    // si ya está en abiertos_: reemplazar si el nuevo g es menor, si no ignorar
    auto it = std::find_if(abiertos_.begin(), abiertos_.end(),
      [&](const Estado& a) { return a.get_coord() == v.get_coord(); });
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
