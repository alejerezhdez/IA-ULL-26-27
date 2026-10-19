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

int GeneticAlgorithm::seleccion() {
  int mejor_posicion_torneo{0};
  int mejor_cmax_torneo{INFINITO};
  std::uniform_int_distribution<int> dist(0, tam_poblacion_ - 1);
  for (int i{0}; i < tam_torneo_; i++) {
    int candidato = dist(generador_);
    int cmax_auxiliar = makespan_[candidato];
    if (cmax_auxiliar < mejor_cmax_torneo) {
      mejor_posicion_torneo = candidato;
      mejor_cmax_torneo = cmax_auxiliar;
    }
  }
  return mejor_posicion_torneo;
}

void GeneticAlgorithm::cruceOX(int indice_p1, int indice_p2) {
  const std::vector<int>& p1 = poblacion_[indice_p1];
  const std::vector<int>& p2 = poblacion_[indice_p2];

  // Con probabilidad 1 - pc no se cruza: los hijos son copias de los padres
  std::uniform_real_distribution<double> prob(0.0, 1.0);
  if (prob(generador_) >= prob_cruce_) {
    hijos_.push_back(p1);
    hijos_.push_back(p2);
    return;
  }

  // Dos puntos de corte a <= b
  std::uniform_int_distribution<int> dist(0, instancia_.get_trabajos() - 1);
  int cota1 = dist(generador_);
  int cota2 = dist(generador_);
  if (cota1 > cota2) {
    std::swap(cota1, cota2);
  }

  // El segundo hijo intercambia los papeles de los padres, con los mismos cortes
  hijos_.push_back(construirHijo(p1, p2, cota1, cota2));
  hijos_.push_back(construirHijo(p2, p1, cota1, cota2));
}

std::vector<int> GeneticAlgorithm::construirHijo(const std::vector<int>& donante_segmento, const std::vector<int>& donante_relleno, int a, int b) {
  int n = instancia_.get_trabajos();
  std::vector<int> hijo(n, -1);
  std::vector<bool> usado(n, false);

  // 1. Copiar el segmento [a..b] del primer padre, en las mismas posiciones
  for (int k{a}; k <= b; k++) {
    hijo[k] = donante_segmento[k];
    usado[donante_segmento[k]] = true;
  }

  // 2. Rellenar los huecos con el orden del otro padre
  int pos{0};
  for (int k{0}; k < n; k++) {
    int trabajo = donante_relleno[k];
    if (usado[trabajo]) continue;      // ya está en el hijo
    while (hijo[pos] != -1) pos++;     // siguiente hueco libre
    hijo[pos] = trabajo;
  }
  return hijo;
}

void GeneticAlgorithm::mutacion(int h) {
  std::uniform_real_distribution<double> p(0.0, 1.0);
  if (p(generador_) >= prob_mutacion_) {
    return;
  }
  
  std::uniform_int_distribution<int> dist(0, instancia_.get_trabajos() - 1);
  int cota1 = dist(generador_);
  int cota2 = dist(generador_);
  
  if (cota1 > cota2) {
    std::swap(cota1, cota2);
  }

  std::bernoulli_distribution moneda(0.5);
  if (moneda(generador_)) {
    int trabajo = hijos_[h][cota1];
    hijos_[h].erase(hijos_[h].begin() + cota1);
    hijos_[h].insert(hijos_[h].begin() + cota2, trabajo);
  } else {
    std::swap(hijos_[h][cota1], hijos_[h][cota2]);
  }
}

void GeneticAlgorithm::reemplazo() {
  int pos_peor{0};
  int peor_cmax{-1};
  for (int i{0}; i < makespans_hijos_.size(); i++) {
    if (makespans_hijos_[i] > peor_cmax) {
      pos_peor = i;
      peor_cmax = makespans_hijos_[i];
    }
  }

  int pos_mejor_pob{0};
  int mejor_cmax_pob{INFINITO};
  for (int j{0}; j < makespan_.size(); j++) {
    if (makespan_[j] < mejor_cmax_pob) {
      pos_mejor_pob = j;
      mejor_cmax_pob = makespan_[j];
    }
  }

  if (mejor_cmax_pob < peor_cmax) {
    hijos_[pos_peor] = poblacion_[pos_mejor_pob];
    makespans_hijos_[pos_peor] = makespan_[pos_mejor_pob];
  }

  poblacion_ = hijos_;
  makespan_ = makespans_hijos_;

  hijos_.clear();
  makespans_hijos_.clear();

  for (int k{0}; k < poblacion_.size(); k++) {
    if (makespan_[k] < mejor_cmax_) {
      mejor_cmax_ = makespan_[k];
      mejor_individuo_ = poblacion_[k];
    }
  }
}

bool GeneticAlgorithm::es_factible(const std::vector<int>& permutacion) {
  int n = instancia_.get_trabajos();
  if (static_cast<int>(permutacion.size()) != n) {
    return false;
  }

  std::vector<bool> visto(n, false);
  for (int i{0}; i < n; i++) {
    int trabajo = permutacion[i];
    if (trabajo < 0 || trabajo >= n) {
      return false;               
    }
    if (visto[trabajo]) {
      return false;               
    }
    visto[trabajo] = true;
  }
  return true;
}

void GeneticAlgorithm::run() {
  inicializar();
}