#include "random_search.h"
#include <numeric>
#include <algorithm>

RandomSearch::RandomSearch(const Instance& instancia, int max_evaluaciones, unsigned semilla) :
  instancia_(instancia), max_evaluaciones_(max_evaluaciones), generador_(semilla) {}

std::vector<int> RandomSearch::run() {
  std::vector<int> permutacion(instancia_.get_trabajos());
  std::iota(permutacion.begin(), permutacion.end(), 0);

  while (evaluaciones_ < max_evaluaciones_) {
    std::shuffle(permutacion.begin(), permutacion.end(), generador_);
    int cmax = instancia_.cmax(permutacion);
    evaluaciones_++;
    if (cmax < mejor_cmax_) {
      mejor_cmax_ = cmax;
      mejor_individuo_ = permutacion;
      historial_.push_back({evaluaciones_, mejor_cmax_});
    }
  }
  return mejor_individuo_;
}