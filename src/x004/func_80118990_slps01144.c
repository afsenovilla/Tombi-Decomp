// VERSION SLPS_01144 x04 80118990 520
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

/* register choices found with decomp-permuter */
void func_80118990_slps01144(TObj *o)
{
  short n;
  unsigned short *p;
  unsigned short *f;
  unsigned short *q;
  unsigned short *q2;
  int new_var;
  unsigned char new_var2;
  p = (unsigned short *) o->da0;
  p += 2;
  n = *p;
  f = p + 2;
  asm("" : "=r"(f) : "0"(f));
  {
    register unsigned char st asm("a2") = o->state;
    new_var2 = st;
    switch (st)
    {
      case 0:
        o->state = new_var2 + 1;
        o->timer = 10;
        o->w22 = 0;
        break;

      case 1:
        if ((--o->timer) == 0)
      {
        q = f + 8;
        o->timer = 10;
        o->w22++;
        do
        {
          unsigned short a;
          unsigned short b;
          unsigned short c;
          unsigned short d;
          int t;
          unsigned short a2;
          unsigned short b2;
          unsigned short c2;
          unsigned short d2;
          a = q[-6];
          b = q[-4];
          c = q[-2];
          d = q[0];
          t = a & 0xff00;
          q[-6] = t;
          new_var = d & 0xff00;
          t = b & 0xff00;
          q[-4] = t;
          t = c & 0xff00;
          q[-2] = t;
          t = new_var;
          q[0] = t;
          a &= 0xff;
          b &= 0xff;
          c &= 0xff;
          d &= 0xff;
          if (o->w22 & 3)
          {
            a2 = a - 0x30;
            b2 = b - 0x30;
            c2 = c - 0x30;
            d2 = d - 0x30;
          }
          else
          {
            a2 = a + 0x90;
            b2 = b + 0x90;
            c2 = c + 0x90;
            d2 = d + 0x90;
          }
          q[-6] |= a2;
          q[-4] |= b2;
          q[-2] |= c2;
          q[0] |= d2;
          q += 26;
        }
        while (--n);
      }
        break;

      case 2:
        q2 = f + 8;
        do
      {
        unsigned short a;
        unsigned short b;
        unsigned short c;
        unsigned short d;
        int t;
        unsigned short a2;
        unsigned short b2;
        unsigned short c2;
        unsigned short d2;
        a = q2[-6];
        b = q2[-4];
        c = q2[-2];
        d = q2[0];
        t = a & 0xff00;
        q2[-6] = t;
        t = b & 0xff00;
        q2[-4] = t;
        t = c & 0xff00;
        q2[-2] = t;
        t = d & 0xff00;
        q2[0] = t;
        a &= 0xff;
        b &= 0xff;
        c &= 0xff;
        d &= 0xff;
        {
          int k = (o->w22 & 3) * 0x30;
          q2[-6] |= a + k;
          q2[-4] |= b + k;
          q2[-2] |= c + k;
          q2[0] |= d + k;
        }
        q2 += 26;
      }
      while (--n);
        break;

    }

  }
}
#endif
