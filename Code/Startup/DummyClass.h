#ifndef _DUMMYCLASS_H_
#define _DUMMYCLASS_H_


#ifdef __cplusplus

class DummyClass
{
  public:
    DummyClass(const int w = 0,
               const int h = 0) : a(w),
                                  b(h) { }

    ~DummyClass() { }

    int mul(void) const
    {
      return a * b;
    }

private:
  int a;
  int b;
};

#endif

#endif
