#include <cstring>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "output.h"

Output* Output::output;

Output* Output::Inst()
{
    if(!output)
        output = new Output();

    return output;
}


Output::Output()
{
}

void Output::kprintf(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);

    char buffer[256];
    sprintf(buffer, msg, args);

    va_end(args);


}
