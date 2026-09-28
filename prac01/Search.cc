#include "Search.h"

Search::Search() :
  map_(Mapa()) {}

Search::Search(Mapa map) :
  map_(map) {}

void Search::BusquedaA() {
  Estado inicial = map_.get_inicial();
  Estado final = map_.get_final();


}

void Search::expandir(const Estado& estado) {
  int ejex = estado.get_coord().first;
  int ejey = estado.get_coord().second;
  bool salida_izq, salida_arriba, salida_der, salida_abajo;

  bool salida_arriba = (ejex - 1) < 1;
  bool salida_abajo = (ejex + 1) > map_.get_filas();
  bool salida_izq = (ejey - 1) < 1;
  bool salida_der = (ejey + 1) > map_.get_cols();

  double minimo{4000};
  if(!salida_arriba) {
    Estado arriba(estado.get_coord().first - 1, estado.get_coord().second, &estado);
    double f_arriba = eval(arriba);
    if (f_arriba <= minimo) {
      minimo = f_arriba;
    }
  }

  if(!salida_abajo) {
    Estado abajo(estado.get_coord().first - 1, estado.get_coord().second, &estado);
    double f_abajo = eval(abajo);
    if (f_abajo <= minimo) {
      minimo = f_abajo;
    }
  }

  if(!salida_izq) {
    Estado izq(estado.get_coord().first - 1, estado.get_coord().second, &estado);
    double f_izq = eval(izq);
    if (f_izq <= minimo) {
      minimo = f_izq;
    }
  }

  if(!salida_der) {
    Estado derecha(estado.get_coord().first - 1, estado.get_coord().second, &estado);
    double f_derecha = eval(derecha);
    if (f_derecha <= minimo) {
      minimo = f_derecha;
    }
  }
}

double Search::eval(const Estado& estado) {
  int x_i = estado.get_coord().first;
  int y_i = estado.get_coord().second;

  int x_z = map_.get_final().get_coord().first;
  int y_z = map_.get_final().get_coord().second;

  return 2 * (std::abs((x_z) - (x_i)) + std::abs((y_z) - (y_i)));
}