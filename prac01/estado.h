#include <iostream>

class Estado {
  public:
    Estado(int coord_x, int coord_y, Estado* estado);
    Estado();
  private:
    int eje_x_;
    int eje_y_;
    Estado* padre_;
};