#include <cstdint>

#pragma pack(push, 1)

struct BMPfileheader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
};

struct BMPinfoheader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};

struct Pixel {
    uint8_t b; // blue
    uint8_t g; // green
    uint8_t r; // red
};

#pragma pack(pop)
