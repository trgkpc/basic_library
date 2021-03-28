#include <iostream>
#include <transform/static_transform.hpp>

using std::cout;
using std::endl;
#define print(frame)\
    cout << "===== " << #frame << " =====" << endl;\
    cout << frame << endl;\
    cout << endl;

int main()
{
  Transform::StaticTransform base(1, 0, M_PI / 2.0);
  Transform::StaticTransform inv(-1, 0, -M_PI / 2.0);
  Transform::StaticTransform add(1, 0, M_PI / 2.0);

  print(base);
  print(add);
  print(inv);
  print(base+add);
  print(-base);
  print(-base+add);
  print(inv+add);
}
