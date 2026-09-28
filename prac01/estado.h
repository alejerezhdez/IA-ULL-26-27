#include <iostream>
#include <utility>

#ifndef ESTADO_H
#define ESTADO_H

class Estado {
  public:
    Estado(int coord_x, int coord_y, const Estado* estado);
    Estado();

    std::pair<int, int> get_coord() const;

  private:
    int eje_x_;
    int eje_y_;
    const Estado* padre_;
};

#endif