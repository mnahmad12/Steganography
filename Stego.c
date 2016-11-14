/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * Stego.c: A program for manipulating images                           *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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

byte getByte(byte b0,int byteNum)
{
  return b0>>byteNum*8;
}



byte getbit(byte b0, int bitnum)
{
  //passing in a byte and getting the bit at index bitnum
  b0 = b0>>bitnum;
  //what does >> do?
  b0 = b0  & 0x01; 
  return b0;

}


//Setting the bit at a specific index:
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




void setlsbs(byte *p,int val)
{
  //Replacing the bits of p with lsb of b0
  
  *p=setbit(p,0,val);
   
}



int main(int argc, char *argv[])
{  
  int i, j, k, cover_bits, bits;
  struct Buffer b = {NULL, 0, 0};
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
  char *s;
  byte b0;
   
  if (argc != 4) 
    {
      printf("\n%s <cover_file> <stego_file> <file_to_hide> \n", argv[0]);
      exit(1);
    }
  ReadImage(argv[1],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros
  ReadBinaryFile(argv[3],&b);    // Read binary data
 
  s = strchr(argv[3],(int)'.');
  if (strlen(s)!=4) s = ".txt";
  printf("hidden file type = <%s>\n",s+1);

  // hidden information 
  // first two bytes is the size of the hidden file
  // next 4 bytes is the extension (3 letters & \0)
  if (!GetColor)
    cover_bits = img.NofC*img.NofR;
  else 
    cover_bits = 3*img.NofC*img.NofR;    
  bits = (6+b.size)*8;
  if (bits > cover_bits)
    {
      printf("Cover file is not large enough %d (bits) > %d (cover_bits)\n",bits,cover_bits);
      exit(1);
    }

  // embed two size bytes
  
  //getting the 2 least significant BYTES of the integer:
  b0 = getByte(b.size,0);
  byte b1 = getByte(b.size,1);
  
  int bitItter;

  //THE FIRST LSB of the size is inserted into the image:
  
  for(bitItter=0;bitItter<8;bitItter++)
    {
      byte needToModify=GetGray(bitItter);
     
      //changing this byte
      setlsbs(&needToModify,getbit(b0,bitItter));
    
      //then setting this byte into the image:
      SetGray(bitItter,needToModify);
    }

  for(bitItter+1;bitItter<16;bitItter++)
    {
      byte needToModify=GetGray(bitItter);
     
      //changing this byte
      setlsbs(&needToModify,getbit(b1,bitItter-8));
       
      //then setting this byte into the image:
      SetGray(bitItter,needToModify);
    }
 
 

  // embed 4 file extension characters (to make it easy for the extraction)
  // 
  //I need bitItter to go from 16 to 48
  int extIndex=0;

  
  //droppping the . in the file extension and adding a null terminator
  char subs[4];
  s=memcpy(subs,&s[1],3);
  s[3]='\0';

 


  for(bitItter+1;bitItter<48;bitItter)
    {
      
      int counter=0;
     
      while(counter<8)
	{
	  byte needToModify=GetGray(bitItter);
	
	  byte extByte=s[extIndex] | 0x00;
	 
	
	  setlsbs(&needToModify,getbit(extByte,counter));
	 
	  SetGray(bitItter,needToModify);
	  bitItter++;
	  counter++;
	}
      extIndex++;
    }



  
  //time to add data!


  for (i=0; i<b.size; i++)
    {
      // here you embed information into the image one byte at the time
      // note that you should change only the least significant bits of the image
      int counter=0;
      while(counter<8)
	{
	  byte needToModify=GetGray(bitItter);
	  setlsbs(&needToModify,getbit(b.data[i],counter));
	  SetGray(bitItter,needToModify);
	  bitItter++;
	  counter++;
		  
	}
    }

  WriteImage(argv[2],img);  // output stego file (cover_file + file_to_hide)
}


