#include "genetic_algorithm.h"

static constexpr int INFINITO = 1000000000;

GeneticAlgorithm::GeneticAlgorithm(const Instance& instancia, int tam_poblacion, double pc, double pm, int tam_torneo, int max_evaluaciones, unsigned semilla) :
  instancia_(instancia), tam_poblacion_(tam_poblacion), prob_cruce_(pc), prob_mutacion_(pm), tam_torneo_(tam_torneo), max_evaluaciones_(max_evaluaciones), generador_(semilla) {}

void GeneticAlgorithm::inicializar() {
  int pos_mejor{0};
  int mejorcmax{INFINITO};
  for (int i{0}; i < tam_poblacion_; i++) {
    std::vector<int> auxiliar(instancia_.get_trabajos());
    std::iota(auxiliar.begin(), auxiliar.end(), 0);
    std::shuffle(auxiliar.begin(), auxiliar.end(), generador_);

    int cmax_temp = instancia_.cmax(auxiliar);
    if (cmax_temp < mejorcmax) {
      pos_mejor = i;
      mejorcmax = cmax_temp;
    }

    poblacion_.push_back(auxiliar);
    makespan_.push_back(cmax_temp);
    evaluaciones_++;
  }
  mejor_individuo_ = poblacion_[pos_mejor];
  mejor_cmax_ = mejorcmax;
}