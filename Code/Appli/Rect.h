#ifndef _RECT_H_
#define _RECT_H_


#ifdef __cplusplus

class Rectangle
{
  public:
    Rectangle(const int w = 0,
              const int h = 0) : width(w),
                                 height(h) { }

    ~Rectangle() { }

    int area(void) const
    {
      return width * height;
    }

private:
  int width;
  int height;
};

#endif

#endif
