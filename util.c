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


int interfaceInit(intDATA *d)
{
memset(d, 0 , sizeof(intDATA));
return(1);
}

//----------------------------------------------------------------------

int interfaceAddIP(intDATA *d, char *ip, char *mask, int secondary)
{

if ( (d->ip_data = (intIP *) realloc(d->ip_data, (d->ip_qtty+1) * sizeof(intIP))) == NULL )
    {
    printf("\n\n addIpToInterface: ALLOCATION ERROR!!!");
    exit(0);
    }

strcpy(d->ip_data[d->ip_qtty].ip, ip);
strcpy(d->ip_data[d->ip_qtty].mask, mask);
d->ip_data[d->ip_qtty].secondary = secondary;
d->ip_qtty++;	

return(1);
}

//----------------------------------------------------------------------

int interfacePrint(intDATA *d)
{
int i=0;

printf("\nInterface: |%s| vrf: |%s| desc: |%s|", d->name, d->vrf, d->description);
for (i=0 ; i<d->ip_qtty ; i++)
	printf("\n   IP: |%s/%s| (secondary: %i)", d->ip_data[i].ip, d->ip_data[i].mask, d->ip_data[i].secondary);

printf("\n");

return(1);
}

//----------------------------------------------------------------------

int interfaceFree(intDATA *d)
{
if (d->ip_data)
	free(d->ip_data);
memset(d, 0 , sizeof(intDATA));
	
return(1);
}

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
