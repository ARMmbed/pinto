#ifndef __PINTO_CONSOLE_SINGLETON_H__
#define __PINTO_CONSOLE_SINGLETON_H__

#include "CircularBufferFile.h"

class ConsoleSingleton {
    public:
        ConsoleSingleton();
        static mbed::CircularBufferFile& Instance(); //{ return console; }
    private:
        static mbed::CircularBufferFile console;
};

#endif
