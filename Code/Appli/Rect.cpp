#include"Rect.h"

const Rectangle global_rectangle(2, 3);

extern "C" int cpp_test()
{
  return global_rectangle.area();
}
