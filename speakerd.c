#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/io.h>

void speekertalk(unsigned int count)
{
  if (count>0 && count<32000)
  {
    outb_p(inb_p(0x61) | 3, 0x61);
    outb_p(0xB6, 0x43);
    outb_p(count & 0xff, 0x42);
    outb((count >> 8) & 0xff, 0x42);
  }
  else
    outb(inb_p(0x61) & 0xFC, 0x61);
}

int main(void)
{
  char ch;
  int i;
  int count=0;

  if (iopl(3)) 
  {
    fprintf(stderr,"You need to be root to run speakerd.\n");
    return EXIT_FAILURE;
  }
  while(fread((void*)&ch,sizeof(char),1,stdin))
  {
    if (ch>='0' && ch<='9')
    {
      count*=10;
      count+=(ch-'0');
    } 
    else
    {
      switch(ch)
      {
        case '.':
          speekertalk(0);
          exit(EXIT_SUCCESS);
          break;
        case '@':
          speekertalk(count);
          break;
        case '~':
          usleep(count);
        default:
          break;  
      }
      count=0;
    }
  }
  speekertalk(0);
  exit(EXIT_SUCCESS);
}
