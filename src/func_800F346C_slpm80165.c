// VERSION SLPM_80165 x00 800F346C 772
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPM_80165)
// Demo version of func_800F6874 (src/func_800F6874_slps01144.c) without the type 31 case.
#include "TOBJ.H"

extern TObj *D_800A0C7C;
extern int rcos(int);
extern int rsin(int);

#define ORB(o, X, K) \
    do { \
        (o)->wb8 = (rcos(X) * (D_800A0C7C->box0 - 12)) >> 12; \
        (o)->wba = -(rsin((D_800A0C7C->d8c + (K)) & 0xfff) * (D_800A0C7C->box0 - 12)) >> 12; \
    } while (0)

/* register choices found with decomp-permuter */
int func_800F6874(TObj *o)
{
  int r = 0;
  register TObj *g asm("a0");
  TObj *new_var2;
  int new_var;
  unsigned short a;
  short b;
  g = D_800A0C7C;
  o->d30 = g->h->p.whole;
  o->d34 = g->y.p.whole;
  switch (g->type)
  {
    case 14:
      o->wb8 = g->d30 - g->h->p.whole;
      o->wba = g->d34 - ((unsigned short) g->y.p.whole);
      break;

    case 20:
      a = g->d8c;
      b = (a + 0x800) & 0xfff;
      new_var2 = g;
      new_var = b;
      switch (new_var2->w7a)
    {
      case 0:
        if (o->animFrame & 1)
      {
        a &= 0xfff;
        do
        {
          o->wb8 = (rcos(a) * (D_800A0C7C->box0 - 12)) >> 12;
          o->wba = (-(rsin((D_800A0C7C->d8c + 0) & 0xfff) * (D_800A0C7C->box0 - 12))) >> 12;
        }
        while (0);
        if (new_var < 0x600)
        {
          o->b69 = 1;
          r = 1;
        }
      }
      else
      {
        do
        {
          o->wb8 = (rcos(new_var) * (D_800A0C7C->box0 - 12)) >> 12;
          o->wba = (-(rsin((D_800A0C7C->d8c + 0x800) & 0xfff) * (D_800A0C7C->box0 - 12))) >> 12;
        }
        while (0);
        if (new_var > 0xa00)
        {
          o->b69 = 1;
          r = 1;
        }
      }
        break;

      case 1:
        a &= 0xfff;
        do
      {
        o->wb8 = (rcos(a) * (D_800A0C7C->box0 - 12)) >> 12;
        o->wba = (-(rsin((D_800A0C7C->d8c + 0) & 0xfff) * (D_800A0C7C->box0 - 12))) >> 12;
      }
      while (0);
        break;

      case 2:
        a += 0x400;
        a &= 0xfff;
        do
      {
        o->wb8 = (rcos(a) * (D_800A0C7C->box0 - 12)) >> 12;
        o->wba = (-(rsin((D_800A0C7C->d8c + 0x400) & 0xfff) * (D_800A0C7C->box0 - 12))) >> 12;
      }
      while (0);
        if (new_var > 0xe00)
      {
        o->b69 = 1;
        r = 1;
      }
        break;

    }

      D_800A0C7C->b69 = 1;
      break;


  }

  return r;
}
#endif
