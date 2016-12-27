//
//  Cisco code for 'VRF' related lines parsing 
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


int SLAInit(SLADATA *d)
{
memset(d, 0 , sizeof(SLADATA));
return(1);
}

//----------------------------------------------------------------------

int SLAAdd(ciscoData *c, SLADATA *v)
{

if ( (c->slas = (SLADATA *) realloc(c->slas, (c->slaQtty+1) * sizeof(SLADATA))) == NULL )
    {
    printf("\n\n SLAAdd: ALLOCATION ERROR!!!");
    exit(0);
    }

memcpy( &(c->slas[c->slaQtty]), v, sizeof(SLADATA));
c->slaQtty++;	

return(1);
}
//----------------------------------------------------------------------

int SLAParse(ciscoData *d, char *line) 
{

if (_verbose > 2)
	printf("\n SLAParse: line: %s", line );

// omit lines not related!    
if (strstr(line, "ip sla logging"))
	return(1);
if (strstr(line, "ip sla group"))
	return(1);
    
if (strncmp(line, "ip sla ", strlen("ip sla ")) == 0)
	{
	SLADATA s;
	char aux[NAME_LENGTH];

	SLAInit(&s);
    processLine(line, aux, "ip sla ");
    strcpy(s.name, aux);
	SLAAdd(d, &s);
	}
else if (strncmp(line, " vrf ", strlen(" vrf ")) == 0)
	{
	char aux[NAME_LENGTH];
	processLine(line, aux, " vrf ");
	if (d->slaQtty > 0)
		strcpy(d->slas[d->slaQtty-1].vrf, aux);
	}	
    
return(1);
}                                                                                          
//----------------------------------------------------------------------

int SLAPrint(ciscoData *d)
{
int i=0;
for (i=0 ; i<d->slaQtty ; i++)
	printf("\n SLA |%-25s| VRF |%-12s| ", d->slas[i].name,  d->slas[i].vrf);
	
return(1);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
