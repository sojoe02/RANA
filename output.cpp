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
