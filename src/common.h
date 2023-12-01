#include <SDL2/SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "libretro.h"

#include "defs.h"
#include "structs.h"

extern bool quit;

//fifo
void fifo_init(fifo_t * f, char * buf, int size);
int fifo_read(fifo_t * f, void * buf, int nbytes);
int fifo_write(fifo_t * f, const void * buf, int nbytes);

//video
void resize_cb(SDL_Renderer  *renderer, int w, int h);
void video_refresh(const void *data, unsigned width, unsigned height, unsigned pitch);
void video_deinit();
void video_configure(const struct retro_game_geometry *geom);
void video_setgeometry(int w,int h);
bool video_set_pixel_format(unsigned format);
void resize_to_aspect(double ratio, int sw, int sh, int *dw, int *dh);
void video_render();

//input
int doInput(void);
void init_keyboards();

//audio
void audio_init(int frequency);
void audio_deinit();
bool audio_stop();
bool audio_start();
bool audio_alive();
size_t audio_write(const void *buf, unsigned frames);

//text
void initFonts(void);
void drawText(int x, int y, int r, int g, int b, int align,char *format, ...);
void drawTextScale(int x, int y, int r, int g, int b, int align,int scale,char *format, ...);
void drawChar(int x, int y, int r, int g, int b, char c);
void drawCharScale(int x, int y, int r, int g, int b, char c,int scale);
void deinitFonts(void);

//utli
void die(const char *fmt, ...);

//render
void prepareScene(void);
void presentScene(void);
void blit(SDL_Texture *texture, int x, int y);
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);
void blitRectScale(SDL_Texture *texture, SDL_Rect *src, int x, int y,int scale);

// widget
void initWidgets(void);
void cleanWidgets();
Widget *createWidget(char *name);

//menu
void menu_setDelegate();
void initMenu(void);

//emu
void emulation_pause();
void emulation_start();
void emulation_cleanup(void);
void emulation_quit();

//core
void core_run();
void core_reset();
int core_main(int argc, char *argv[]);
void core_unload();
void save_savestate(const char *savestated);
void options_deinit();

//core-options
void split(char *string,const char sep[],int ind);
void freesuboptval(int ind);

