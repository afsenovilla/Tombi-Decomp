// VERSION PCPX_96084 tomba 800252F4 268
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// HUD state init (block at D_800E60E8): the seven sprite tables at 0x80011B0C.., a 600-frame timer, the counters
// from D_800DE0E4, three cleared rows of flags.
extern unsigned char D_800E60E8[];
extern unsigned char D_800E60EA, D_800E60EB;
extern int D_800E612C;
extern short D_800E6134, D_800E6132, D_800E613A, D_800E6138, D_800E613C;
extern void *D_800E6104, *D_800E6108, *D_800E610C, *D_800E6110, *D_800E6114, *D_800E6118, *D_800E611C;
extern int D_800E6100;
extern char D_80011B0C[], D_80011B30[], D_80011B44[], D_80011B6C[], D_80011BC0[], D_80011D80[], D_80011E18[];
extern unsigned char D_800DE0E4;
extern int D_1F800160;

void func_800252F4_pcpx96084(void)
{
    int i;
    unsigned short n;
    unsigned char *b = D_800E60E8;
    b[0] = 1;
    D_800E612C = -1;
    D_800E6134 = 0x258;
    D_800E6104 = D_80011B0C;
    D_800E6108 = D_80011B30;
    D_800E610C = D_80011B44;
    D_800E6110 = D_80011B6C;
    D_800E6114 = D_80011BC0;
    D_800E6118 = D_80011D80;
    D_800E60EA = 0;
    D_800E60EB = 0;
    D_800E6132 = 0;
    D_800E611C = D_80011E18;
    D_800E6100 = D_1F800160;
    n = D_800DE0E4;
    D_800E613A = n;
    D_800E6138 = n;
    D_800E613C = n;
    for (i = 0; i < 3; i++) {
        (b + i)[92] = 0;
        (b + i)[96] = 0;
        (b + i)[100] = 0;
    }
}
#endif
