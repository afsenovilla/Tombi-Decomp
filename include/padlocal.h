#ifndef PADLOCAL_H
#define PADLOCAL_H

/*
 * LIBPAD internal state of the PAL MAIN0.EXE (libpad objects PADMAIN/PADCMD/PADIF/PADPORTD/PADSEQD).
 * Adapted from psx_tomba (US) src/scus_942.36/psyq/libpad/padlocal.h (field names there come from
 * khasinski/parasite-eve-decomp). PAL data lives at US 0x800975xx + 0xC9C and US 0x8009Bxxx/0x8009FDxx + 0xC98.
 * Differences of the PAL padPort: payload_2c_len/command/saved_command sit one byte later (0x36/0x37/0x38).
 * Matched with `// AS gnu` (cc1 2.7.2 -O2 -G0 + GNU as in reorder mode), see tools/ncheck.py.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned long u_long;

/* Actuator capability record (PadInfoAct). */
typedef struct {
    u8 protocol[2];
    u8 payloadBytes;
    u8 activationCost;
    u8 high_bit;
} PadCapabilityRecord;

/* Actuator combination record (PadInfoComb). */
typedef struct {
    u8 length;
    u8 reserved[3];
    u8* data;
} PadCombinationRecord;

/* Per-port command object. */
typedef struct padPort {
    u16* modeTable;                       /* 0x00 */
    PadCapabilityRecord* capabilities;    /* 0x04 */
    PadCombinationRecord* combinations;   /* 0x08 */
    struct padPort* subPorts;             /* 0x0C: multitap sub-ports */
    struct padPort* ownerPort;            /* 0x10 */
    void (*fn_14)(void*);                 /* 0x14 */
    int (*fn_18)(struct padPort*);        /* 0x18 */
    u8 pad_1C[0x04];
    u8* requestedActuatorMap;             /* 0x20 */
    u8 pad_24[0x04];
    u8* payload_28;                       /* 0x28 */
    u8* payload_2c;                       /* 0x2C */
    u8* output_30;                        /* 0x30 */
    u8 payload_28_len;                    /* 0x34 */
    u8 pad_35;                            /* 0x35 (PAL: extra byte) */
    u8 payload_2c_len;                    /* 0x36 */
    u8 command;                           /* 0x37 */
    u8 saved_command;                     /* 0x38 */
    u8 pad_39[0x03];
    u8* response_3c;                      /* 0x3C */
    u8* statusBuffer;                     /* 0x40 */
    u8 response_index;                    /* 0x44 */
    u8 payload_index;                     /* 0x45 */
    u8 sequenceStep;                      /* 0x46: 0xFE/0xFF terminal */
    u8 infoRecordIndex;                   /* 0x47 */
    u8 combinationBytesRemaining;         /* 0x48 */
    u8 communicationState;                /* 0x49: PadGetState */
    u8 retryCount;                        /* 0x4A */
    u8 pad_4B[0x01];
    int frameCounter;                     /* 0x4C */
    u8 pad_50[0x01];
    u8 requestedModeIndex;                /* 0x51 */
    u8 requestedModeLock;                 /* 0x52 */
    u8 modeAlreadySelected;               /* 0x53 */
    u8 pad_54[0x03];
    u8 actuatorEnabled[0x06];             /* 0x57 */
    u8 actuatorMap[0x06];                 /* 0x5D: 0xFF = unused */
    u8 pad_63[0x80];
    u8 modeCount;                         /* 0xE3 */
    u8 currentModeIndex;                  /* 0xE4 */
    u8 pad_e5[0x01];
    u16 currentExtendedId;                /* 0xE6 */
    u8 currentId;                         /* 0xE8: 8 = multitap */
    u8 actuatorCount;                     /* 0xE9 */
    u8 combinationCount;                  /* 0xEA */
    u8 pad_eb[0x01];
    u16 combinationStorageBytes;          /* 0xEC (PAL: u16) */
    u16 lastExtendedId;                   /* 0xEE (PAL only) */
} padPort;



#define OFFSETOF(type, member) ((u32) & (((type*)0)->member))

/* PadInitDirect walks the objects by their statusBuffer member. */
#define PADPORT_FROM_STATUS_BUFFER(p) \
    ((padPort*)((u8*)(p) - OFFSETOF(padPort, statusBuffer)))

/* Address arithmetic in the original's operand order. */
typedef union {
    padPort* obj;
    PadCapabilityRecord* capability;
    int* count;
    u8* bytes;
    u32 word;
} padAddr;

