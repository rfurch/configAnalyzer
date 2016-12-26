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


int VRFInit(VRFDATA *d)
{
memset(d, 0 , sizeof(VRFDATA));
return(1);
}

//----------------------------------------------------------------------

int VRFAdd(ciscoData *c, VRFDATA *v)
{

if ( (c->vrfs = (VRFDATA *) realloc(c->vrfs, (c->vrfQtty+1) * sizeof(VRFDATA))) == NULL )
    {
    printf("\n\n VRFAdd: ALLOCATION ERROR!!!");
    exit(0);
    }

memcpy( &(c->vrfs[c->vrfQtty]), v, sizeof(VRFDATA));
c->vrfQtty++;	

return(1);
}
//----------------------------------------------------------------------

int parseVRF(ciscoData *d, char *line) 
{

if (_verbose > 1)
	printf("\n parseVRF: line: %s", line );
    
if (strncmp(line, "ip vrf ", strlen("ip vrf ")) == 0)
	{
	VRFDATA v;
	char aux[NAME_LENGTH];

	VRFInit(&v);
    processLine(line, aux, "ip vrf ");
    strcpy(v.name, aux);
	VRFAdd(d, &v);
	}
else if (strncmp(line, " description ", strlen(" description ")) == 0)
	{
	char aux[NAME_LENGTH];
	processLine(line, aux, " description ");
	if (d->vrfQtty > 0)
		strcpy(d->vrfs[d->vrfQtty-1].description, aux);
	}	
else if (strncmp(line, " rd ", strlen(" rd ")) == 0)
	{
	char aux[NAME_LENGTH];
	processLine(line, aux, " rd ");
	if (d->vrfQtty > 0)
		strcpy(d->vrfs[d->vrfQtty-1].rd, aux);
	}	
	
    
return(1);
}                                                                                          
//----------------------------------------------------------------------

int VRFPrint(ciscoData *d)
{
int i=0;
for (i=0 ; i<d->vrfQtty ; i++)
	printf("\n VRF |%-25s| RD |%-12s| DESCR |%s|", d->vrfs[i].name,  d->vrfs[i].rd,  d->vrfs[i].description);
	
return(1);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
