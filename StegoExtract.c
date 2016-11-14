//EXTRACTION#
#include <string.h>
#include "image.h"


/*
       THE WAY BYTES ARE INDEXED
  00000000 00000000 00000000 00000000
      3       2        1        0
*/


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


byte getbit(byte b0, int bitnum)
{
  //passing in a byte and getting the bit at index bitnum
  b0 = b0>>bitnum;
  //what does >> do?
  b0 = b0  & 0x01; 
  return b0;

}

byte getlsbs(byte *p)
{
  return(getbit(*p,0));
 
  
}


byte setbit(byte *b0, int bitnum, int val)
{
  byte mask=1;
  mask=mask<<bitnum;
 
  if(val)//meaning val is equal to 1
    {
      byte ch= mask | *b0;
      
      return ch;
      
    }

  
  else //meaning val = 0
    {
      byte ch = ~mask & *b0;
      
      return ch;
    }

}

int main(int argc, char *argv[])
{
   struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
   ReadImage(argv[1],&img);

   //image is stored in img, needs to now be processed

   //GETTING first 2 size bytes
   int i;
   byte first=255;
   byte second=255;
   for(i=0;i<8;i++)
     {
       first=setbit(&first,7-i,getlsbs(&GetGray(i)));
       
     }

   for(i=8;i<16;i++)
     {
       second=setbit(&second,16-i,getlsbs(&GetGray(i)));
      ;
     }

   //HAVE FIRST & SECOND SIZE BYTES:
   printf("The First Byte of Size is: "); PRINTBIN(first);
   printf("The Second Byte of Size is: "); PRINTBIN(second);
}
