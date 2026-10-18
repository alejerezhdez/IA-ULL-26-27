#ifndef GENETIC_ALGORITHM
#define GENETIC_ALGORITHM

#include "instance.h"
#include <random>
#include <numeric>
#include <algorithm>
#include <random>
#include <cstdlib>

class GeneticAlgorithm {
  public:
    GeneticAlgorithm(const Instance& instancia, int tam_poblacion, double pc, double pm, int tam_torneo, int max_evaluaciones, unsigned semilla);

    void inicializar();
    int seleccion();
    void cruceOX(int indice_p1, int indice_p2);
    std::vector<int> construirHijo(const std::vector<int>& donante_segmento, const std::vector<int>& donante_relleno, int a, int b);
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
    std::vector<int> mejor_individuo_;
    int mejor_cmax_;
    
    std::vector<std::vector<int>> hijos_;
    std::vector<int> makespans_hijos_;

    std::vector<std::vector<int>> poblacion_;
    std::vector<int> makespan_;
};

#endif