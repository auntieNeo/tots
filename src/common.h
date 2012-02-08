#ifndef COMMON_H_
#define COMMON_H_

#define FRAMES_PER_SECOND 30

#define MAX_VELOCITY (double)800
#define ACCELERATION (double)8000
#define FRICTION (double)3000

#define MIN(a,b) ((a) > (b) ? (b) : (a))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define ABS(a) ((a) < 0 ? -(a) : (a))
#define SIGN(a) ((a) < 0 ? -1 : 1)

class IntPoint
{
  public:
    IntPoint() {}
    IntPoint(int x, int y) { m_x = x; m_y = y; }
    int m_x, m_y;
};

class DoublePoint
{
  public:
    DoublePoint() {}
    DoublePoint(double x, double y) { m_x = x; m_y = y; }
    double m_x, m_y;
};

#endif
