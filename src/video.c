#include "common.h"

extern SDL_Renderer* renderer;
extern float g_scale;

static G_video g_video  = {0};

SDL_Window* window = NULL;

int SCREEN_W,SCREEN_H;

void video_setgeometry(int w,int h){
        g_video.clip_w = w;
        g_video.clip_h = h;       
}

void resize_cb(SDL_Renderer  *renderer, int w, int h) {
	SDL_Rect rect={0,0,w,h};
	SDL_RenderSetViewport(renderer,&rect);	
		
	SCREEN_W=w;
	SCREEN_H=h;					  
}

static void create_window(int width, int height) {
	
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
	
	windowFlags =  SDL_WINDOW_RESIZABLE;
	
	window = SDL_CreateWindow("sdl2arch", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);
								
	if (!window)
		 die("Failed to create window: %s", SDL_GetError());

	int w,h;
	SDL_GetWindowSize(window,&w,&h);
	
	printf("Create windows size:%dx%d\n",w,h);

	SDL_SetWindowFullscreen( window,0);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	renderer = SDL_CreateRenderer(window, -1, rendererFlags);
	
	if (!renderer)
		die("Failed to create renderer.");

	resize_cb(renderer, width, height);
}

void video_configure(const struct retro_game_geometry *geom) {
	int nwidth, nheight;

	resize_to_aspect(geom->aspect_ratio, geom->base_width * 1, geom->base_height * 1, &nwidth, &nheight);

	nwidth *= g_scale;
	nheight *= g_scale;

	if (!window)
		create_window(nwidth, nheight);

	if (g_video.texture)
		SDL_DestroyTexture(g_video.texture);

	g_video.texture = NULL;

	if (!g_video.pixfmt)
		g_video.pixfmt = SDL_PIXELFORMAT_RGBA5551;
		
        SDL_SetWindowSize(window, nwidth, nheight);
	//SDL_RenderSetLogicalSize(renderer, nwidth, nheight);
 
	g_video.texture = SDL_CreateTexture(renderer,g_video.pixfmt,SDL_TEXTUREACCESS_STREAMING, geom->max_width, geom->max_height);
	

	if (!g_video.texture)
		die("Failed to create the video texture");

	g_video.pitch = geom->base_width * g_video.bpp;

	g_video.tex_w = geom->max_width;
	g_video.tex_h = geom->max_height;
	g_video.clip_w = geom->base_width;
	g_video.clip_h = geom->base_height;

}


void resize_to_aspect(double ratio, int sw, int sh, int *dw, int *dh) {

	printf("r:%f sw:%d sh:%d ",ratio,sw,sh);
	*dw = sw;
	*dh = sh;

	if (ratio <= 0)
		ratio = (double)sw / sh;

	if ((float)sw / sh < 1)
		*dw = (int)roundf((float)(*dh * ratio));
	else
		*dh = (int)roundf((float)(*dw / ratio)); 
 
#if 0
	 if ((float)sw / sh < 1)
		*dw = *dh * ratio;
	else
		*dh = *dw / ratio;
#endif	 			
	printf("dw:%d dh:%d\n",*dw,*dh);
}


bool video_set_pixel_format(unsigned format) {

	switch (format) {
	case RETRO_PIXEL_FORMAT_0RGB1555:
		g_video.pixfmt = SDL_PIXELFORMAT_ARGB1555;
		g_video.bpp = sizeof(uint16_t);
		break;
	case RETRO_PIXEL_FORMAT_XRGB8888:
		g_video.pixfmt = SDL_PIXELFORMAT_ARGB8888;
		g_video.bpp = sizeof(uint32_t);
		break;
	case RETRO_PIXEL_FORMAT_RGB565:
		g_video.pixfmt  = SDL_PIXELFORMAT_RGB565;
		g_video.bpp = sizeof(uint16_t);
		break;
	default:
		die("Unknown pixel type %u", format);
	}

	return true;
}


void video_refresh(const void *data, unsigned width, unsigned height, unsigned pitch) {
	
	if (g_video.clip_w != width || g_video.clip_h != height) {
		g_video.clip_h = height;
		g_video.clip_w = width;
	}
	if (pitch != g_video.pitch) {
		g_video.pitch = pitch;
		printf("change pitch:%d\n",pitch);
		printf("refresh t(%d,%d) c(%d,%d)\n",g_video.tex_w,g_video.tex_h,g_video.clip_w,g_video.clip_h);
	}

	if (data) {
				
		Uint32 *pixels;
		void *tmp;
		Uint32 *dst;
		
		int mpitch;
		
		SDL_LockTexture(g_video.texture,NULL, &tmp, &mpitch);
	
		pixels = tmp;		

		for(int i=0;i<height;i++){
		 	dst = (Uint32*)((Uint8*)pixels + i * mpitch);
			memcpy(dst, data, pitch);
			data+=pitch;
		}
		
		SDL_UnlockTexture(g_video.texture);						
	}
	
}

void video_render() {
	
	SDL_Rect src={0,0, g_video.clip_w, g_video.clip_h};
	SDL_RenderCopy(renderer, g_video.texture, &src, NULL);
}

void video_deinit() {

	if (g_video.texture)
		SDL_DestroyTexture(g_video.texture);

	g_video.texture = NULL;
}
