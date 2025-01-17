#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <gdt.h>
#include <kernel.h>
#include <segment.h>

#define GDT_DATA 0 << 3 // Data Segment
#define GDT_CODE 1 << 3  // Code Segment

#define GDT_DIRECTION_UP 0 << 2
#define GDT_DIRECTION_DOWN 1 << 2

#define GDT_CONFORM_OFF 0 << 2
#define GDT_CONFORM_ON 1 << 2

#define GDT_READ_OFF 0 << 1
#define GDT_READ_ON 1 << 1
#define GDT_WRITE_OFF 0 << 1
#define GDT_WRITE_ON 1 << 1

#define GDT_ACCESSED_OFF 0
#define GDT_ACCESSED_ON 1


#define GDT_GRANULARITY_1B 0 << 3
#define GDT_GRANULARITY_4K 1 << 3
#define GDT_GRANULARITY_1M 0 << 3
#define GDT_GRANULARITY_4G 1 << 3

#define GDT_16_BIT 0b00 << 1
#define GDT_32_BIT 0b10 << 1
#define GDT_64_BIT 0b01 << 1

typedef struct {
    uint16_t limit_low : 16;
    uint16_t base_low : 16;
    uint8_t base_mid : 8;
    uint8_t access_byte : 8;
    uint8_t limit_high : 4;
    uint8_t flags : 4;
    uint8_t base_high : 8;
} __attribute__((packed)) gdt_entry_real;

union gdt_entry {
    gdt_entry_real entry;
    uint64_t raw;
};

static gdt_entry gdt_array[] = {
    {},  // Null Descriptor,
    {{   // 16-bit Code
        0xFFFF, 0x0000, 0x00,
        GDT_ACCESSED_OFF | GDT_READ_ON | GDT_CONFORM_OFF | GDT_CODE | SYS_SD_CDS |
            SYS_SD_RING_0 | SYS_SD_PRESENT,
        0x0, GDT_16_BIT | GDT_GRANULARITY_1B, 0x00}},
    {{// 16-bit Data
        0xFFFF, 0x0000, 0x00,
        GDT_ACCESSED_OFF | GDT_WRITE_ON | GDT_DIRECTION_UP | GDT_DATA |
            SYS_SD_CDS | SYS_SD_RING_0 | SYS_SD_PRESENT,
        0x0, GDT_16_BIT | GDT_GRANULARITY_1B, 0x00}},
    {{// 32-bit Code
        0xFFFF, 0x0000, 0x00,
        GDT_ACCESSED_OFF | GDT_READ_ON | GDT_CONFORM_OFF | GDT_CODE | SYS_SD_CDS |
            SYS_SD_RING_0 | SYS_SD_PRESENT,
        0xF, GDT_32_BIT | GDT_GRANULARITY_4K, 0x00}},
    {{// 32-bit Data
        0xFFFF, 0x0000, 0x00,
        GDT_ACCESSED_OFF | GDT_WRITE_ON | GDT_DIRECTION_UP | GDT_DATA |
            SYS_SD_CDS | SYS_SD_RING_0 | SYS_SD_PRESENT,
        0xF, GDT_32_BIT | GDT_GRANULARITY_4K, 0x00}},
    {{// 64-bit Code
        0xFFFF, 0x0000, 0x00,
        GDT_ACCESSED_OFF | GDT_READ_ON | GDT_CONFORM_OFF | GDT_CODE | SYS_SD_CDS |
            SYS_SD_RING_0 | SYS_SD_PRESENT,
        0xF, GDT_64_BIT | GDT_GRANULARITY_1M, 0x00}},
    {{// 64-bit Data
        0xFFFF, 0x0000, 0x00,
        GDT_ACCESSED_OFF | GDT_WRITE_ON | GDT_DIRECTION_UP | GDT_DATA |
            SYS_SD_CDS | SYS_SD_RING_0 | SYS_SD_PRESENT,
        0xF, GDT_64_BIT | GDT_GRANULARITY_1M, 0x00}}
};

struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdtr;

void gdt::Load() {
    gdtr.limit = sizeof(gdt_array) - 1;
    gdtr.base = (uintptr_t)&gdt_array;
    __asm__ volatile("lgdt %0" ::"m"(gdtr));
    krnl::Printf_ok("GDT Loaded");
}

extern "C" void asm_gdt_reload_segment(size_t cs, size_t ds);

void gdt::SetSegments(size_t code_segment, size_t data_segment) {
    asm_gdt_reload_segment(code_segment, data_segment);
    krnl::Printf_ok("Switched GDT Segment - CS: %#4X DS: %#4X", code_segment, data_segment);
}

