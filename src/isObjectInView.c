// FUNC 800203dc 108 MAIN0
// MATCHING 800203dc 108
#if defined(VERSION_PCPX_96084) /* PrePre 8 scratchpad layout */
#define SCR_X 0x1F80028A
#define SCR_Y 0x1F800392
#else
#define SCR_X 0x1F800176
#define SCR_Y 0x1F800186
#endif
typedef struct P { short x; unsigned short y; } P;
typedef struct S { unsigned char on; char p[0x15]; unsigned short z; char q[0x28]; P *pos; } S;
static __inline__ int vis(void *q)
{
    S *p = q;
    if (p->on == 0) return 0;
    if ((unsigned short)(p->pos->y - *(unsigned short *)SCR_X + 0x40) >= 0x1c1) return 0;
    return (unsigned short)(*(unsigned short *)SCR_Y - p->z + 0x40) < 0x171;
}
int isObjectInView(S *o)
{
    return vis(o);
}
