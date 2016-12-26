#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include "cfgana.h"


extern int  		_verbose;


//----------------------------------------------------------------------
//----------------------------------------------------------------------
int processLine(char *org, char *dst, char *pattern)
{
int 	i=0;
char 	*p=NULL;

for (i=0 ; i<strlen(org) ; i++)
  {	
  if (org[i] == 0x0D || org[i] == 0x0A)
	{
	org[i] = 0;
	break;
	}
  }
  
if ( (p = strstr(org, pattern)) != NULL )
  {
  strcpy(dst, p + strlen(pattern));
  return(1);
  }
      		
return(0);
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------

int getHostNameFromFileName()
{
char 		devicename[400];
int 		i=0;

strcpy(devicename, _fname);
// get hostname from filename, for DB matching conditions
for (i=strlen(devicename)-1 ; i>0 ; i--)	// remove "extension"  (characters after dot)
  if (devicename[i] == '.')
    {
	devicename[i] = 0;
	break;
	}		

for (i=strlen(devicename)-1 ; i>0 ; i--) // remove path
  if (devicename[i] == '/')
    {
	memmove(devicename, &devicename[i+1], strlen(devicename) - i + 1);
	devicename[strlen(devicename) - i + 1] = 0;	
	break;
	}		

if (_verbose > 2)
	printf("\n Device NAME: |%s| ", devicename);
               
return(1);
}
  


//----------------------------------------------------------------------
//----------------------------------------------------------------------
