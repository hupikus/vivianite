#include "res.h"

#include "fonts.h"

bool InitResources(void)
{
    return InitFonts();
}

void DestroyResources(void)
{
    DestroyFonts();
}
