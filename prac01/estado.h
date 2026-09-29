#ifndef ESTADO_H
#define ESTADO_H

#include <iostream>
#include <utility>

class Estado {
  public:
    Estado(int coord_x, int coord_y, const Estado* estado = nullptr);
    Estado();

    std::pair<int, int> get_coord() const;
    
    void set_f(double f);
    void set_g(double g);

    double get_f() const;
    double get_g() const;

    const Estado* get_padre() const;
  private:
    int eje_x_;
    int eje_y_;
    const Estado* padre_;
    double f_;
    double g_;
};

#endif