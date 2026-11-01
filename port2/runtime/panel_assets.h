/* Second screen panel: the game's graphics and fonts from the user's disc (panel_assets.c) */
#ifndef PANEL_ASSETS_H
#define PANEL_ASSETS_H
#define PA_FLIPX  1
#define PA_DIM    2
#define PA_SHADE  4
#define PA_OPAQUE 8
#define PA_OUTLINE 16  /* 1 px dark outline around the opaque texels (game-style marker) */
#define PA_X2 128      /* drawn at 2x (each texel a 2 x 2 block: map markers) */
#define PA_X3 256      /* drawn at 3x */
#define PA_CUTBOX 32   /* drop the texels of CLUT indices pa_cut_mask reachable from the rect border (a boxed icon) */
extern unsigned pa_cut_mask, pa_drop_mask, pa_outline;
extern int pa_ok, pa_font_ok;
extern unsigned *pa_cv; extern int pa_cw, pa_ch;          /* the canvas, in game pixels (0xAABBGGRR) */
extern int pa_clip_x0, pa_clip_y0, pa_clip_x1, pa_clip_y1;
void pa_init(void);
unsigned char *pa_read(const char *name, unsigned *n);   /* a disc file, GAM unpacked */
void pa_put(int x, int y, unsigned c);
void pa_blend(int x, int y, unsigned c, int a);
void pa_fill(int x, int y, int w, int h, unsigned c);
unsigned pa_clut_color(int cx, int cy, int i);
void pa_spr(int x, int y, int tx, int ty, int u, int v, int w, int h, int cx, int cy, int flags);
void pa_tile(int x, int y, int W, int H, int tx, int ty, int u, int v, int w, int h, int cx, int cy);
void pa_nine(int x, int y, int W, int H, int tx, int ty, int u, int v, int w, int h, int c, int cx, int cy, unsigned fill);
int pa_event_icon(int x, int y, int e, int flags);
int pa_event_icon_w(int e), pa_event_icon_h(int e);
void pa_item_icon(int x, int y, int n, int flags);
int pa_text_w(int h, const char *s);
int pa_text(int x, int y, int h, const char *s, unsigned body, unsigned edge);
int pa_text_ol(int x, int y, int h, const char *s, unsigned body, unsigned edge, unsigned outline);
int pa_text_fit(int x, int y, int h, int w, const char *s, unsigned body, unsigned edge);
int pa_wfm_dialogue(const unsigned char *wfm, unsigned n, int idx, char *out, int max);
#endif
