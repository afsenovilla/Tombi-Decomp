// FUNC 8012788c 52 X001
// MATCHING 8012788c 52
#if defined(VERSION_PCPX_96084) /* PrePre 8 scratchpad layout */
#define SPAD_19E 0x1f8002a6
#else
#define SPAD_19E 0x1f80019e
#endif
extern short pushCat2ObjectOutOfType04();

void pushCat2ObjectOutOfCat4Type0A(void)
{
    if (pushCat2ObjectOutOfType04() != 0)
        *(short *)SPAD_19E = 0;
}
