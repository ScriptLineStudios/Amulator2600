#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    int scanline, clock;
    bool scanline_complete;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *tex;
    Uint32 *pixels;
} TV;

#define RGB(r, g, b) ((((r)&0xFF)<<(8*0)) | (((g)&0xFF)<<(8*1)) | (((b)&0xFF)<<(8*2)) | (((255)&0xFF)<<(8*3)))
#define RANGE(lower, upper) data >= lower && data < upper
#define SET_PIXEL(_r, _g, _b) do{*r=_r; *g=_g; *b=_b;}while(0)
#define COLOR(lower, upper, r, g, b) if (RANGE(lower, upper)) SET_PIXEL(r, g, b)
#define SCALE 1

void get_color(Uint32 data, Uint8 *r, Uint8 *b, Uint8 *g) {
    COLOR(0, 2, 0, 0, 0);
    COLOR(2, 4, 26, 26, 26);
    COLOR(4, 6, 57, 57, 57);
    COLOR(6, 8, 91, 91, 91);
    COLOR(8, 10, 126, 126, 126);
    COLOR(10, 12, 162, 162, 162);
    COLOR(12, 14, 199, 199, 199);
    COLOR(14, 16, 237, 237, 237);
    COLOR(16, 18, 25, 2, 2);
    COLOR(18, 20, 58, 31, 7);
    COLOR(20, 22, 93, 65, 15);
    COLOR(22, 24, 130, 101, 24);
    COLOR(24, 26, 167, 137, 33);
    COLOR(26, 28, 204, 173, 42);
    COLOR(28, 30, 242, 211, 51);
    COLOR(30, 32, 251, 238, 65);
    COLOR(32, 34, 55, 9, 7);
    COLOR(34, 36, 94, 22, 19);
    COLOR(36, 38, 131, 41, 30);
    COLOR(38, 40, 169, 75, 40);
    COLOR(40, 42, 206, 109, 50);
    COLOR(42, 44, 241, 144, 59);
    COLOR(44, 46, 246, 179, 57);
    COLOR(46, 48, 254, 223, 111);
    COLOR(48, 50, 72, 15, 12);
    COLOR(50, 52, 115, 29, 25);
    COLOR(52, 54, 152, 42, 37);
    COLOR(54, 56, 190, 55, 48);
    COLOR(56, 58, 228, 84, 59);
    COLOR(58, 60, 238, 115, 84);
    COLOR(60, 62, 241, 154, 127);
    COLOR(62, 64, 246, 197, 186);
    COLOR(64, 66, 68, 14, 11);
    COLOR(66, 68, 112, 29, 31);
    COLOR(68, 70, 151, 46, 64);
    COLOR(70, 72, 188, 63, 98);
    COLOR(72, 74, 226, 82, 133);
    COLOR(74, 76, 234, 98, 168);
    COLOR(76, 78, 236, 137, 212);
    COLOR(78, 80, 240, 182, 245);
    COLOR(80, 82, 47, 28, 74);
    COLOR(82, 84, 90, 45, 102);
    COLOR(84, 86, 128, 65, 140);
    COLOR(86, 88, 165, 84, 177);
    COLOR(88, 90, 202, 104, 215);
    COLOR(90, 92, 231, 123, 253);
    COLOR(92, 94, 233, 135, 245);
    COLOR(94, 96, 240, 180, 246);
    COLOR(96, 98, 29, 52, 130);
    COLOR(98, 100, 61, 67, 162);
    COLOR(100, 102, 95, 86, 201);
    COLOR(102, 104, 129, 106, 240);
    COLOR(104, 106, 161, 114, 253);
    COLOR(106, 108, 195, 118, 254);
    COLOR(108, 110, 235, 152, 254);
    COLOR(110, 112, 242, 191, 248);
    COLOR(112, 114, 26, 58, 145);
    COLOR(114, 116, 41, 78, 189);
    COLOR(116, 118, 61, 96, 228);
    COLOR(118, 120, 85, 108, 252);
    COLOR(120, 122, 110, 110, 252);
    COLOR(122, 124, 144, 138, 253);
    COLOR(124, 126, 183, 176, 253);
    COLOR(126, 128, 223, 216, 254);
    COLOR(128, 130, 18, 44, 114);
    COLOR(130, 132, 29, 70, 170);
    COLOR(132, 134, 32, 89, 214);
    COLOR(134, 136, 43, 106, 251);
    COLOR(136, 138, 65, 128, 251);
    COLOR(138, 140, 100, 165, 251);
    COLOR(140, 142, 137, 206, 251);
    COLOR(142, 144, 176, 246, 251);
    COLOR(144, 146, 1, 19, 58);
    COLOR(146, 148, 0, 49, 110);
    COLOR(148, 150, 0, 85, 162);
    COLOR(150, 152, 26, 121, 200);
    COLOR(152, 154, 48, 157, 238);
    COLOR(154, 156, 75, 193, 250);
    COLOR(156, 158, 105, 233, 250);
    COLOR(158, 160, 141, 249, 250);
    COLOR(160, 162, 11, 32, 2);
    COLOR(162, 164, 30, 68, 39);
    COLOR(164, 166, 48, 106, 88);
    COLOR(166, 168, 66, 142, 123);
    COLOR(168, 170, 84, 178, 159);
    COLOR(170, 172, 102, 215, 195);
    COLOR(172, 174, 116, 246, 234);
    COLOR(174, 176, 131, 249, 250);
    COLOR(176, 178, 14, 36, 4);
    COLOR(178, 180, 35, 75, 8);
    COLOR(180, 182, 57, 113, 17);
    COLOR(182, 184, 77, 150, 47);
    COLOR(184, 186, 97, 188, 79);
    COLOR(186, 188, 117, 225, 112);
    COLOR(188, 190, 121, 237, 148);
    COLOR(190, 192, 144, 240, 181);
    COLOR(192, 194, 12, 34, 3);
    COLOR(194, 196, 33, 71, 7);
    COLOR(196, 198, 54, 108, 7);
    COLOR(198, 200, 75, 145, 8);
    COLOR(200, 202, 95, 181, 17);
    COLOR(202, 204, 116, 219, 41);
    COLOR(204, 206, 132, 233, 74);
    COLOR(206, 208, 180, 236, 105);
    COLOR(208, 210, 6, 22, 2);
    COLOR(210, 212, 24, 54, 1);
    COLOR(212, 214, 48, 89, 0);
    COLOR(214, 216, 83, 126, 0);
    COLOR(216, 218, 118, 163, 0);
    COLOR(218, 220, 154, 200, 6);
    COLOR(220, 222, 190, 234, 31);
    COLOR(222, 224, 228, 237, 64);
    COLOR(224, 226, 26, 2, 2);
    COLOR(226, 228, 59, 31, 7);
    COLOR(228, 230, 94, 65, 16);
    COLOR(230, 232, 131, 101, 25);
    COLOR(232, 234, 168, 136, 34);
    COLOR(234, 236, 206, 174, 43);
    COLOR(236, 238, 244, 211, 52);
    COLOR(238, 240, 251, 238, 65);
    COLOR(240, 242, 56, 10, 8);
    COLOR(242, 244, 95, 23, 19);
    COLOR(244, 246, 132, 41, 30);
    COLOR(246, 248, 170, 75, 41);
    COLOR(248, 250, 208, 109, 51);
    COLOR(250, 252, 241, 143, 59);
    COLOR(252, 254, 246, 179, 57);
    COLOR(254, 256, 254, 223, 112);
}

