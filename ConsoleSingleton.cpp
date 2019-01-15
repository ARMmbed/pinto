#include "ConsoleSingleton.h"

mbed::CircularBufferFile ConsoleSingleton::console;

mbed::CircularBufferFile& ConsoleSingleton::Instance() { return console; }
