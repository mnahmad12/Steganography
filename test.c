#include <stdio.h>
#include <stdlib.h>

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d\n"

#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 

#define PRINTBIN(x)  printf(BYTETOBINARYPATTERN, BYTETOBINARY(x)); 

int main()
{
  int a=65534;
  PRINTBIN(a);
  a=a>>8;
  PRINTBIN(a);
}
