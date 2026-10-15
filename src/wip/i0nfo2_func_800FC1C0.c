// FUNC 800fc1c0 680 I0NFO2
#include "TOBJ.H"
#include "raw7.h"
typedef struct {
    short type, w2, frame, w6, w8, wA, wC, wE, w10, w12, x, y, sub, bc;
} SpawnA;
typedef struct {
    short type, w2, w4, w6, x, y, sub, bc;
} SpawnB;
extern TObj *func_800183B8();
extern TObj *func_80018448();
extern unsigned char D_8009CDF1;
extern unsigned char D_8009CE0A;
extern unsigned char D_8009CF9F;
extern unsigned char D_8009CFA0;
extern unsigned char D_8009CFA1;
extern unsigned char D_8009CFA5;
extern TObj *D_8009F2D8[];
extern SpawnB D_80100368[];
extern SpawnA *D_80100784;

void func_800FC1C0(TObj *parent)
{
  SpawnA *tbl;
  SpawnA *p;
  SpawnB *e;
  TObj *o;
  short i;
  int m;
  if (D_8009CE0A != 0)
  {
    m = 1;
    if (D_8009CDF1 == 0xFF)
    {
      m = 2;
    }
  }
  else
  {
    m = 0;
  }
  tbl = D_80100784;
  D_8009CFA5 = m;
  p = tbl;
  i = 0;
  m = 0xFF;
  while (p->type != m)
  {
    o = func_800183B8();
    if (o != 0)
    {
      o->active = 1;
      o->type = 0x30;
      o->b0a = 0xD;
      o->animFrame = p->frame;
      o->subtype = p->sub;
      o->b0c = p->bc;
      o->_pad0e[0] = p->type;
      o->a.p.whole = p->x;
      o->b.p.whole = 0;
      o->y.p.whole = p->y;
      o->wb4 = p->w6;
      o->wb6 = p->w2;
      o->wb8 = p->w12;
      o->wba = p->w10;
      o->w74 = p->w8;
      o->w76 = p->wA;
      o->w78 = p->wC;
      o->w7a = p->wE;
      D_8009F2D8[i] = o;
      o->d90 = (int) parent;
    }
    i++;
    p = &tbl[i];
  }

  i = 0;
  for (e = D_80100368; e->type != 0xFF; e++)
  {
    o = func_80018448();
    if (o != 0)
    {
      o->active = 1;
      o->type = 0x1D;
      o->b0a = 0xD;
      o->animFrame = 0;
      o->subtype = e->sub;
      o->b0c = e->bc;
      o->_pad0e[0] = e->type;
      o->a.p.whole = e->x;
      o->b.p.whole = 0;
      o->y.p.whole = e->y;
      o->wb4 = e->w6;
      if (i == 0xF)
      {
        o->wb4 = e->w6 + D_8009CFA1;
      }
      o->wb8 = i++;
      o->wb6 = e->w2;
    }
  }

  D_8009CF9F = 0;
  D_8009CFA0 = 0;
}
