#include "toBinary.h"

#include "sam.h"
void toBinary(uint8_t a)
{
    uint8_t i;

    for(i=0x80;i!=0;i>>=1)
        printf("%d",(a&i)?1:0);
}