#include "compression.h"

GolombTextCompression::GolombTextCompression() : buffer(new uint8_t[buff_size]) {}
GolombTextCompression::~GolombTextCompression() { delete[] buffer; }

size_t GolombTextCompression::operator() (const void* data, size_t len, void* out) {
    out = buffer;
    size_t compressed_size = GolombRice::compress(data, len, buffer, ascii_remap);
    return compressed_size;
}
