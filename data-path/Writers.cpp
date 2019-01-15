#include "Writers.h"

WriterInterface::~WriterInterface() {}
void WriterInterface::init(void* client) {}
bool WriterInterface::ready(void){ return true; }
