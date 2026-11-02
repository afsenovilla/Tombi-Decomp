// FUNC 8004baa8 220 MAIN0
// MATCHING 8004baa8 220
// VERSION SLPS_01144 slps01144 80056B58 160
#if defined(VERSION_SLPS_01144)
extern int DAT_1f8003c0;
extern short DAT_1f80019e;
extern unsigned short CAT4_OBJECT_COUNT_SAVED;
extern unsigned char **CAT4_OBJECT_LIST_SAVED;
extern void func_80056874_slps01144(void *, unsigned char *);

/* JP: no object type filter (PAL switches on p[2]) */
int func_80056B58(void *o)
{
    unsigned char **q = CAT4_OBJECT_LIST_SAVED;
    unsigned char *p;
    DAT_1f8003c0 = 0;
    DAT_1f80019e = CAT4_OBJECT_COUNT_SAVED;
    while (DAT_1f80019e != 0) {
        p = *q;
        DAT_1f80019e = DAT_1f80019e - 1;
        q++;
        if (p[0] & 1) func_80056874_slps01144(o, p);
    }
    return DAT_1f8003c0;
}
#else
extern int DAT_1f8003c0;
extern short DAT_1f80019e;

extern unsigned short CAT4_OBJECT_COUNT_SAVED;
extern unsigned char **CAT4_OBJECT_LIST_SAVED;
extern void probeGrabObjectBox(void *, unsigned char *);

int probeClimbGrabObject(void *o)
{
    unsigned char **q = CAT4_OBJECT_LIST_SAVED;
    unsigned char *p;
    DAT_1f8003c0 = 0;
    DAT_1f80019e = CAT4_OBJECT_COUNT_SAVED;
    while (DAT_1f80019e != 0) {
        p = *q;
        DAT_1f80019e = DAT_1f80019e - 1;
        q++;
        if (p[0] & 1) {
            switch (p[2]) {
            case 0x10: case 0x11: case 0x19: case 0x35: case 0x37: case 0x3d: case 0x3e:
                probeGrabObjectBox(o, p);
            }
        }
    }
    return DAT_1f8003c0;
}
#endif
