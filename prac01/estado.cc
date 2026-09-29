#include "estado.h"

Estado::Estado(int coord_x, int coord_y, const Estado* estado) :
  eje_x_(coord_x), eje_y_(coord_y), padre_(estado) {}

Estado::Estado() :
  eje_x_(0), eje_y_(0), padre_(nullptr) {}

std::pair<int, int> Estado::get_coord() const {
  return std::make_pair(eje_x_, eje_y_);
}

void Estado::set_f(double f) {
  f_ = f;
}
    
void Estado::set_g(double g) {
  g_ = g;
}

double Estado::get_f() const {
  return f_;
}
    
double Estado::get_g() const {
  return g_;
}

const Estado* Estado::get_padre() const {
  return padre_;
}