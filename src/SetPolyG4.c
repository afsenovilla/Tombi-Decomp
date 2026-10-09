// FUNC 8005e6fc 20 MAIN0
// MATCHING 8005e6fc 20
// Portado de psx_tomba (psyq/libgpu/prim.c, SetPolyG4); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libgpu.h"

#define CMD_CLEAR_CACHE 0x01000000
#define CMD_COPY_CPU_TO_VRAM 0xA0000000

u_short GetTPage(int tp, int abr, int x, int y);

u_short GetClut(int x, int y);

/**
 * @brief Display texture page information for debugging
 * 
 * Prints detailed texture page information to the debug output.
 * Shows texture format, position, and color mode based on the
 * current graphics type.
 * 
 * @param tpage Texture page identifier as returned by GetTPage() or LoadTPage()
 */
void DumpTPage(u_short tpage);

/**
 * @brief Display CLUT (palette) information for debugging
 * 
 * Prints the CLUT coordinates to the debug output. Used for debugging
 * texture and palette issues.
 * 
 * @param clut CLUT identifier as returned by GetClut() or LoadClut()
 */
void DumpClut(u_short clut);

/**
 * @brief Get pointer to next primitive in a linked list
 * 
 * Returns a pointer to the next primitive in a chain of linked primitives.
 * Used for traversing primitive lists.
 * 
 * @param p Pointer to current primitive
 * @return Pointer to next primitive, or NULL if end of list
 */
void* NextPrim(void *p);

/**
 * @brief Check if primitive is the end of a list
 * 
 * Determines if the given primitive is the last one in a linked list.
 * 
 * @param p Pointer to primitive to check
 * @return Non-zero if this is the end primitive, zero otherwise
 */
int IsEndPrim(void *p);

/**
 * @brief Add primitive to ordering table
 * 
 * Links a graphics primitive into the specified ordering table entry.
 * Primitives are rendered in order from OT entry 0 to highest entry.
 * 
 * @param ot Pointer to ordering table entry
 * @param p Pointer to primitive to add
 */
void AddPrim(void *ot, void *p);

/**
 * @brief Add multiple primitives to ordering table
 * 
 * Adds multiple primitives to the ordering table at once,
 * treating p0 through p1 as a range of primitives to add.
 * 
 * @param ot Pointer to ordering table
 * @param p0 Pointer to first primitive in range
 * @param p1 Pointer to last primitive in range
 */
void AddPrims(void *ot, void *p0, void *p1);

/**
 * @brief Connect two primitives together
 * 
 * Links two primitives by setting the address of p0 to point to p1,
 * creating a chain of primitives for the GPU to process.
 * 
 * @param p0 Pointer to first primitive
 * @param p1 Pointer to second primitive to link to
 */
void CatPrim(void *p0, void *p1);

/**
 * @brief Terminate a primitive chain
 * 
 * Marks a primitive as the end of a primitive chain by setting
 * its address to NULL, stopping GPU command traversal.
 * 
 * @param p Pointer to primitive to terminate
 */
void TermPrim(void *p);

/**
 * @brief Set semi-transparency mode for a primitive
 * 
 * Enables or disables semi-transparency blending for a primitive.
 * When enabled, the primitive will be blended with the framebuffer
 * using the current blending equation.
 * 
 * @param p Pointer to primitive
 * @param abe Semi-transparency flag (0=disabled, 1=enabled)
 */
void SetSemiTrans(void *p, int abe);

/**
 * @brief Set texture gouraud shading mode for a primitive
 * 
 * Enables or disables texture gouraud shading, which applies
 * vertex colors to textured primitives for lighting effects.
 * 
 * @param p Pointer to primitive
 * @param tge Texture gouraud flag (0=disabled, 1=enabled)
 */
void SetShadeTex(void *p, int tge);

/**
 * @brief Initialize a flat-shaded 3-vertex polygon primitive
 * 
 * Sets up a POLY_F3 primitive with proper packet length and
 * GPU command code for rendering a 3-vertex polygon with
 * flat (single) color shading.
 * 
 * @param p Pointer to POLY_F3 primitive structure
 */
void SetPolyF3(POLY_F3 *p);

/**
 * @brief Initialize a flat-shaded textured 3-vertex polygon primitive
 * 
 * Sets up a POLY_FT3 primitive with proper packet length and
 * GPU command code for rendering a 3-vertex polygon with
 * flat color shading and texture mapping.
 * 
 * @param p Pointer to POLY_FT3 primitive structure
 */
void SetPolyFT3(POLY_FT3 *p);

/**
 * @brief Initialize a gouraud-shaded 3-vertex polygon primitive
 * 
 * Sets up a POLY_G3 primitive with proper packet length and
 * GPU command code for rendering a 3-vertex polygon with
 * gouraud (interpolated) color shading.
 * 
 * @param p Pointer to POLY_G3 primitive structure
 */
