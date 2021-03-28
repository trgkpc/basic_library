#include <iostream>
#include <Eigen/Dense>
#include <utility>

using namespace Eigen;
struct MyVector
{
    MyVector(double x, double y) : x(x), y(y)
    {
    }

    double getX() const
    {
        return x;
    }

    double getY() const
    {
        return y;
    }

    std::pair<double, double> pairing() const
    {
        return std::make_pair<double, double>(getX(), getY());
    }

    double x;
    double y;
};

int main()
{
  Matrix2d a;
  a << 1, 2,
       3, 4;
  MatrixXd b(2,2);
  b << 2, 3,
       1, 4;
  std::cout << "a + b =\n" << a + b << std::endl;
  std::cout << "a - b =\n" << a - b << std::endl;
  std::cout << "Doing a += b;" << std::endl;
  a += b;
  std::cout << "Now a =\n" << a << std::endl;
  Vector3d v(1,2,3);
  Vector3d w(1,0,0);
  std::cout << "-v + w - v =\n" << -v + w - v << std::endl;

  /*
  Vector2d r(2.3, 3.4);
  auto [x, y] = r;
  std::cout << "x,y =" << "(" << x << "," << y << ")" << std::endl;
  */
  MyVector r(1., 2.);
  auto [x, y] = r.pairing();
  std::cout << "x,y =" << "(" << x << "," << y << ")" << std::endl;
}