TV *new_tv() {
    SDL_Init(SDL_INIT_VIDEO);
    TV *tv = malloc(sizeof(TV));
    tv->scanline = 0;
    tv->clock = 0;
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

char *int_to_bitstring_alloc(int x, int count) {
    count = count<1 ? sizeof(x)*8 : count;
    char *pstr = malloc(count+1);
    for(int i = 0; i<count; i++)
        pstr[i] = '0' | ((x>>(count-1-i))&1);
    pstr[count]=0;
    return pstr;
}

void set_pf1(TV *tv, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    int render_x = (x * 4) + 16;
    for (int i = 0; i < 4; i++) {
        tv->pixels[y * (160 * SCALE) + (render_x+i)] = RGB(r, g, b);
    }
}

void set_pf0(TV *tv, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    int render_x = (x * 4);
    for (int i = 0; i < 4; i++) {
        tv->pixels[y * (160 * SCALE) + (render_x+i)] = RGB(r, g, b);
    }
}

void set_pf2(TV *tv, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    int render_x = (x * 4) + 48;
    for (int i = 0; i < 4; i++) {
        tv->pixels[y * (160 * SCALE) + (render_x+i)] = RGB(r, g, b);
    }
}

void set_pf2_mirror(TV *tv, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    int render_x = (x * 4) + 128;
    for (int i = 0; i < 4; i++) {
        tv->pixels[y * (160 * SCALE) + (render_x+i)] = RGB(r, g, b);
    }
}

void set_pf0_mirror(TV *tv, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    int render_x = (x * 4) + 80;
    for (int i = 0; i < 4; i++) {
        tv->pixels[y * (160 * SCALE) + (render_x+i)] = RGB(r, g, b);
    }
}

void set_pf1_mirror(TV *tv, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    int render_x = (x * 4) + 96;
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


bool tv_tick(TV *tv, unsigned char **memory) {

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
        char *string = int_to_bitstring_alloc((*memory)[0x000D], 4);
        string = reverse(string, 4);
        
        if (x >= 0 && x <= 16) {
            int dx = x % 4;
            if (string[dx] == '1') {
                get_color((*memory)[0x0008], &r, &g, &b);
                set_pf0(tv, dx, y, r, g, b);        
            }
        }

        else if (x >= 80 && x <= 80+16) {
            int dx = x % 4;
            if (string[dx] == '1') {
                get_color((*memory)[0x0008], &r, &g, &b);
                set_pf0_mirror(tv, dx, y, r, g, b);        
            }
        }

        //PF1
        string = int_to_bitstring_alloc((*memory)[0x0000E], 8);

        if (x >= 16 && x <= 16 + 32) {
            int dx = x % 8;
            if (string[dx] == '1') {
                get_color((*memory)[0x0008], &r, &g, &b);
                set_pf1(tv, dx, y, r, g, b);        
            }
        }

        else if (x >= 96 && x <= 96 + 32) {
            int dx = x % 8;
            if (string[dx] == '1') {
                get_color((*memory)[0x0008], &r, &g, &b);
                set_pf1_mirror(tv, dx, y, r, g, b);        
            }
        }

        //PF2
        string = int_to_bitstring_alloc((*memory)[0x000F], 8);
        string = reverse(string, 8);
        
        if (x >= 48 && x <= 80) {
            int dx = x % 8;
            if (string[dx] == '1') {
                get_color((*memory)[0x0008], &r, &g, &b);
                set_pf2(tv, dx, y, r, g, b);        
            }
        }
        else if (x >= 128 && x <= 160) {
            int dx = x % 8;
            if (string[dx] == '1') {
                get_color((*memory)[0x0008], &r, &g, &b);
                set_pf2_mirror(tv, dx, y, r, g, b);        
            }
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