#include "path.h"

namespace Tots
{
  Path::Path(double x, double y, double initialSpeed)
  {
    m_initialPoint.m_x = x;
    m_initialPoint.m_y = y;
    m_curveInitialPoint = m_initialPoint;
    m_initialSpeed = initialSpeed;
    m_firstCurve = NULL;
    m_currentCurve = NULL;
    m_lastCurve = NULL;
    m_curveTime = 0;
  }

  Path::~Path()
  {
    delete m_firstCurve;
  }

  void Path::lineTo(double x, double y, double time)
  {
    if(m_firstCurve == NULL)
    {
      m_firstCurve = new Line(x, y, time, m_initialSpeed);
      m_lastCurve = m_currentCurve = m_firstCurve;
    }
    else
    {
      m_lastCurve->m_nextCurve = new Line(x, y, time, 0.5);  // TODO: actually calculate the initial speed
      m_lastCurve = m_lastCurve->m_nextCurve;
    }
  }

  Path::Line::Line(double x, double y, double time, double speed)
  {
    m_endPoint.m_x = x;
    m_endPoint.m_y = y;
    m_endTime = time;
    m_initialSpeed = speed;
  }

  void Path::quadTo(double cX, double cY, double endPointX, double endPointY, double time)
  {
    if(m_firstCurve == NULL)
    {
      m_firstCurve = new Quad(cX, cY, endPointX, endPointY, time, m_initialSpeed);
      m_lastCurve = m_currentCurve = m_firstCurve;
    }
    else
    {
      m_lastCurve->m_nextCurve = new Quad(cX, cY, endPointX, endPointY, time, 0.5);  // TODO: actually calculate the initial speed
      m_lastCurve = m_lastCurve->m_nextCurve;
    }
  }

  Path::Quad::Quad(double cX, double cY, double endPointX, double endPointY, double time, double speed)
  {
    m_c.m_x = cX;
    m_c.m_y = cY;
    m_endPoint.m_x = endPointX;
    m_endPoint.m_y = endPointY;
    m_endTime = time;
    m_initialSpeed = speed;
  }

  void Path::cubicTo(double c1X, double c1Y, double c2X, double c2Y, double endPointX, double endPointY, double time)
  {
    if(m_firstCurve == NULL)
    {
      m_firstCurve = new Cubic(c1X, c1Y, c2X, c2Y, endPointX, endPointY, time, m_initialSpeed);
      m_lastCurve = m_currentCurve = m_firstCurve;
    }
    else
    {
      m_lastCurve->m_nextCurve = new Cubic(c1X, c1Y, c2X, c2Y, endPointX, endPointY, time, 0.5);  // TODO: actually calculate the initial speed
      m_lastCurve = m_lastCurve->m_nextCurve;
    }
  }

  Path::Cubic::Cubic(double c1X, double c1Y, double c2X, double c2Y, double endPointX, double endPointY, double time, double speed)
  {
    m_c1.m_x = c1X;
    m_c1.m_y = c1Y;
    m_c2.m_x = c2X;
    m_c2.m_y = c2Y;
    m_endPoint.m_x = endPointX;
    m_endPoint.m_y = endPointY;
    m_endTime = time;
    m_initialSpeed = speed;
  }

  bool isEndOfPath()
  {
    return false;
  }

  DoublePoint Path::step(double seconds)
  {
    if(m_currentCurve == NULL)
    {
      return m_curveInitialPoint;
    }
    m_curveTime += seconds;
    if(m_curveTime > m_currentCurve->m_endTime)
    {
      m_curveInitialPoint = m_currentCurve->m_endPoint;
      m_currentCurve = m_currentCurve->m_nextCurve;
      m_curveTime = seconds;
      if(m_currentCurve == NULL)
      {
        return m_curveInitialPoint;
      }
    }

    double t = m_curveTime / m_currentCurve->m_endTime;  // TODO: calculate t using the initial speed and the end time

    DoublePoint p;

    switch(m_currentCurve->type())
    {
      case LINE:
        break;
      case QUAD:
        {
          Quad * quad = static_cast<Quad *>(m_currentCurve);
          p.m_x = (1-t)*(1-t) * m_curveInitialPoint.m_x + 2 * (1-t) * t * quad->m_c.m_x + (t*t) * quad->m_endPoint.m_x;
          p.m_y = (1-t)*(1-t) * m_curveInitialPoint.m_y + 2 * (1-t) * t * quad->m_c.m_y + (t*t) * quad->m_endPoint.m_y;
        }
        break;
      case CUBIC:
        break;
    }

    return p;
  }
}
