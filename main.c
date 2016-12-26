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

int parseEventManager(ciscoData *d, char *line) 
{
    
if (_verbose > 1)
		printf("\n parseEventManager: line: %s", line );
return(1);
}    

//----------------------------------------------------------------------

int parseIPSLA(ciscoData *d, char *line) 
{
    
if (_verbose > 1)
		printf("\n parseIPSLA: line: %s", line );
return(1);
}    

//----------------------------------------------------------------------

int parsePolicyMap(ciscoData *d, char *line) 
{
    
if (_verbose > 1)
		printf("\n parsePolicyMap: line: %s", line );
return(1);
}    
      
//----------------------------------------------------------------------

int parseClassMap(ciscoData *d, char *line) 
{
    
if (_verbose > 1)
		printf("\n parseClassMap: line: %s", line );
return(1);
}    

//----------------------------------------------------------------------

int parseTrack(ciscoData *d, char *line) 
{
    
if (_verbose > 1)
		printf("\n parseTrack: line: %s", line );
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
