//
//  Cisco code for 'interface' related lines parsing 
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


extern int  		_verbose;


//----------------------------------------------------------------------
//----------------------------------------------------------------------

int INTInit(IntDATA *d)
{
memset(d, 0 , sizeof(IntDATA));
return(1);
}

//----------------------------------------------------------------------

int INTAdd(ciscoData *c, IntDATA *v)
{

if ( (c->interfaces = (IntDATA *) realloc(c->interfaces, (c->intQtty+1) * sizeof(IntDATA))) == NULL )
    {
    printf("\n\n INTAdd: ALLOCATION ERROR!!!");
    exit(0);
    }

memcpy( &(c->interfaces[c->intQtty]), v, sizeof(IntDATA));
c->intQtty++;	

return(1);
}

//----------------------------------------------------------------------

int INTAddIP(IntDATA *d, char *ip, char *mask, int secondary)
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

int INTPrint(ciscoData *d)
{
int i=0, j=0;

for (i=0 ; i<d->intQtty ; i++)
	{
	printf("\n\n INT |%-25s| VRF |%-12s| SHUT: %i  SWITCHPORT: %i MPLS: %i MTU: %i DESCR |%s|", d->interfaces[i].name,  d->interfaces[i].vrf, d->interfaces[i].shutdown, d->interfaces[i].switchport, d->interfaces[i].mpls, d->interfaces[i].mtu, d->interfaces[i].description);
		
	for (j=0 ; j<d->interfaces[i].ip_qtty ; j++)
		printf("\n     IP: |%-16s| MASK: |%-16s|  %s", d->interfaces[i].ip_data[j].ip, d->interfaces[i].ip_data[j].mask, (d->interfaces[i].ip_data[j].secondary > 0) ? "Secondary" : " ");
	}
return(1);
}

//----------------------------------------------------------------------

int INTFree(IntDATA *d)
{
if (d->ip_data)
	free(d->ip_data);
memset(d, 0 , sizeof(IntDATA));
	
return(1);
}


//----------------------------------------------------------------------

int INTProcessIP(IntDATA *d, char *dline)
{
int 		i=0, j=0;
int 		secondary=0;
char 		ip[100], mask[100];

if (strcasestr(dline, " secondary"))  // check if it is secondary
  secondary=1;

// copy IP
for (i=0 ; i<strlen(dline) && dline[i] != ' ' ; i++)
  ip[i] = dline[i];
ip[i] = 0;

// copy MASK
for ( j=0, ++i ; i<strlen(dline) && dline[i] != ' ' ; i++, j++)
  mask[j] = dline[i];
mask[j] = 0;

if (INTAddIP(d, ip, mask, secondary))
	return(1);  
return(0);	
}

//----------------------------------------------------------------------

int INTParse(ciscoData *d, char *line) 
{
 
if (_verbose > 2)
	printf("\n INTParse: line: %s", line ); 
	
	
if (strncmp(line, "interface ", strlen("interface ")) == 0)
	{
	IntDATA in;
	char aux[NAME_LENGTH];

	INTInit(&in);
    processLine(line, aux, "interface ");
    strcpy(in.name, aux);
	INTAdd(d, &in);
	}
else if (strncmp(line, " description ", strlen(" description ")) == 0)
	{
	char aux[NAME_LENGTH];
	processLine(line, aux, " description ");
	if (d->intQtty > 0)
		strcpy(d->interfaces[d->intQtty-1].description, aux);
	}	
else if (strncmp(line, " ip vrf forwarding ", strlen(" ip vrf forwarding ")) == 0)
	{
	char aux[NAME_LENGTH];
	processLine(line, aux, " ip vrf forwarding ");
	if (d->intQtty > 0)
		strcpy(d->interfaces[d->intQtty-1].vrf, aux);
	}	
else if (strncmp(line, " mtu ", strlen(" mtu ")) == 0)
	{
	char aux[NAME_LENGTH];
	processLine(line, aux, " mtu ");
	if (d->intQtty > 0)
		d->interfaces[d->intQtty-1].mtu = atol(aux);
	}	
else if ( strstr(line, " switchport") && strlen(line) < 15 )
	{
	if (d->intQtty > 0)
		d->interfaces[d->intQtty-1].switchport = 1;
	}	
else if ( strstr(line, " mpls ip") )
	{
	if (d->intQtty > 0)
		d->interfaces[d->intQtty-1].mpls = 1;
	}	
else if (strncmp(line, " ip address ", strlen(" ip address ")) == 0)
	{
	char aux[NAME_LENGTH];
	processLine(line, aux, " ip address ");
	if (d->intQtty > 0)
		INTProcessIP(&(d->interfaces[d->intQtty-1]), aux);
	}	
	

	

return(1);
}    


//----------------------------------------------------------------------
//----------------------------------------------------------------------
