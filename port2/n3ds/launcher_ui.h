/* The 3DS start screen's drawing (launcher_ui.c), apart from the console so that a PC can draw it too */
#ifndef LAUNCHER_UI_H
#define LAUNCHER_UI_H
typedef struct {
    int row;                 /* the option chosen: 0 language, 1 movies, 2 speed, 3 picture */
    const char *lang;        /* the language's name (Latin-1) */
    int lang_es_improved;    /* 1: the improved Spanish translation is the language */
    int es_ready;            /* its files are on the SD card already */
    int movies, hz, picture; /* 1 play / 0 skip; 60 / 50; 0 4:3, 1 wide, 2 3D */
    const char *disc;        /* the disc image's name */
    int new3ds;
    int start_down;          /* the Start button is being touched */
    int pressed;             /* the touched button (runtime/launcher_game.c): 0 none, 1 Start, 2 up, 3 down, 4 left, 5 right */
} LuiState;
/* top: 400 x 240, bottom: 320 x 240, both RGB, three bytes a pixel, rows from the top */
void lui_draw(unsigned char *top, unsigned char *bot, const LuiState *s);
/* the Start button's rectangle on the bottom screen (touch) */
void lui_start_rect(int *x0, int *y0, int *x1, int *y1);
#endif
