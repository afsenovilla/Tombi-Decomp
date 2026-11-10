// VERSION BETA_980203 beta980203 80027F54 92
#if defined(VERSION_BETA_980203)
// Beta version: PAL joypadInit.c clearing only PAD_ACT_BUFFER[0..3].
extern short PAD_VIBRATION_ENABLED;
extern char PAD_ACT_BUFFER[8];
extern char DAT_800a;
extern int PAD_BUFFERS[];
extern void PadInitDirect(int *, int *);
extern void PadStartCom(void);
void joypadInit(void)
{
PAD_VIBRATION_ENABLED = 1;
PAD_ACT_BUFFER[0] = 0;
PAD_ACT_BUFFER[1] = 0;
PAD_ACT_BUFFER[2] = 0;
PAD_ACT_BUFFER[3] = 0;
PadInitDirect(PAD_BUFFERS, (int *)((char *)PAD_BUFFERS + 0x22));
PadStartCom();
}
#endif
