// BresenhamC.cpp : Defines the exported functions for the DLL application.
//

#include "header.h"
#include "BresenhamC.h"


// This is an example of an exported variable
BRESENHAMC_API int nBresenhamC=0;

// This is an example of an exported function.
BRESENHAMC_API int fnBresenhamC(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
CBresenhamC::CBresenhamC()
{
    return;
}
