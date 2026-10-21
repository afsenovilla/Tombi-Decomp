// FUNC 800e847c 560 I0NFO3
// MATCHING 800e847c 560
#include "TOBJ.H"
extern unsigned char D_8009CE21;
extern unsigned char D_8009CE24;
extern unsigned char D_8009CE34;
extern signed char D_8009D2B0;

#define X o->a.p.whole
#define Y o->y.p.whole

void tunnelClampPlayerToGates(TObj *o)
{
  short y;
  short t;
  short n;
  unsigned short xx, yy;
  if (D_8009D2B0 != 3)
  {
    if (((D_8009CE21 != 0xFF) && (o->a.p.whole >= 0x3B6)) && (o->a.p.whole < 0x3E9))
    {
      y = o->y.p.whole;
      t = y + 0x1A6;
      if (((unsigned short) t) < 0x14)
      {
        if (y < (-0x19B))
        {
          o->y.p.whole = -0x1A6;
        }
        else
        {
          o->y.p.whole = -0x192;
        }
      }
    }
    if (D_8009CE24 != 0xFF)
    {
      xx = o->a.p.whole;
      if ((unsigned short)(xx - 0x30C) < 0x1F)
      {
        yy = o->y.p.whole;
        t = yy + 0x3A4;
        if (((unsigned short) t) < 0x5B)
        {
          n = -(xx + 0x5C);
          if ((short)yy > n)
          {
            t = n + 10;
            if ((short)yy < t)
            {
              o->y.p.whole = t;
            }
          }
          else
          {
            t = n - 0x14;
            if (t < (short)yy)
            {
              o->y.p.whole = t;
            }
          }
        }
      }
    }
    if (D_8009CE34 == 0)
    {
      if ((o->a.p.whole >= 0x2FC) && (o->a.p.whole < 0x36F))
      {
        t = o->y.p.whole + 0x1E4;
        if (((unsigned short) t) < 0x8B)
        {
          if (o->a.p.whole < 0x336)
          {
            o->a.p.whole = 0x2FC;
          }
          else
          {
            o->a.p.whole = 0x36E;
          }
        }
      }
      if ((o->a.p.whole >= 0x510) && (o->a.p.whole < 0x54F))
      {
        y = o->y.p.whole;
        t = y + 0x359;
        if (((unsigned short) t) < 0x65)
        {
          t = o->a.p.whole - 0x208;
          t = -t;
          if (y > t)
          {
            o->y.p.whole = t;
          }
        }
      }
      if ((o->a.p.whole >= 0x56E) && (o->a.p.whole < 0x5AB))
      {
        y = o->y.p.whole;
        t = y + 0x304;
        if (((unsigned short) t) < 0x65)
        {
          t = 0x2C8 - o->a.p.whole;
          if (y < t)
          {
            o->y.p.whole = t;
          }
        }
      }
    }
  }
}
