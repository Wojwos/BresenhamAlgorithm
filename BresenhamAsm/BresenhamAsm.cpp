// BresenhamAsm.cpp : Defines the exported functions for the DLL application.
//

#include "header.h"
#include "BresenhamAsm.h"


// This is an example of an exported variable
BRESENHAMASM_API int nBresenhamAsm=0;

// This is an example of an exported function.
BRESENHAMASM_API int fnBresenhamAsm(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
CBresenhamAsm::CBresenhamAsm()
{
    return;
}
