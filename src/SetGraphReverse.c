// FUNC 8005edc0 276 MAIN0
// MATCHING 8005edc0 276
// Portado de psx_tomba (psyq/libgpu/sys.c, SetGraphReverse); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libgpu.h"

#define OT_TYPE u_long
#define CMD_CLEAR_CACHE 0x01000000
#define CMD_COPY_VRAM_TO_CPU 0xC0000000
#define CMD_COPY_CPU_TO_VRAM 0xA0000000
#define CMD_FILL_RECTANGLE_IN_VRAM(color) ((color & 0xFFFFFF) | 0x02000000)
#define CMD_MONOCHROME_RECTANGLE(color) ((color & 0xFFFFFF) | 0x60000000)
#define CLAMP(a, b, c) (a >= b ? (a > c ? c : a) : b)
#define CMPRECT(r1, r2)  (\
    ((volatile RECT*)r1)->x == r2.x && \
    ((volatile RECT*)r1)->y == r2.y && \
    ((volatile RECT*)r1)->w == r2.w && \
    ((volatile RECT*)r1)->h == r2.h\
)
#define TERM_PRIM(ot, p) *ot = (u_long)p & 0xFFFFFF

typedef struct {
    /* 0x00 */ const char* ver; // D_80015BA8
    /* 0x04 */ void (*addque)();
    /* 0x08 */ int (*addque2)();
    /* 0x0C */ int (*clr)();
    /* 0x10 */ void (*ctl)(unsigned int);
    /* 0x14 */ int (*cwb)(u32* data, s32 n);
    /* 0x18 */ void (*cwc)();
    /* 0x1C */ int (*drs)();
    /* 0x20 */ int (*dws)();
    /* 0x24 */ int (*exeque)();
    /* 0x28 */ int (*getctl)(int);
    /* 0x2C */ void (*otc)(OT_TYPE* ot, s32 n);
    /* 0x30 */ int (*param)(int);
    /* 0x34 */ int (*reset)(int);
    /* 0x38 */ u_long (*status)(void);
    /* 0x3C */ int (*sync)(int mode);
} GPU;

typedef struct {
    // GPU version
    // https://psx-spx.consoledev.net/graphicsprocessingunitgpu/#gpu-versions
    u_char version;
    u_char D_80090C9D;
    u_char level;
    u_char reverse;
    short w;
    short h;
    u8 unk8[4];
    void (*drawSyncCb)();
    DRAWENV draw;
    DISPENV disp;
} DEBUG;                               // size = 0x40

typedef struct {
    u_long tag;
    u_long code[2];
} DR_PRIO;

typedef struct QueueItem {
    int unk0;
    int unk4;
    int unk8;
    int unkC[2];
    int unk14;
    int unk18;
    char padding[0x14 + 0x1C + 0x14];
} QueueItem;

extern s32 D_80090C54;
extern GPU* D_80091930;
extern DEBUG GPU_INFO;
extern s32 D_80090D1C[];
extern s32 D_80090D30[];
extern u32 D_80090D4C[];
extern volatile s32 D_80090D58;
extern volatile s32* GPU_DATA;
extern volatile s32* GPU_STATUS;
extern volatile s32* DMA1_MADR;
extern volatile s32* DMA1_BCR;
extern volatile s32* DMA1_CHCR;
extern volatile s32* DMA2_CHCR;
extern volatile s32* DMA2_MADR;
extern volatile s32* DMA2_BCR;
extern volatile s32* DPCR;
extern volatile s32 D_80090D90;
extern volatile s32 D_80090D94;
extern volatile s32 D_80090D98;
extern volatile s32 GPU_QIN;
extern volatile s32 GPU_QOUT;
extern s32 D_80090DA8;
extern s32 D_80090DAC;
extern s32 D_80090DB0;
extern s32 D_80090DB4;
extern s32 D_80090DB8;
extern s32 D_8009B148;
extern s32 D_8009B14C;
extern s32 D_8009B150;
extern s32 D_8009B154;
extern s32 D_8009B158;
extern s32 D_8009B15C;
extern s32 D_8009B160;
extern s32 D_8009B164;
extern s32 D_8009B168;
extern s32 D_8009B16C;
extern s32 D_8009B170;
extern s32 D_8009B174;
extern s32 D_8009B178;
extern u8 GPU_CTLBUF[];
extern volatile QueueItem GPU_QITEM[];

