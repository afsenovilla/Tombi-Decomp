// VERSION SLPS_01144 optsub00 800E9D08 592
#if defined(VERSION_SLPS_01144)
// JP optionsMenuStep (PAL o0ptsub0 800e8284): no vibration rows, pad bits 0x1000/0x4000/0x40/0x20
typedef struct S { char p[0x48]; short w48; unsigned short w4a; unsigned short w4c; unsigned short w4e; } S;
asm(".set TC_,0x1f8001d4");
asm(".set JS_,0x1f8001fc");
extern S *TC_;
extern unsigned short JS_;
extern unsigned char D_8009F20C, D_800A0C78;
extern void func_8006E0F4(void); /* SsSetMono */
extern void func_8006E108(void); /* SsSetStereo */
extern void func_8001E81C(int id); /* keyOnSFX */
extern void func_8001E2D4(int id); /* playSFX */
extern void func_8001E910(int a, int b); /* playSFXAndSetNote */
extern void func_8001DAF0(void); /* updateSound */
extern void optionsMenuDraw(int);

int func_800E9D08(void)
{
    unsigned short b;

    switch (TC_->w4e) {
    case 0:
        TC_->w4e++;
        break;
    case 1:
        b = JS_;
        if (b & 0x4000) {
            func_8001E2D4(8);
            TC_->w4e++;
        } else if (b & 0x40) {
            func_8001E910(10, 10);
            return 1;
        } else if (b & 0x20) {
            func_8001E81C(8);
            if (++D_8009F20C >= 3) D_8009F20C = 0;
        }
        func_8001DAF0();
        break;
    case 2:
        b = JS_;
        if (b & 0x1000) {
            func_8001E2D4(8);
            TC_->w4e--;
        } else if (b & 0x4000) {
            func_8001E2D4(8);
            TC_->w4e++;
        } else if (b & 0x40) {
            func_8001E910(10, 10);
            return 1;
        } else if (b & 0x20) {
            func_8001E2D4(8);
            if (++D_800A0C78 >= 2) D_800A0C78 = 0;
            if (D_800A0C78 == 0) func_8006E108();
            else func_8006E0F4();
        }
        func_8001DAF0();
        break;
    case 3:
        b = JS_;
        if (b & 0x1000) {
            func_8001E2D4(8);
            TC_->w4e--;
        } else if (b & 0x40) {
            func_8001E910(10, 10);
            return 1;
        } else if (b & 0x20) {
            func_8001E2D4(8);
            TC_->w4e = 0;
            TC_->w4c++;
        }
        func_8001DAF0();
        break;
    }
    optionsMenuDraw(TC_->w4e);
    return 0;
}
#endif
