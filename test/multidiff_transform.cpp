#include <iostream>
#include <transform/transform.hpp>

using std::cout;
using std::endl;

#define print(frame)\
    cout << "===== " << #frame << " =====" << endl;\
    cout << frame << endl;\
    cout << endl;

int main()
{
  Transform::StaticTransform pos(1, 0, M_PI / 2.0);
  Transform::DynamicTransform vel(0, 1, 0);
  Transform::DynamicTransform acc(-1, 0, 0);

  Transform::MultidiffTransform<2> frame(pos, vel, acc);

  print(frame);

  auto [vx, vy] = vel.getPos();
  print(vx);
  print(vy);

  print(frame+pos);
}
