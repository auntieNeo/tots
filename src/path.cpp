#include "entity.h"
#include "path.h"

#include <iostream>
using namespace std;

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace Tots
{
  Path::Path(const std::string &name, double initialSpeed) : Movement(name)
  {
    m_initialSpeed = initialSpeed;
    m_firstCurve = NULL;
    m_lastCurve = NULL;
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
      m_firstCurve->m_startPoint = DoublePoint(0, 0);
      m_firstCurve->m_precedingTime = 0;
      m_lastCurve = m_firstCurve;
    }
    else
    {
      m_lastCurve->m_nextCurve = new Line(x, y, time, 0.5);  // TODO: actually calculate the initial speed
      m_lastCurve->m_nextCurve->m_startPoint = m_lastCurve->m_endPoint;
      m_lastCurve->m_nextCurve->m_precedingTime = m_lastCurve->m_precedingTime + m_lastCurve->m_time;
      m_lastCurve = m_lastCurve->m_nextCurve;
    }
  }

  Path::Line::Line(double x, double y, double time, double speed)
  {
    m_endPoint.m_x = x;
    m_endPoint.m_y = y;
    m_time = time;
    m_initialSpeed = speed;
  }

  void Path::quadTo(double cX, double cY, double endPointX, double endPointY, double time)
  {
    if(m_firstCurve == NULL)
    {
      m_firstCurve = new Quad(cX, cY, endPointX, endPointY, time, m_initialSpeed);
      m_firstCurve->m_startPoint = DoublePoint(0, 0);
      m_firstCurve->m_precedingTime = 0;
      m_lastCurve = m_firstCurve;
    }
    else
    {
      m_lastCurve->m_nextCurve = new Quad(cX, cY, endPointX, endPointY, time, 0.5);  // TODO: actually calculate the initial speed
      m_lastCurve->m_nextCurve->m_startPoint = m_lastCurve->m_endPoint;
      m_lastCurve->m_nextCurve->m_precedingTime = m_lastCurve->m_precedingTime + m_lastCurve->m_time;
      m_lastCurve = m_lastCurve->m_nextCurve;
    }
  }

  Path::Quad::Quad(double cX, double cY, double endPointX, double endPointY, double time, double speed)
  {
    m_c.m_x = cX;
    m_c.m_y = cY;
    m_endPoint.m_x = endPointX;
    m_endPoint.m_y = endPointY;
    m_time = time;
    m_initialSpeed = speed;
  }

  void Path::cubicTo(double c1X, double c1Y, double c2X, double c2Y, double endPointX, double endPointY, double time)
  {
    if(m_firstCurve == NULL)
    {
      m_firstCurve = new Cubic(c1X, c1Y, c2X, c2Y, endPointX, endPointY, time, m_initialSpeed);
      m_firstCurve->m_startPoint = DoublePoint(0, 0);
      m_firstCurve->m_precedingTime = 0;
      m_lastCurve = m_firstCurve;
    }
    else
    {
      m_lastCurve->m_nextCurve = new Cubic(c1X, c1Y, c2X, c2Y, endPointX, endPointY, time, 0.5);  // TODO: actually calculate the initial speed
      m_lastCurve->m_nextCurve->m_startPoint = m_lastCurve->m_endPoint;
      m_lastCurve->m_nextCurve->m_precedingTime = m_lastCurve->m_precedingTime + m_lastCurve->m_time;
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
    m_time = time;
    m_initialSpeed = speed;
  }

  bool isEndOfPath()
  {
    return false;
  }

  void Path::tick(Entity *entity)
  {
    DoublePoint p;
    double pathTime = (double)entity->tickCount() / FRAMES_PER_SECOND;  // FIXME: should be ticks since start of state
    Curve *currentCurve = m_firstCurve;
    double totalCurveTime = 0;
    // traverse the linked list of curves and add up their times to find the correct curve to use
    // TODO: find the current curve using ticks rather than doubles, for easier indexing and caching
    while(currentCurve != NULL && (totalCurveTime += currentCurve->m_time) < pathTime)
    {
      currentCurve = currentCurve->m_nextCurve;
    }
    if(currentCurve == NULL)
    {
      // we're out of curves, don't do anything
      // TODO: add a repeat feature
      return;
    }

    double t = (pathTime - currentCurve->m_precedingTime) / currentCurve->m_time;  // TODO: calculate t using the initial speed and the end time

    switch(currentCurve->type())
    {
      case LINE:
        break;
      case QUAD:
        {
          Quad * quad = static_cast<Quad *>(currentCurve);
          p.m_x = (1-t)*(1-t) * quad->m_startPoint.m_x + 2 * (1-t) * t * quad->m_c.m_x + (t*t) * quad->m_endPoint.m_x;
          p.m_y = (1-t)*(1-t) * quad->m_startPoint.m_y + 2 * (1-t) * t * quad->m_c.m_y + (t*t) * quad->m_endPoint.m_y;
        }
        break;
      case CUBIC:
        break;
    }

    // p is relative to the path, but the entity's new position is relative to the position of the entity at the start of its current state
    entity->setX(p.m_x + entity->xAtStartOfState());
    entity->setY(p.m_y + entity->yAtStartOfState());
  }

  /*!
   * \brief Lua equivalent: Path *Path(String name, double initialSpeed)
   */
  int Path_Path(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 2)
      return 0;

    std::string name(lua_tostring(lua, 1));
    double initialSpeed = lua_tonumber(lua, 2);
    lua_pushlightuserdata(lua, new Path(name, initialSpeed));

    return 1;
  }

  /*!
   * \brief Lua equivalent: void Path_lineTo(Path *path, double x, double y, double time)
   */
  int Path_lineTo(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 4)
      return 0;

    Path *path = static_cast<Path*>(lua_touserdata(lua, 1));
    double x = lua_tonumber(lua, 2);
    double y = lua_tonumber(lua, 3);
    double time = lua_tonumber(lua, 4);
    path->lineTo(x, y, time);

    return 0;
  }

  /*!
   * \brief Lua equivalent: void Path_quadTo(Path *path, double cX, double cY, double endPointX, double endPointY, double time)
   */
  int Path_quadTo(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 6)
      return 0;

    Path *path = static_cast<Path*>(lua_touserdata(lua, 1));
    double cX = lua_tonumber(lua, 2);
    double cY = lua_tonumber(lua, 3);
    double endPointX = lua_tonumber(lua, 4);
    double endPointY = lua_tonumber(lua, 5);
    double time = lua_tonumber(lua, 6);
    path->quadTo(cX, cY, endPointX, endPointY, time);

    return 0;
  }

  /*!
   * \brief Lua equivalent: void Path_cubicTo(Path *path, double c1X, double c1Y, double c2X, double c2Y, double endPointX, double endPointY, double time)
   */
  int Path_cubicTo(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 8)
      return 0;

    Path *path = static_cast<Path*>(lua_touserdata(lua, 1));
    double c1X = lua_tonumber(lua, 2);
    double c1Y = lua_tonumber(lua, 3);
    double c2X = lua_tonumber(lua, 4);
    double c2Y = lua_tonumber(lua, 5);
    double endPointX = lua_tonumber(lua, 6);
    double endPointY = lua_tonumber(lua, 7);
    double time = lua_tonumber(lua, 8);
    path->cubicTo(c1X, c1Y, c2X, c2Y, endPointX, endPointY, time);

    return 0;
  }

  void Path::registerLuaFunctions(lua_State *lua)
  {
    lua_register(lua, "Path", Path_Path);
    lua_register(lua, "Path_lineTo", Path_lineTo);
    lua_register(lua, "Path_quadTo", Path_quadTo);
    lua_register(lua, "Path_cubicTo", Path_cubicTo);
  }
}
