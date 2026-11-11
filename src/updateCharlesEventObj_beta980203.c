// VERSION BETA_980203 x00 8013B7FC 352
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/updateCharlesEventObj_scus94236.c (updateCharlesEventObj, twin score 0.900) [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL updateCharlesEventObj.c (src/updateCharlesEventObj.c): extern unsigned char DAT_a, DAT_b, DAT_c;=>extern unsigned char D_8009C1B2, D_8009C225, D_8009C376;; DAT_a =>D_8009C1B2 ; DAT_b <=>D_8009C225 <; DAT_c =>D_8009C376 ; FUN_80018980=>PoolFree_1F800210.
extern unsigned char D_8009C1B2, D_8009C225, D_8009C376;
extern unsigned char *allocObjectLayer2(void);
extern short charlesTalkCheck(void *);
extern void updateCharlesTalkScene(void *);
extern void PoolFree_1F800210(void *);

void updateCharlesEventObj(unsigned char *o)
{
    unsigned char *q;
    unsigned char **p;
    switch (o[4]) {
    case 0:
        p = (unsigned char **)(o + 0x1c);
        if (D_8009C1B2 == 0xff)
            o[4] = 3;
        if (D_8009C225 < 6) {
            q = allocObjectLayer2();
            if (q != 0) {
                q[0] = 1;
                q[2] = 0x15;
                if (D_8009C376 == 0) {
                    *(short *)(q + 0x2e) = 1;
                    *(short *)(q + 0x12) = 0x140;
                    *(short *)(q + 0x16) = -0x32;
                    *(short *)(q + 0x1a) = 0;
                    q[3] = 0x63;
                } else {
                    *(short *)(q + 0x12) = 0x138;
                    *(short *)(q + 0x2e) = 0;
                    *(short *)(q + 0x16) = -0x11c;
                    *(short *)(q + 0x1a) = 0;
                    q[3] = 0;
                }
                *p = q;
            }
            o[4] = 1;
        } else
            o[4] = 3;
        break;
    case 1:
        if (charlesTalkCheck(o) != 0)
            updateCharlesTalkScene(o);
        break;
    case 2:
        o[4] = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
#endif
