// VERSION SLPM_80165 slpm80165 8001A7F0 508
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of areaMode2Handler (src/areaMode2Handler.c): the gameplay task. Its start also clears the text
// window D_800A2A18; each frame bumps the play time, runs func_8001A9EC and ends the area when the play-time limit
// (func_8010FD38) has set D_800893D7 to 2; the end clears six display colour bytes and goes to mode 7.
typedef struct Th { char pad[0x4c]; short b; unsigned short c; } Th;
extern Th *D_1F8001D4;
extern unsigned char D_1F8001CF, D_1F8001C2;
extern char D_800A2A18[];
extern unsigned char D_8009A817, D_800B0A80, D_800893D7;
extern int D_8009A810;
extern int D_8009A818;
extern unsigned short D_8009B5C0;
extern unsigned char D_8009D645, D_8009D646, D_8009D647, D_8009F6D5, D_8009F6D6, D_8009F6D7;
extern void InitSubsystems(void), func_800167F0(void), func_80018254(char *), func_800215CC(void);
extern void func_80021894(void), func_8004E8E4_slpm80165(void), func_80025FF0(void), func_8001E96C(void);
extern void func_8001A9EC(void), func_8001EB30(void), func_8001EA1C(int);

void func_8001A7F0(void)
{
    char pad[8];
    switch (D_1F8001D4->c) {
    case 0:
        InitSubsystems();
        func_800167F0();
        func_80018254(D_800A2A18);
        D_1F8001CF = 1;
        func_800215CC();
        func_80021894();
        func_8004E8E4_slpm80165();
        func_80025FF0();
        D_800B0A80 = 0;
        if (D_8009A817 != 1 || D_8009A810 == 0x30000)
            func_8001E96C();
        D_1F8001D4->c++;
        break;
    case 1:
        {
            int *p = &D_8009A818;
            *p = *p + 1;
        }
        func_8001A9EC();
        if (D_800893D7 == 2) {
            D_1F8001D4->c = 2;
            func_8001EB30();
            func_8001EA1C(0);
        } else if (D_1F8001C2) {
            volatile unsigned short *q = &D_8009B5C0;
            if ((*q & 0x800) && (*q & 8))
                D_1F8001D4->c = 2;
        }
        break;
    case 2:
        func_80018254(D_800A2A18);
        D_8009D645 = 0;
        D_8009D646 = 0;
        D_8009D647 = 0;
        D_8009F6D5 = 0;
        D_8009F6D6 = 0;
        D_8009F6D7 = 0;
        D_1F8001D4->b = 7;
        D_1F8001D4->c = 0;
        break;
    }
}
#endif
