// VERSION PCPX_96084 tomba 80018550 328
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of ObjCullRegister (src/ObjCullRegister.c): screen origin at 0x1F80028A / 0x1F800392, nine draw lists.
extern unsigned short D_1F80028A, D_1F800392;
extern void ObjListPush_1F800218(unsigned char *), ObjListPush_1F80021C(unsigned char *), ObjListPush_1F800224(unsigned char *),
    ObjListPush_1F800230(unsigned char *), ObjListPush_1F800228(unsigned char *), func_8001ABE4_pcpx96084(unsigned char *),
    func_8001AC20_pcpx96084(unsigned char *), func_8001AC5C_pcpx96084(unsigned char *), ObjListPush_1F80022C(unsigned char *);

int func_80018550(unsigned char *o)
{
    if (o[0] == 0)
        return 0;
    o[1] = 0;
    if ((unsigned short)(*(unsigned short *)(*(int *)(o + 0x40) + 2) - D_1F80028A + 0x40) >= 0x1c1)
        return 0;
    if ((unsigned short)(D_1F800392 - *(unsigned short *)(o + 0x16) + 0x40) >= 0x171)
        return 0;
    o[1] = 1;
    switch (o[0x1c] & 0x7f) {
    case 1: ObjListPush_1F800218(o); return 1;
    case 2: ObjListPush_1F80021C(o); return 1;
    case 3: ObjListPush_1F800224(o); return 1;
    case 4: ObjListPush_1F800230(o); return 1;
    case 5: ObjListPush_1F800228(o); return 1;
    case 6: func_8001ABE4_pcpx96084(o); return 1;
    case 7: func_8001AC20_pcpx96084(o); return 1;
    case 8: func_8001AC5C_pcpx96084(o); return 1;
    case 9: ObjListPush_1F80022C(o);
    default: return 1;
    }
}
#endif