const char D_80015BA8[] = "$Id: sys.c,v 1.129 1996/12/25 03:36:20 noda Exp $";

/**
 * @brief Initialize or reset the graphics system
 * 
 * Resets the GPU hardware and initializes the graphics library.
 * This function must be called before using any other graphics functions.
 * Different modes control the level of reset performed.
 * 
 * @param mode Reset mode:
 *             0 = Complete reset with debug output
 *             3 = Complete reset with debug output  
 *             5 = Complete reset without debug output
 *             Other = Partial reset
 * @return GPU version number (1 for GPU v1, 2 for GPU v2)
 */
int ResetGraph(int mode);

int SetGraphReverse(int mode) {
    u_char prev = GPU_INFO.reverse;
    if (GPU_INFO.level >= 2) {
        GPU_printf("SetGraphReverse(%d)...\n", mode);
    }
    GPU_INFO.reverse = mode;
    D_80091930->ctl(D_80091930->getctl(8) | (GPU_INFO.reverse ? 0x08000080 : 0x08000000));
    if (GPU_INFO.version == 2) {
        D_80091930->ctl(GPU_INFO.reverse ? 0x20000501 : 0x20000504);
    }
    return prev;
}

/**
 * @brief Set graphics debugging level
 * 
 * Controls the amount of debugging information output by graphics functions.
 * Higher levels provide more detailed information about GPU operations.
 * 
 * @param level Debug level:
 *              0 = No debug output
 *              1 = Basic error checking
 *              2 = Detailed function tracing
 * @return Previous debug level
 */
int SetGraphDebug(int level);

int SetGraphQueue(int mode);

/**
 * @brief Get GPU hardware version
 * 
 * Returns the version of the GPU hardware detected during initialization.
 * Different GPU versions have slightly different capabilities and timing.
 * 
 * @return GPU version (1 for original GPU, 2 for revised GPU)
 */
u8 GetGraphType(void);

/**
 * @brief Get current graphics debugging level
 * 
 * Returns the current debugging level set by SetGraphDebug().
 * 
 * @return Current debug level (0-2)
 */
s32 GetGraphDebug(void);

/**
 * @brief Set callback function for draw synchronization
 * 
 * Registers a callback function that will be called when GPU drawing
 * operations complete. Used for frame synchronization and timing.
 * 
 * @param func Pointer to callback function, or NULL to disable
 * @return Pointer to previous callback function
 */
u_long DrawSyncCallback(void (*func)());

/**
 * @brief Enable or disable display output
 * 
 * Controls whether the GPU outputs video to the display. When disabled,
 * the screen will be black but rendering can still continue to VRAM.
 * 
 * @param mask Non-zero to enable display, zero to disable
 */
void SetDispMask(int mask);

/**
 * @brief Wait for GPU drawing operations to complete
 * 
 * Synchronizes CPU execution with GPU rendering. Different modes
 * provide different levels of synchronization.
 * 
 * @param mode Sync mode:
 *             0 = Wait for all operations to complete
 *             Other values = Implementation specific
 */
int DrawSync(int mode);

void checkRECT(const char* log, RECT* r);

/**
 * @brief Clear rectangular area of VRAM with solid color
 * 
 * Fills the specified rectangular area in VRAM with a solid color.
 * This is commonly used to clear the background before rendering.
 * 
 * @param rect Pointer to RECT structure defining area to clear
 * @param r Red component (0-255)
 * @param g Green component (0-255)  
 * @param b Blue component (0-255)
 * @return Operation result from GPU queue
 */
int ClearImage(RECT* rect, u8 r, u8 g, u8 b);

/**
 * @brief Clear rectangular area of VRAM with solid color (alternate mode)
 * 
 * Similar to ClearImage but with different GPU command flags.
 * The exact difference depends on GPU implementation details.
 * 
 * @param rect Pointer to RECT structure defining area to clear
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @return Operation result from GPU queue
 */
