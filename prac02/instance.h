#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include <string>

class Instance {
  public:
    Instance();
    Instance(std::string fichero);

    int cmax(std::vector<int> permutacion);
    
  private:
    int n_trabajos_;
    int n_maquinas_;
    std::vector<std::vector<int>> tiempos_;
};

#endif
