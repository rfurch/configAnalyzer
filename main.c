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

#define		CISCO_SEARCH_FOR_HOSTNAME			50

#define		CISCO_SEARCH_FOR_INTERFACE			100
#define		CISCO_SEARCH_FOR_INTERFACE_IP		101

#define		CISCO_SEARCH_FOR_END_OF_INTERFACE	500
#define		CISCO_SEARCH_FOR_END_OF_FILE		900

int  		_verbose=0;
char 		_fname[2000];

//----------------------------------------------------------------------
//----------------------------------------------------------------------

void PrintUsage(char *prgname)
{
printf ("============================================================\n");
printf ("Config  analyzer\n\n");
printf ("USAGE: %s [options]\n", prgname);
printf ("Options:\n");
printf ("  -v 	Verbose mode\n");
printf ("  -f 	filename \n");
printf ("============================================================\n\n");
fflush(stdout);
}


//----------------------------------------------------------------------

int processIP(intDATA *d, char *dline)
{
int 		i=0, j=0;
int 		secondary=0;
char 		ip[100], mask[100];

//printf("|%s|", dline);

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

if (interfaceAddIP(d, ip, mask, secondary))
	return(1);  
return(0);	
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

int processFile()
{
FILE 		*f=NULL;
char 		dline[3000];	
char 		auxline[3000];	

char 		hostname[400];
char 		devicename[400];
intDATA	    int01;
int 		status=CISCO_SEARCH_FOR_END_OF_FILE;

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
	memmove(devicename, &devicename[i+1], strlen(devicename) - i);
	devicename[strlen(devicename) - i] = 0;	
	break;
	}		

if (_verbose > 2)
	printf("\n Device NAME: |%s| ", devicename);

if ( (f = fopen(_fname, "r")) != NULL )
  {
  while (fgets(dline, 2998, f))
	{
	if (_verbose > 3)
	  printf("\n line-- |%s|", dline);

	switch(status)
	  {
	  case CISCO_SEARCH_FOR_END_OF_FILE:
		{
		if (strncmp(dline, "hostname", strlen("hostname")) == 0)
		  processLine(dline, hostname, "hostname ");
		else if (strncmp(dline, "interface", strlen("interface")) == 0)
		  {
		  interfaceInit(&int01);
		  processLine(dline, int01.name, "interface ");
		  status = CISCO_SEARCH_FOR_END_OF_INTERFACE;
		  }	  	  	  
		}
	  break;

	  case CISCO_SEARCH_FOR_END_OF_INTERFACE:
		  {
		  if (strlen(dline) < 5)			// end of interface block
			{
			if (!int01.shutdown && strlen(int01.vrf) < 2)  // global interfaces, no shutdown
				{
				interfacePrint(&int01);
				interfaceToDB(&int01, devicename);
				}
			interfaceFree(&int01);

			status=CISCO_SEARCH_FOR_END_OF_FILE;
			}
		  else if (strncmp(dline, " ip vrf", strlen(" ip vrf")) == 0)
  			processLine(dline, int01.vrf, " ip vrf forwarding ");
		  else if (strncmp(dline, " ip address", strlen(" ip address")) == 0)
			{
			auxline[0]=0;
  			processLine(dline, auxline, " ip address ");
			processIP(&int01, auxline);
  			}
		  else if (strncmp(dline, " description", strlen(" description")) == 0)
  			processLine(dline, int01.description, " description ");
		  else if (strncmp(dline, " shutdo", strlen(" shutdo")) == 0)
  			int01.shutdown=1;
		  }
	  break;

	  }
    }
  fclose(f);
  }  // fopen
  
return(1);

}

                                                                                 
//----------------------------------------------------------------------

int parseArgs(int argc, char *argv[])

{

int 			c=0; 

while ( ( c= getopt(argc,argv,"p:t:i:I:f:n:s:vp:")) != -1 )
  {
  switch(c)
	{
	case 'v': 
	  _verbose++; 
	break;
  
	case 't':
	break;

	case 'p':
	break;

	case 'i':
	break;

	case 's':
	break;

    case 'I':
	break;

	case 'f':
	  if (optarg)
		strcpy(_fname, optarg);
	break;

	case 'n':
	break;

	case '?': 
	  PrintUsage(argv[0]); 
	  exit(0); 
	break;
	}
  }


if (_verbose > 0)
    {
    printf ("* Verbose mode     (-v): %i \n", _verbose);
    printf ("* Compiled: %s / %s \n", __DATE__, __TIME__);
    }

return(1);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

int main(int argc,char *argv[])
{

if ( ! parseArgs (argc, argv) )
  {
  PrintUsage(argv[0]); 
  exit(0); 
  }
	
processFile();

printf("\n\n");
return(0);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
