//
//  Cisco code running config parsing.   Generic functions (high level)
//


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


//----------------------------------------------------------------------
//----------------------------------------------------------------------


int ciscoInit(ciscoData *d)
{
memset(d, 0 , sizeof(ciscoData));
return(1);
}
                          
//----------------------------------------------------------------------
                          
int HostnameParse(ciscoData *d, char *line) 
{
 
if (_verbose > 2)
	printf("\n HostnameParse: line: %s", line );   

if (strncmp(line, "hostname ", strlen("hostname ")) == 0)
	processLine(line, d->hostname, "hostname ");

if (_verbose > 1)
	printf("\n Hostname: |%s|", d->hostname );   

return(1);
}    
                                                    
//----------------------------------------------------------------------

int processFile()
{
FILE 		*f=NULL;
char 		dline[3000];	
ciscoData 	d;
int 		(*parseFunction)(ciscoData *, char *) = NULL;


ciscoInit(&d);

// parse filename to get hostname (not safe but better than nothing!)
getHostNameFromFileName();


if ( (f = fopen(_fname, "r")) != NULL )
  {
  while (fgets(dline, 2998, f))
	{
	if (_verbose > 3)
	  printf("\n line--> |%s|", dline);
    
    if (dline[0] != ' ')
   		parseFunction = NULL;

	if (strncmp(dline, "hostname ", strlen("hostname ")) == 0)
		parseFunction = &HostnameParse;
	else if (strncmp(dline, "interface ", strlen("interface ")) == 0)
		parseFunction = &INTParse;
/*	else if (strncmp(dline, "event manager applet ", strlen("event manager applet ")) == 0)
		parseFunction = &parseEventManager;
	else if (strncmp(dline, "ip sla ", strlen("ip sla ")) == 0)
		parseFunction = &parseIPSLA;
	else if (strncmp(dline, "policy-map ", strlen("policy-map ")) == 0)
		parseFunction = &parsePolicyMap;
	else if (strncmp(dline, "class-map ", strlen("class-map ")) == 0)
		parseFunction = &parseClassMap;
	else if (strncmp(dline, "track ", strlen("track ")) == 0)
		parseFunction = &parseTrack; */
	else if (strncmp(dline, "ip vrf ", strlen("ip vrf ")) == 0)
		parseFunction = &VRFParse;

	// real line parsing
	if (parseFunction)
		parseFunction(&d, dline);

    }
  fclose(f);
  }  // fopen
  
  
VRFPrint(&d);
INTPrint(&d);
  
return(1);

}
             
//----------------------------------------------------------------------
//----------------------------------------------------------------------
