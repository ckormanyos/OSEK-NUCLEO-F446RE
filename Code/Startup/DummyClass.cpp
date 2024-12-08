#include"DummyClass.h"

const DummyClass global_DummyClass(2, 3);

extern "C" int dummy_cpptest()
{
    return global_DummyClass.mul();
}
