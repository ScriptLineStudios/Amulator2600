#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "tv.c"

#define WSYNC 2
#define COLUPF 8
#define PF1 0x000E

#define UPDATE_ZERO_FLAG(x) cpu.flags.z = (x == 0) ? 1:0
#define INS_COMPLETE do{ip++; break;}while(0)
#define MEMORY_SIZE 8192
#define TIA_TICK(cycles) do{for (int i = 0; i < cycles*3; i++) {tv_tick(tv, &cpu.memory);}}while(0)
static unsigned char memory[MEMORY_SIZE];

typedef struct {
    uint8_t n, v, b, d, i, z, x;
} Flags;

typedef struct {
    Flags flags;
    unsigned char *memory;
    uint8_t X, Y, A, SP;
} Cpu;  

int main(void) {
    FILE *file = fopen("roms/kernel_15.bin", "rb");
    
    Cpu cpu = {.flags={}, .memory=memory};
    TV *tv = new_tv();

    uint8_t *byte_stream = (uint8_t*)malloc(sizeof(uint8_t) * 4096);

    for (int i = 0; i < 4096; i++) {
        byte_stream[i] = fgetc(file);
    }
    fclose(file);

    bool should_exit = false;

    uint8_t byte;
    int ip = 0;
    int wsync_count = 0;
    while (ip < 4096) {
        byte = byte_stream[ip];
        // printf("%x -> %d\n", byte, byte);
        switch (byte) {
            case 216:
                TIA_TICK(2);
                cpu.flags.d = 0;
                ip++;
                break;
            case 120:
                TIA_TICK(2);
                cpu.flags.i = 1;
                ip++;
                break;
            case 162:
                //LDX
                TIA_TICK(2);
                ip++;
                cpu.X = byte_stream[ip];
                //TODO: sigN flag
                UPDATE_ZERO_FLAG(byte_stream[ip]);
                ip++;
                break;
            case 160:
                //LDY
                TIA_TICK(2);
                ip++;
                cpu.Y = byte_stream[ip];
                //TODO: sigN flag
                UPDATE_ZERO_FLAG(byte_stream[ip]);
                ip++;
                break;
            case 154:
                //TXS
                TIA_TICK(2);
                cpu.SP = cpu.X;
                UPDATE_ZERO_FLAG(cpu.X);
                ip++;
                break;
            case 232:
                // INX
                TIA_TICK(2);
                cpu.X++;
                UPDATE_ZERO_FLAG(cpu.X);
                ip++;
                break;
            case 200:
                // INY
                TIA_TICK(2);
                cpu.Y++;
                UPDATE_ZERO_FLAG(cpu.Y);
                ip++;
                break;
            case 136:
                // DEY
                TIA_TICK(2);
                cpu.Y--;
                UPDATE_ZERO_FLAG(cpu.Y);
                ip++;
                break;
            case 138:
                TIA_TICK(2);
                cpu.A = cpu.X;
                UPDATE_ZERO_FLAG(cpu.A);
                ip++;
                break;
            case 149:
                //STA
                TIA_TICK(2);
                ip++;
                uint8_t address = byte_stream[ip];
                cpu.memory[address + cpu.X] = cpu.A;
                ip++;
                break;
            case 132:
                //STY
                TIA_TICK(2);
                ip++;
                cpu.memory[byte_stream[ip]] = cpu.Y;
                ip++;
                break;
            case 208:
                //BNE
                TIA_TICK(2);
                ip++;
                int8_t jump = byte_stream[ip];
                if (cpu.flags.z == 0) {
                    // ip = jump / 32;
                    ip += jump+1;
                }
                else {
                    ip++;
                }
                break;
            case 169:
                TIA_TICK(2);
                ip++;
                cpu.A = byte_stream[ip];
                UPDATE_ZERO_FLAG(cpu.A);
                ip++;
                break;
            case 133:
                //STA
                ip++;
                TIA_TICK(2);
                if (byte_stream[ip] == WSYNC) {
                    wsync_count++;
                    while (!tv->scanline_complete) {
                        if (!tv_tick(tv, &cpu.memory)) {
                            should_exit = true;
                            break;
                        } 
                    }
                }
                cpu.memory[byte_stream[ip]] = cpu.A;
                ip++;
                break;
            case 10:
                cpu.A <<= 1; 
                UPDATE_ZERO_FLAG(cpu.A);
                ip++;
                break;
            case 202:
                cpu.X--;
                TIA_TICK(2);
                UPDATE_ZERO_FLAG(cpu.X);
                ip++;
                break;
            case 134:
                ip++;
                TIA_TICK(2);
                cpu.memory[byte_stream[ip]] = cpu.X;  
                ip++;
                break;
            case 76:
                ip++;
                TIA_TICK(2);
                ip = byte_stream[ip];
                break;
            case 224:
                ip++;
                TIA_TICK(2);
                int8_t t = cpu.X - byte_stream[ip];
                UPDATE_ZERO_FLAG(t);
                ip++;
                break;
            case 192:
                //CPY
                ip++;
                TIA_TICK(2);
                int8_t T = cpu.Y - byte_stream[ip];
                UPDATE_ZERO_FLAG(T);
                ip++;
                break;
            case 0xa5:
                //LDA
                TIA_TICK(3);
                ip++;
                cpu.A = cpu.memory[byte_stream[ip]]; 
                ip++;
                break;
            case 234:
                //NOP
                TIA_TICK(2);
                ip++;
                break;
            case 0xe6:
                ip++;
                cpu.memory[byte_stream[ip]]++;
                ip++;
                break;
            
            default:
                printf("%d\n", byte);
                assert(false);
        }
        if (!tv_tick(tv, &cpu.memory) || should_exit) {
            should_exit = true;
            break;
        }
        if (should_exit) break;
    }
    free(byte_stream);
    return 0;
}