int ClearImage2(RECT* rect, u8 r, u8 g, u8 b);

/**
 * @brief Transfer image data from main memory to VRAM
 * 
 * Copies pixel data from main memory to the specified rectangular
 * area in VRAM. This is the primary method for loading textures
 * and other image data.
 * 
 * @param rect Pointer to RECT structure defining VRAM destination
 * @param p Pointer to source pixel data in main memory
 * @return Operation result from GPU queue
 */
int LoadImage(RECT* rect, u_long* p);

/**
 * @brief Transfer image data from VRAM to main memory
 * 
 * Copies pixel data from the specified rectangular area in VRAM
 * to main memory. Used for reading back rendered images or
 * saving VRAM contents.
 * 
 * @param rect Pointer to RECT structure defining VRAM source area
 * @param p Pointer to destination buffer in main memory
 * @return Operation result from GPU queue
 */
int StoreImage(RECT* rect, u_long* p);

/**
 * @brief Copy rectangular area within VRAM
 * 
 * Copies pixel data from one rectangular area in VRAM to another.
 * Both source and destination are within VRAM. This is faster than
 * transferring through main memory.
 * 
 * @param rect Pointer to RECT structure defining source area
 * @param x X coordinate of destination
 * @param y Y coordinate of destination
 * @return 0 on success, -1 if rect has zero width/height
 */
int MoveImage(RECT* rect, s32 x, s32 y);

/**
 * @brief Initialize ordering table for primitive sorting
 * 
 * Initializes an ordering table by linking all entries and setting
 * up termination. Ordering tables are used to sort graphics primitives
 * by depth for proper rendering order.
 * 
 * @param ot Pointer to ordering table array
 * @param n Number of entries in the ordering table
 * @return Pointer to last entry in the ordering table
 */
OT_TYPE* ClearOTag(OT_TYPE* ot, int n);

/**
 * @brief Initialize ordering table in reverse order
 * 
 * Similar to ClearOTag but initializes the ordering table in reverse
 * order. This can be useful for certain rendering techniques.
 * 
 * @param ot Pointer to ordering table array
 * @param n Number of entries in the ordering table
 * @return Pointer to first entry in the ordering table
 */
OT_TYPE* ClearOTagR(OT_TYPE* ot, int n);

/**
 * @brief Draw a single primitive immediately
 * 
 * Renders a single graphics primitive directly to the GPU without
 * using the ordering table system. Used for immediate rendering
 * or special effects.
 * 
 * @param p Pointer to primitive structure to render
 */
void DrawPrim(void* p);

/**
 * @brief Render all primitives in an ordering table
 * 
 * Processes and renders all graphics primitives stored in the ordering
 * table. This is the main function for rendering a complete frame.
 * Primitives are rendered in order from entry 0 to the highest entry.
 * 
 * @param p Pointer to the first entry of the ordering table
 */
void DrawOTag(u_long* p);

DRAWENV* PutDrawEnv(DRAWENV* env);

/**
 * @brief Render ordering table with specific drawing environment
 * 
 * Processes and renders all graphics primitives in the ordering table
 * with a specific drawing environment applied. The drawing environment
 * is linked to the ordering table and both are sent to the GPU.
 * 
 * @param p Pointer to the first entry of the ordering table
 * @param env Pointer to drawing environment to apply during rendering
 */
void DrawOTagEnv(u_long* p, DRAWENV* env);

DRAWENV* GetDrawEnv(DRAWENV* env);

DISPENV* PutDispEnv(DISPENV* env);

/**
 * @brief Get current display environment settings
 * 
 * Retrieves the current display environment configuration from
 * the GPU system. Used to query current video display settings.
 * 
 * @param env Pointer to DISPENV structure to fill with current settings
 * @return Pointer to the filled environment structure
 */
DISPENV* GetDispEnv(DISPENV* env);

