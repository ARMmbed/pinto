#ifndef __PINTO_COMPRESSION_H_
#define __PINTO_COMPRESSION_H_
#include "pinto_compress.h"

/*class CompressEngineInterface {
    public:
        virtual ~CompressEngineInterface();
        virtual size_t operator() (const void* data, size_t len, void* out) = 0;
};
*/
#define GOLOMB_BUFF_MULTIPLIER 1.5

class DoNoCompression {
    public:
        inline size_t operator() (const void* data, size_t len, void* out) { out = const_cast<void*>(data); return len; }
};

class GolombTextCompression { 
    public:
        GolombTextCompression();
        ~GolombTextCompression();
        size_t operator() (const void* data, size_t len, void* out);

    private:
       uint8_t* buffer;
       const size_t buff_size = (size_t) (GOLOMB_BUFF_MULTIPLIER*MBED_CONF_PINTO_CIRCULAR_BUFFER_FILE_DEPTH);
};
#endif
