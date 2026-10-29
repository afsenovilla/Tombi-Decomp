// FUNC 80126d48 52 X000
// MATCHING 80126d48 52
#if defined(VERSION_PCPX_96084) /* PrePre 8 scratchpad layout */
#define SPAD_19E 0x1f8002a6
#else
#define SPAD_19E 0x1f80019e
#endif
extern short pushObjectOutOfObjectBoxB();

void pushCat2ObjectOutOfCat4Type0A(void)
{
    if (pushObjectOutOfObjectBoxB() != 0)
        *(short *)SPAD_19E = 0;
}