void SetPolyG3(POLY_G3 *p);

/**
 * @brief Initialize a gouraud-shaded textured 3-vertex polygon primitive
 * 
 * Sets up a POLY_GT3 primitive with proper packet length and
 * GPU command code for rendering a 3-vertex polygon with
 * gouraud color shading and texture mapping.
 * 
 * @param p Pointer to POLY_GT3 primitive structure
 */
void SetPolyGT3(POLY_GT3 *p);

/**
 * @brief Initialize a flat-shaded 4-vertex polygon primitive
 * 
 * Sets up a POLY_F4 primitive (quad) with proper packet length and
 * GPU command code for rendering a 4-vertex polygon with
 * flat (single) color shading.
 * 
 * @param p Pointer to POLY_F4 primitive structure
 */
void SetPolyF4(POLY_F4 *p);

/**
 * @brief Initialize a flat-shaded textured 4-vertex polygon primitive
 * 
 * Sets up a POLY_FT4 primitive (textured quad) with proper packet length and
 * GPU command code for rendering a 4-vertex polygon with
 * flat color shading and texture mapping.
 * 
 * @param p Pointer to POLY_FT4 primitive structure
 */
void SetPolyFT4(POLY_FT4 *p);

/**
 * @brief Initialize a gouraud-shaded 4-vertex polygon primitive
 * 
 * Sets up a POLY_G4 primitive (gouraud quad) with proper packet length and
 * GPU command code for rendering a 4-vertex polygon with
 * gouraud (interpolated) color shading.
 * 
 * @param p Pointer to POLY_G4 primitive structure
 */
void SetPolyG4(POLY_G4 *p) {
    setlen(p, 8);
    setcode(p, 0x38);
}

/**
 * @brief Initialize a gouraud-shaded textured 4-vertex polygon primitive
 * 
 * Sets up a POLY_GT4 primitive (textured gouraud quad) with proper packet length and
 * GPU command code for rendering a 4-vertex polygon with
 * gouraud color shading and texture mapping.
 * 
 * @param p Pointer to POLY_GT4 primitive structure
 */
void SetPolyGT4(POLY_GT4 *p);

/**
 * @brief Initialize an 8x8 pixel sprite primitive
 * 
 * Sets up a SPRT_8 primitive with proper packet length and
 * GPU command code for rendering an 8x8 pixel textured sprite.
 * 
 * @param p Pointer to SPRT_8 primitive structure
 */
void SetSprt8(SPRT_8 *p);

/**
 * @brief Initialize a 16x16 pixel sprite primitive
 * 
 * Sets up a SPRT_16 primitive with proper packet length and
 * GPU command code for rendering a 16x16 pixel textured sprite.
 * 
 * @param p Pointer to SPRT_16 primitive structure
 */
void SetSprt16(SPRT_16 *p);

/**
 * @brief Initialize a variable-size sprite primitive
 * 
 * Sets up a SPRT primitive with proper packet length and
 * GPU command code for rendering a textured sprite with
 * arbitrary width and height.
 * 
 * @param p Pointer to SPRT primitive structure
 */
void SetSprt(SPRT *p);

/**
 * @brief Initialize a 1x1 pixel tile primitive
 * 
 * Sets up a TILE_1 primitive with proper packet length and
 * GPU command code for rendering a single pixel with solid color.
 * 
 * @param p Pointer to TILE_1 primitive structure
 */
void SetTile1(TILE_1 *p);

/**
 * @brief Initialize an 8x8 pixel tile primitive
 * 
 * Sets up a TILE_8 primitive with proper packet length and
 * GPU command code for rendering an 8x8 pixel solid color rectangle.
 * 
 * @param p Pointer to TILE_8 primitive structure
 */
void SetTile8(TILE_8 *p);

/**
 * @brief Initialize a 16x16 pixel tile primitive
 * 
 * Sets up a TILE_16 primitive with proper packet length and
 * GPU command code for rendering a 16x16 pixel solid color rectangle.
 * 
 * @param p Pointer to TILE_16 primitive structure
 */
void SetTile16(TILE_16 *p);

/**
 * @brief Initialize a variable-size tile primitive
 * 
 * Sets up a TILE primitive with proper packet length and
 * GPU command code for rendering a solid color rectangle with
 * arbitrary width and height.
 * 
 * @param p Pointer to TILE primitive structure
 */
void SetTile(TILE *p);

/**
 * @brief Initialize a flat-shaded 2-point line primitive
 * 
 * Sets up a LINE_F2 primitive with proper packet length and
 * GPU command code for rendering a line between two points
 * with flat (single) color.
 * 
 * @param p Pointer to LINE_F2 primitive structure
 */
