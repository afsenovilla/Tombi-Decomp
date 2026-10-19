// FUNC 800ebc30 684 I0NFO
#include "TOBJ.H"
#include "raw7.h"
extern signed char D_8009D2B0;
extern TObj D_800A6038;
extern signed char D_800A6047;

#define LST(i) (*(TObj **)((int)list + ((i) << 2)))
void func_800EBC30(void)
{
  short k;
  TObj *list[64];
  TObj *o;
  unsigned short total;
  unsigned short by;
  short i;
  short j;
  short best;
  short m;
  TObj **pp;
  TObj *c;
  for (i = 0; i < 0x40; i++)
  {
    *((TObj **) (((int) list) + (i << 2))) = 0;
  }

  i = 0;
  total = (k = *((unsigned short *) 0x1F800246));
  pp = *((TObj ***) 0x1F80021C);
  while (k != 0)
  {
    TObj *o = *(pp++);
    k--;
    if (!((o->type == 0x19) || (((unsigned int) (o->type - 0x25)) < 2)))
    {
      total--;
      continue;
    }
    if (o->_pad0e[0] != 0)
    {
      total--;
      continue;
    }
    *((TObj **) (((int) list) + ((i++) << 2))) = o;
  }

  k = *((unsigned short *) 0x1F80024A);
  pp = *((TObj ***) 0x1F800220);
  total += k;
  while (k != 0)
  {
    TObj *o = *(pp++);
    k--;
    if (!((o->type == 0x2E) || (o->type == 0x1D)))
    {
      total--;
      continue;
    }
    if (o->_pad0e[0] != 0)
    {
      total--;
      continue;
    }
    *((TObj **) (((int) list) + ((i++) << 2))) = o;
  }

  if (D_8009D2B0 != 3)
  {
    total++;
    *((TObj **) (((int) list) + (i << 2))) = &D_800A6038;
  }
  for (m = 0; m < ((short) total); m++)
  {
    best = m;
    by = (*((TObj **) (((int) list) + (m << 2))))->y.p.whole;
    for (j = m; j < ((short) total); j++)
    {
      if ((*((TObj **) (((int) list) + (j << 2))))->y.p.whole > ((short) by))
      {
        pp = list;
        best = j;
        by = (*((TObj **) (((int) pp) + (j << 2))))->y.p.whole;
      }
    }

    c = list[m];
    list[m] = list[best];
    list[best] = c;
  }

  for (i = 0; (*((TObj **) ((i << 2) + ((int) list)))) != 0; i++)
  {
    o = *((TObj **) (((int) list) + (i << 2)));
    ;
    o->b0f = i - 10;
    if (((TObj *) (*((void **) (((char *) o) + 0x94)))) != 0)
    {
      ((TObj *) (*((void **) (((char *) o) + 0x94))))->b0f = i - 9;
    }
  }

  if (D_8009D2B0 == 3)
  {
    D_800A6047 = -0xC;
  }
}
