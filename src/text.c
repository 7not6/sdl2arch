#include "common.h"
#include "font10x16.h"

extern SDL_Renderer* renderer;

static char drawTextBuffer[MAX_LINE_LENGTH];
static SDL_Texture *fontTexture;

void initFonts(void)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "init font10x16 texture" );	
	//fontTexture =  IMG_LoadTexture(renderer, "./font10x16.png");	
	SDL_Surface *tmp = NULL;
	unsigned char tmpfont[FONT10X16_WIDTH*FONT10X16_HEIGHT*4];
	
	int i,j,n,ind=0;
	Uint32 *ptr=(Uint32*)tmpfont;
	unsigned char binary[8];
	
	for(j=0;j<FONT10X16_HEIGHT;j++){
	
		for(i=0;i<FONT10X16_WIDTH;i+=8){
		
			unsigned char val=font10x16[ind++];
			
			for(n = 0; n < 8; n++)
			 	binary[7-n] = (val >> n) & 1;
			 	
			for(n = 0; n < 8; n++){	
				if(binary[n] == 1)*ptr=0xffffffff;
				else *ptr=0;	
				ptr++;
			} 						
		}		
	}
	
	tmp = SDL_CreateRGBSurfaceFrom((void *)tmpfont, FONT10X16_WIDTH, FONT10X16_HEIGHT, 32,FONT10X16_WIDTH*4, 0xff000000, 0x00ff0000, 0x0000ff00,0x000000ff);
	
	if(NULL == tmp){
		fprintf(stderr, "Error SDL_CreateRGBSurface tmp: %s", SDL_GetError());	
		goto quit;
	}
		
	fontTexture = SDL_CreateTextureFromSurface(renderer, tmp);
	
	SDL_FreeSurface(tmp);
	
	quit:

	if (!fontTexture)printf("create font failed\n");
}

void deinitFonts(void)
{
	if (fontTexture){
		//printf("destroy font\n");
		SDL_DestroyTexture(fontTexture);
		
	}
}

void drawChar(int x, int y, int r, int g, int b, char c)
{
	SDL_Rect rect;
	
	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	
	SDL_SetTextureColorMod(fontTexture, r, g, b);
	
	int cy=c/GLYPH_HEIGHT;
	int cx=c-cy*GLYPH_HEIGHT;
		
	rect.y = (cy*GLYPH_HEIGHT);
	rect.x = (cx*GLYPH_WIDTH);
					
	blitRect(fontTexture, &rect, x, y);
}

void drawCharScale(int x, int y, int r, int g, int b, char c,int scale)
{
	SDL_Rect rect;
	
	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	
	SDL_SetTextureColorMod(fontTexture, r, g, b);
	
	int cy=c/GLYPH_HEIGHT;
	int cx=c-cy*GLYPH_HEIGHT;
		
	rect.y = (cy*GLYPH_HEIGHT);
	rect.x = (cx*GLYPH_WIDTH);
					
	blitRectScale(fontTexture, &rect, x, y,scale);
}

void drawText(int x, int y, int r, int g, int b, int align,char *format, ...)
{
	int i, len, c;
	SDL_Rect rect;
	va_list args;
	
	memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

	va_start(args, format);
	vsprintf(drawTextBuffer, format, args);
	va_end(args);
	
	len = strlen(drawTextBuffer);
	
	switch (align)
	{
		case TEXT_RIGHT:
			x -= (len * GLYPH_WIDTH);
			break;
			
		case TEXT_CENTER:
			x -= (len * GLYPH_WIDTH) / 2;
			break;
	}
	
	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	rect.y = 0;
	
	SDL_SetTextureColorMod(fontTexture, r, g, b);
		
	for (i = 0 ; i < len ; i++)
	{
		c = drawTextBuffer[i];
		
		int cy=c/GLYPH_HEIGHT;
		int cx=c-cy*GLYPH_HEIGHT;
		
		rect.y = (cy*GLYPH_HEIGHT);
		rect.x = (cx*GLYPH_WIDTH);
					
		blitRect(fontTexture, &rect, x, y);
		
		x += GLYPH_WIDTH;

	}
	
}

void drawTextScale(int x, int y, int r, int g, int b, int align,int scale,char *format, ...)
{
	int i, len, c;
	SDL_Rect rect;
	va_list args;
	
	memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

	va_start(args, format);
	vsprintf(drawTextBuffer, format, args);
	va_end(args);
	
	len = strlen(drawTextBuffer);
	
	switch (align)
	{
		case TEXT_RIGHT:
			x -= (len * GLYPH_WIDTH);
			break;
			
		case TEXT_CENTER:
			x -= (len * GLYPH_WIDTH) / 2;
			break;
	}
	
	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	rect.y = 0;
	
	SDL_SetTextureColorMod(fontTexture, r, g, b);
		
	for (i = 0 ; i < len ; i++)
	{
		c = drawTextBuffer[i];
		
		int cy=c/GLYPH_HEIGHT;
		int cx=c-cy*GLYPH_HEIGHT;
		
		rect.y = (cy*GLYPH_HEIGHT);
		rect.x = (cx*GLYPH_WIDTH);					
		
		blitRectScale(fontTexture, &rect, x, y,scale);
		x += GLYPH_WIDTH*scale;

	}
	
}