/* I_STAT / I_MASK at 0x1F801070. */
typedef struct {
    volatile u_long stat;
    volatile u_long mask;
} padIntr;

/* SIO0 at 0x1F801040. */
typedef struct {
    volatile u_char data;
    u_char unk1[3];
    volatile u_short stat;
    u_short unk6;
    volatile u_short mode;
    volatile u_short ctrl;
    u_short unkC;
    volatile u_short baud;
} padSio;

/* BIOS / LIBAPI. */
int EnterCriticalSection(void);
void ExitCriticalSection(void);
int ChangeClearRCnt(int, int);
int SysDeqIntRP(int, void*);
int SysEnqIntRP(int, void*);
void bzero(void*, int);

/* PAL function names (address-named, see notes/names_main0.csv). */
int _padVsyncHandler(void);                        /* vsync handler (US func_80069058) */
void _padStartCom(void);                        /* _padStartCom */
int _padInitSioMode(padPort*);                    /* _padInitSioMode */
void padCommandStep(padPort*);                    /* one command step */
int _padSioRW(padPort*, int);               /* _padSioRW */
int _padSioExchangeByte(padPort*, int);               /* byte exchange with ack wait */
int _padClrIntSio0(void);                        /* _padClrIntSio0 */
void sioWaitRxReady(void);                       /* wait SIO tx ready */
void padQueueActInfoCmd(padPort*);                   /* per-step handlers stored in fn_14 / fn_18 */
int func_8006A718(padPort*);
void padQueueMainModeCmd(padPort*);
int _padChkMainModeDone(padPort*);
void _padSetVsyncParam(void);                        /* _padSetVsyncParam */
void _padInitDirSeq(void);                        /* _padInitDirSeq */
int func_8006B62C(int);                         /* completion (D_800981AC) */
void padResetPortState(padPort*);                   /* reset object (D_800981B0) */
int padGetSendByte(padPort*, int);               /* payload byte (D_800981B4) */
void func_8006B7EC(padPort*);                   /* actuator map (D_800981B8) */
int padGetPortId(padPort*);                    /* channel of object (D_800981BC) */
padPort* padGetPortBuffer(int);                    /* object of channel (D_800981C0) */
void _padLatchId(padPort*);                    /* swap command (D_800981D0) */
void setRC2wait(int);                         /* setRC2wait */
int chkRC2wait(void);                         /* chkRC2wait */

/* State. */
extern int (*D_800981AC)(int);                  /* DMA/transfer completion */
extern void (*D_800981B0)(padPort*);            /* reset object */
extern int (*D_800981B4)(padPort*, int);        /* read payload byte */
extern void (*D_800981B8)(padPort*);            /* actuator map */
extern int (*D_800981BC)(padPort*);             /* channel id of object */
extern padPort* (*D_800981C0)(int);             /* object of channel */
extern int (*D_800981C4)(padPort*);             /* state dispatch */
extern int (*D_800981C8)();                     /* transfer busy */
extern void (*D_800981CC)(padPort*);            /* response handler */
extern void (*D_800981D0)(padPort*);            /* swap byte field */
extern int (*D_800981D4)(void);
extern void (*D_800981D8)(void);
extern void (*D_800981DC)(void);
extern padPort* D_800981E0;                     /* port objects */
extern int D_800981E4;                          /* service ready */
extern int D_800981E8;
extern int D_800981EC;                          /* active port index */
extern int D_800981F0;                          /* command step */
extern int D_800981F4;                          /* actuator cost total */
extern int D_800981F8;                          /* multitap command byte */
extern int D_800981FC;                          /* port 1 absent */
extern int D_80098200;                          /* last port index */
extern int D_80098204[];                        /* channel counts */
extern padIntr* D_8009820C;
extern padSio* D_80098210;
extern int D_80098214;                          /* vsync callback pending */
extern int D_80098224;
extern volatile u8* D_80098228;
extern int D_8009822C;
extern int (*D_80098230[])(void);               /* command step handlers */
extern padSio* D_80098244;
extern int D_8009BF78[];                        /* VSync interrupt queue node */
extern int D_8009BF88[2];                       /* idle frames per port */
extern u8* D_8009BF98;
extern u8 D_8009BFA8[];
extern u8 D_8009BFF0[];
extern padPort D_8009C038[];
extern u32 D_8009C33C;                          /* timeout start */
extern u32 D_800A0A18;                          /* timeout limit */

#endif
