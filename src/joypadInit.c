// FUNC 80025bf4 108 MAIN0
// MATCHING 80025bf4 108
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
PAD_ACT_BUFFER[6] = 0;
PAD_ACT_BUFFER[7] = 0;
PadInitDirect(PAD_BUFFERS, (int *)((char *)PAD_BUFFERS + 0x22));
PadStartCom();
}
