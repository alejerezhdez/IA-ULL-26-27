#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include <string>

class Instance {
  public:
    Instance();
    Instance(std::string fichero);

    int get_trabajos() const { return n_trabajos_; }
    int get_maquinas() const { return n_maquinas_; }
    int get_tiempo(int x, int y) const { return tiempos_[x][y]; }

    int cmax(const std::vector<int>& permutacion) const;

  private:
    int n_trabajos_;
    int n_maquinas_;
    std::vector<std::vector<int>> tiempos_;
};

#endif