/**
 * @brief Get Odd/Even display field status
 * 
 * Returns the current field being displayed in interlaced video modes.
 * Used for field-based rendering and synchronization.
 * 
 * @return 0 for even field, 1 for odd field
 */
int GetODE(void);

/**
 * @brief Set texture window drawing command
 * 
 * Sets up a DR_TWIN primitive that defines the texture window
 * for texture coordinate wrapping. The texture window controls
 * how texture coordinates wrap when they exceed texture boundaries.
 * 
 * @param p Pointer to DR_TWIN primitive structure
 * @param tw Pointer to RECT defining texture window area
 */
void SetTexWindow(DR_TWIN* p, RECT* tw);

/**
 * @brief Set drawing area drawing command
 * 
 * Sets up a DR_AREA primitive that defines the drawing area bounds.
 * All rendering operations will be clipped to this rectangular area.
 * 
 * @param p Pointer to DR_AREA primitive structure
 * @param r Pointer to RECT defining the drawing area
 */
void SetDrawArea(DR_AREA* p, RECT* r);

/**
 * @brief Set drawing offset drawing command
 * 
 * Sets up a DR_OFFSET primitive that defines the drawing offset.
 * All subsequent rendering operations will be offset by these values.
 * 
 * @param p Pointer to DR_OFFSET primitive structure
 * @param ofs Pointer to array of two offsets [x_offset, y_offset]
 */
void SetDrawOffset(DR_OFFSET* p, u_short* ofs);

/**
 * @brief Set rendering priority drawing command
 * 
 * Sets up a DR_PRIO primitive that controls primitive rendering priority.
 * This affects the order in which primitives are processed and can
 * influence depth testing behavior.
 * 
 * @param p Pointer to DR_PRIO primitive structure
 * @param pbc Priority comparison flag
 * @param pbw Priority window flag
 */
void SetPriority(DR_PRIO* p, int pbc, int pbw);

/**
 * @brief Set drawing mode drawing command
 * 
 * Sets up a DR_MODE primitive that configures various drawing modes
 * including dithering, texture page settings, and texture window.
 * 
 * @param p Pointer to DR_MODE primitive structure
 * @param dfe Dither flag enable (0=disabled, 1=enabled)
 * @param dtd Draw to display area flag (0=disabled, 1=enabled)
 * @param tpage Texture page ID
 * @param tw Pointer to RECT defining texture window
 */
void SetDrawMode(DR_MODE* p, s32 dfe, s32 dtd, s32 tpage, RECT* tw);

/**
 * @brief Set drawing environment drawing command
 * 
 * Sets up a DR_ENV primitive that contains all drawing environment
 * settings from a DRAWENV structure. This includes clipping area,
 * drawing offset, texture window, and various GPU modes.
 * 
 * @param dr_env Pointer to DR_ENV primitive structure to initialize
 * @param env Pointer to DRAWENV structure containing settings
 */
void SetDrawEnv(DR_ENV* dr_env, DRAWENV* env);

int SetDrawEnv2(DR_ENV* dr_env, DRAWENV* env);

u_long get_mode(int dfe, int dtd, u_short tpage);

u_long get_cs(short x, short y);

u_long get_ce(short x, short y);

u_long get_ofs(short x, short y);

u_long get_tw(RECT* rect);

u_long get_dx(DISPENV* env);

int _status(void);

int _otc(OT_TYPE ot, s32 n);

s32 _clr(RECT* rect, u32 color);

s32 _dws(RECT* arg0, s32* arg1);

s32 _drs(RECT* arg0, s32* arg1);

void _ctl(u32 arg0);

s32 _getctl(s32 arg0);

s32 _cwb(s32* arg0, s32 arg1);

void _cwc(s32 arg0);

s32 _param(s32 arg0);

void _addque(int arg0, int arg1, int arg2);

s32 _addque2(void (*arg0)(s32*, s32), s32* arg1, s32 arg2, s32 arg3);

s32 _exeque(void);

s32 _reset(s32 mode);

s32 _sync(s32 arg0);

void set_alarm(void);

s32 get_alarm(void);

int _version(int mode);

void * GPU_memset(s8* ptr, int value, s32 num);
