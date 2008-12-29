#include <camera.h>
#include <game/game.h>

#include "unit.h"
#include <cstdio>


Unit::Unit (const Point & foots, const string & xmldoc):
  m_name ("constructor"), m_body(xmldoc)
{
  Point pos(foots.x - m_body.getWidth() / 2, foots.y - m_body.getHeight());

  m_width =  m_body.getWidth();
  m_height = m_body.getHeight();
  setTopLeftCorner(pos);
  setMaxSpeed(0.75);
  m_walking = false;
}

void
Unit::startWalk (const Point & dest)
{
    m_dest = dest;
    m_walking = true;
    m_body.start ();
}

void
Unit::stopWalk ()
{
    m_body.stop ();
    m_walking = false;
}

void
Unit::refresh ()
{
    ComputeNewXY();
    Point loc = getPos();

    m_body.setLocation(loc);
    m_body.update (true);
}
