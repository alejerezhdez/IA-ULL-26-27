#ifndef GENETIC_ALGORITHM
#define GENETIC_ALGORITHM

#include "instance.h"
#include <random>

class GeneticAlgorithm {
  public:
    GeneticAlgorithm(const Instance& instancia, int tam_poblacion, double pc, double pm, int tam_torneo, int max_evaluaciones, unsigned semilla);

    void inicializar();
    int seleccion();
    void cruceOX(int p1, int p2);
    void mutacion(int h);
    void reemplazo();
    bool es_factible(const std::vector<int>& permutacion);

  private:
    const Instance& instancia_;
    int tam_poblacion_;
    const int max_evaluaciones_;
    double prob_cruce_;
    double prob_mutacion_;
    int tam_torneo_;
    int evaluaciones_{0};
    std::mt19937 generador_;
    
    std::vector<std::vector<int>> hijos_;
    std::vector<int> makespans_hijos_;

    std::vector<std::vector<int>> poblacion_;
    std::vector<int> makespan_;
};

#endif