void SetLineF2(LINE_F2 *p);

/**
 * @brief Initialize a gouraud-shaded 2-point line primitive
 * 
 * Sets up a LINE_G2 primitive with proper packet length and
 * GPU command code for rendering a line between two points
 * with gouraud (interpolated) color blending.
 * 
 * @param p Pointer to LINE_G2 primitive structure
 */
void SetLineG2(LINE_G2 *p);

/**
 * @brief Initialize a flat-shaded 3-point polyline primitive
 * 
 * Sets up a LINE_F3 primitive with proper packet length and
 * GPU command code for rendering a polyline through three points
 * with flat (single) color. The pad field is set to disable
 * automatic line termination.
 * 
 * @param p Pointer to LINE_F3 primitive structure
 */
void SetLineF3(LINE_F3 *p);

/**
 * @brief Initialize a gouraud-shaded 3-point polyline primitive
 * 
 * Sets up a LINE_G3 primitive with proper packet length and
 * GPU command code for rendering a polyline through three points
 * with gouraud (interpolated) color blending. The pad field is set
 * to disable automatic line termination.
 * 
 * @param p Pointer to LINE_G3 primitive structure
 */
void SetLineG3(LINE_G3 *p);

/**
 * @brief Initialize a flat-shaded 4-point polyline primitive
 * 
 * Sets up a LINE_F4 primitive with proper packet length and
 * GPU command code for rendering a polyline through four points
 * with flat (single) color. The pad field is set to disable
 * automatic line termination.
 * 
 * @param p Pointer to LINE_F4 primitive structure
 */
void SetLineF4(LINE_F4 *p);

/**
 * @brief Initialize a gouraud-shaded 4-point polyline primitive
 * 
 * Sets up a LINE_G4 primitive with proper packet length and
 * GPU command code for rendering a polyline through four points
 * with gouraud (interpolated) color blending. The pad field is set
 * to disable automatic line termination.
 * 
 * @param p Pointer to LINE_G4 primitive structure
 */
void SetLineG4(LINE_G4 *p);

/**
 * @brief Initialize a texture page drawing command
 * 
 * Sets up a DR_TPAGE primitive that configures texture page settings
 * for subsequent rendering operations. This command is added to the
 * ordering table to change GPU texture parameters.
 * 
 * @param p Pointer to DR_TPAGE primitive structure
 * @param dfe Dither flag enable (0=disabled, 1=enabled)
 * @param dtd Draw to display area flag (0=disabled, 1=enabled)  
 * @param tpage Texture page ID containing format and position info
 */
void SetDrawTPage(DR_TPAGE* p, s32 dfe, s32 dtd, s32 tpage);

/**
 * @brief Initialize a VRAM move command
 * 
 * Sets up a DR_MOVE primitive that copies a rectangular area from
 * one location in VRAM to another. The command includes cache
 * clear operations to ensure data integrity.
 * 
 * @param p Pointer to DR_MOVE primitive structure
 * @param rect Source rectangle in VRAM to copy from
 * @param x Destination X coordinate in VRAM
 * @param y Destination Y coordinate in VRAM
 */
void SetDrawMove(DR_MOVE* p, RECT* rect, int x, int y);

/**
 * @brief Initialize a CPU-to-VRAM data transfer command
 * 
 * Sets up a DR_LOAD primitive that prepares for transferring pixel data
 * from CPU memory to VRAM. The command calculates the required packet
 * size based on pixel count and includes cache operations.
 * 
 * @param p Pointer to DR_LOAD primitive structure
 * @param rect Target rectangle in VRAM where data will be loaded
 */
void SetDrawLoad(DR_LOAD* p, RECT* rect);

/**
 * @brief Merge two primitives into one
 * 
 * Combines two graphics primitives into a single primitive packet.
 * This can be used to optimize rendering by reducing the number
 * of separate GPU commands.
 * 
 * @param p0 Pointer to first primitive (becomes the merged primitive)
 * @param p1 Pointer to second primitive (will be cleared)
 * @return 0 on success, -1 if merged primitive would be too large
 */
int MargePrim(void* p0, void* p1);

/**
 * @brief Display drawing environment information for debugging
 * 
 * Prints detailed information about a drawing environment structure
 * to the debug output. Shows clipping area, offset, texture window,
 * dithering settings, and texture page information.
 * 
 * @param env Pointer to DRAWENV structure to display
 */
void DumpDrawEnv(DRAWENV* env);

/**
 * @brief Display video display environment information for debugging
 * 
 * Prints detailed information about a display environment structure
 * to the debug output. Shows display area, screen area, interlace
 * mode, and RGB24 mode settings.
 * 
 * @param env Pointer to DISPENV structure to display
 */
void DumpDispEnv(DISPENV* env);
