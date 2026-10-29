// FUNC 80126d04 68 X000
// MATCHING 80126d04 68
#if defined(VERSION_PCPX_96084) /* PrePre 8 scratchpad layout */
#define SPAD_19E 0x1f8002a6
#else
#define SPAD_19E 0x1f80019e
#endif
extern short pushObjectOutOfObjectBoxB();

void pushCat5ObjectOutOfCat4Type04(int a, char *p)
{
    if (*(int *)(p + 0x94) == 0 && pushObjectOutOfObjectBoxB() != 0)
        *(short *)SPAD_19E = 0;
}
