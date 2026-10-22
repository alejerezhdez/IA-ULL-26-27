#ifndef RANDOM_SEARCH_H
#define RANDOM_SEARCH_H

#include "instance.h"
#include <random>
#include <vector>
#include <utility>

// Búsqueda aleatoria: genera y evalúa permutaciones aleatorias hasta agotar
// el presupuesto de evaluaciones (mismo presupuesto que el AG).
class RandomSearch {
  public:
    RandomSearch(const Instance& instancia, int max_evaluaciones, unsigned semilla);

    std::vector<int> run();
    int get_evaluaciones() const { return evaluaciones_; }
    const std::vector<std::pair<int,int>>& get_historial() const { return historial_; }

  private:
    const Instance& instancia_;
    int max_evaluaciones_;
    int evaluaciones_{0};
    std::mt19937 generador_;
    std::vector<int> mejor_individuo_;
    int mejor_cmax_{2000000000};
    std::vector<std::pair<int,int>> historial_;
};

#endif