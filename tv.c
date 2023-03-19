#include <SDL2/SDL.h>
#include <stdbool.h>

#include "utils/utils.c"

typedef struct {
    int scanline, clock;
    bool scanline_complete;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *tex;
    Uint32 *pixels;
    bool play_field_mirrored;
} TV;

#define PLAY_FIELD(lower, upper, size, offset, function) do{if (x >= lower && x <= upper) {int dx = x % size;if (string[dx] == '1') {get_color((*memory)[0x0008], &r, &g, &b);function(tv, dx, y, r, g, b, offset);}}}while(0)                                             
#define SCALE 1

TV *new_tv() {
    SDL_Init(SDL_INIT_VIDEO);
    TV *tv = malloc(sizeof(TV));
    tv->scanline = 0;
    tv->clock = 0;
    tv->play_field_mirrored = true;
    tv->pixels = malloc(sizeof(Uint32) * 160 * SCALE * 192 * SCALE);
    for (int i = 0; i < 160 * SCALE * 192 * SCALE; i++) {
        tv->pixels[i] = 0;
    }
    tv->window = SDL_CreateWindow("TV",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          160 * SCALE, (192) * SCALE,
                                          0);
    tv->renderer = SDL_CreateRenderer(tv->window, -1, SDL_RENDERER_ACCELERATED | SDL_SWSURFACE);
    tv->tex = SDL_CreateTexture(tv->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 160 * SCALE, 192 * SCALE);
    return tv;
}


void set_pf1(TV *tv, int x, int y, Uint8 r, Uint8 g, Uint8 b, int offset) {
    int render_x = (x * 4) + offset;
    for (int i = 0; i < 4; i++) {
        tv->pixels[y * (160 * SCALE) + (render_x+i)] = RGB(r, g, b);
    }
}


void set_pf0(TV *tv, int x, int y, Uint8 r, Uint8 g, Uint8 b, int offset) {
    int render_x = (x * 4) + offset;
    for (int i = 0; i < 4; i++) {
        tv->pixels[y * (160 * SCALE) + (render_x+i)] = RGB(r, g, b);
    }
}

void set_pf2(TV *tv, int x, int y, Uint8 r, Uint8 g, Uint8 b, int offset) {
    int render_x = (x * 4) + offset;
    for (int i = 0; i < 4; i++) {
        tv->pixels[y * (160 * SCALE) + (render_x+i)] = RGB(r, g, b);
    }
}

char *reverse(char *string, int length) {
    char *new = malloc(sizeof(char)*strlen(string));

    if (length == 4) {
        new[0] = string[3];
        new[1] = string[2];
        new[2] = string[1];
        new[3] = string[0];
        new[4] = '\0';
    }    
    else {
        new[0] = string[7];
        new[1] = string[6];
        new[2] = string[5];
        new[3] = string[4];
        new[4] = string[3];
        new[5] = string[2];
        new[6] = string[1];
        new[7] = string[0];
        new[8] = '\0';
    }

    free(string);

    return new;
}


bool tv_tick(TV *tv, unsigned char **memory, Cpu *cpu) {
    SDL_Event e;
    while(SDL_PollEvent(&e) > 0) {
        switch(e.type) {
            case SDL_QUIT:
                return false;
        }
    }
    tv->scanline_complete = false;

    tv->clock++;
    if (tv->clock >= 68 && tv->scanline >= 39 && tv->scanline <= (192*SCALE)+38) {        
        SDL_PixelFormat form;
        form.format = SDL_PIXELFORMAT_RGB555;   

        Uint8 r, g, b, a;
        a = 255;

        get_color((*memory)[9], &r, &g, &b);

        int x = (tv->clock - 68) * SCALE;
        int y = (tv->scanline - 39) * SCALE;

        tv->pixels[y * 160 * SCALE + x] = RGB(r, g, b);

        //Playfield

        /*
             PF0    PF1       PF2      |
            ---- -------- --------     |
            0000 00000000 00000000     | 
             16     32       32        |

             16     48       80

            Each zero = 4 pixels
            20 * 4 * 2 = 160
        */
        
        //PF0
        char *string = int_to_bitstring_alloc((*memory)[0x000D], 8);
        string[4] = '\0';
        string = reverse(string, 4);

        PLAY_FIELD(0, 16, 4, 0, set_pf0);
        if (!tv->play_field_mirrored) {
            PLAY_FIELD(80, 96, 4, 80, set_pf0);
        }
        else {
            string = reverse(string, 4);
            PLAY_FIELD(144, 160, 4, 144, set_pf0);
        }

        //PF1
        string = int_to_bitstring_alloc((*memory)[0x0000E], 8);

        PLAY_FIELD(16, 48, 8, 16, set_pf1);
        if (!tv->play_field_mirrored) {
            PLAY_FIELD(96, 128, 8, 96, set_pf1);
        }
        else {
            string = reverse(string, 8);
            PLAY_FIELD(112, 144, 8, 112, set_pf1);
        }
        //PF2
        string = int_to_bitstring_alloc((*memory)[0x000F], 8);
        string = reverse(string, 8);
        

        PLAY_FIELD(48, 80, 8, 48, set_pf2);
        if (!tv->play_field_mirrored) {
            PLAY_FIELD(128, 160, 8, 128, set_pf2);
        }
        else {
            string = reverse(string, 8);
            PLAY_FIELD(80, 112, 8, 80, set_pf2);
        }
    
        free(string);
    }
    if (tv->scanline >= 262) {
        SDL_UpdateTexture(tv->tex, NULL, (const void *)tv->pixels, 1);
        SDL_SetRenderDrawColor(tv->renderer, 0, 0, 0, 255);
        SDL_RenderClear(tv->renderer);
        SDL_RenderCopy(tv->renderer, tv->tex, NULL,NULL);
        SDL_RenderPresent(tv->renderer);
        tv->clock = 0;
        tv->scanline = 0;
    }
    if (tv->clock >= 228) {
        
        tv->scanline_complete = true;
        tv->scanline++;
        tv->clock = 0;
    }
    // SDL_Delay(1000/1000);

    return true;
}