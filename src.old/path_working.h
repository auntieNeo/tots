#ifndef PATH_H_
#define PATH_H_

#include <cstddef>

#include "common.h"
#include "movement.h"

namespace Tots
{
  // TODO: maybe abstract this so that movement dictating classes other than "Path", such as "Follow" or "LuaPath", can be used
  class Path : public Movement
  {
    public:
      Path(const std::string &name, double x, double y, double initialSpeed);
      ~Path();

      void lineTo(double x, double y, double time);
      void quadTo(double cX, double cY, double endPointX, double endPointY, double time);
      void cubicTo(double c1X, double c1Y, double c2X, double c2Y, double endPointX, double endPointY, double time);

      bool isEndOfPath();
      void tick(Entity *entity);

    private:
      enum CurveType { LINE, QUAD, CUBIC };

      class Curve
      {
        public:
          Curve() { m_nextCurve = NULL; }
          ~Curve() { delete m_nextCurve; }

          virtual CurveType type() = 0;

          Curve *m_nextCurve;
          double m_endTime, m_initialSpeed;
          DoublePoint m_endPoint;
      };

      class Line : public Curve
      {
        public:
          Line(double x, double y, double time, double speed);
          ~Line() {}

          CurveType type() { return LINE; }
      };

      class Quad : public Curve
      {
        public:
          Quad(double cX, double cY, double endPointX, double endPointY, double time, double speed);
          ~Quad() {}

          CurveType type() { return QUAD; }

          DoublePoint m_c;
      };

      class Cubic : public Curve
      {
        public:
          Cubic(double c1X, double c1Y, double c2X, double c2Y, double endPointX, double endPointY, double time, double speed);
          ~Cubic() {}

          CurveType type() { return CUBIC; }

          DoublePoint m_c1, m_c2;
      };

      DoublePoint m_initialPoint, m_curveInitialPoint;
      double m_initialSpeed, m_curveTime;
      Curve *m_firstCurve, *m_currentCurve, *m_lastCurve;
  };
}

#endif
