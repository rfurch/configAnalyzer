

#define	IP_LENGTH		50
#define	NAME_LENGTH		400


typedef struct intIP
  {
  char 		ip[IP_LENGTH];
  char 		mask[IP_LENGTH];
  short		secondary;

  }intIP;



typedef struct IntDATA
  {
  char 			name[NAME_LENGTH];
  char 			description[NAME_LENGTH];
  char 			vrf[NAME_LENGTH];  	

  short 		switchport;				// interface switchport mode
  int 			shutdown;				// interface is in shutdown state (ADMIN)	
  int 			ip_qtty;	
  intIP			*ip_data;
  }IntDATA;
   
    
 typedef struct VRFDATA
  {
  char 			name[NAME_LENGTH];
  char 			description[NAME_LENGTH];
  char 			rd[NAME_LENGTH];  	
  }VRFDATA;
   
    
typedef struct ciscoData
  {
  char			hostname[NAME_LENGTH];
  
  VRFDATA		*vrfs;			// array of VRFs
  int			vrfQtty;		// number of VRFs
  
  IntDATA		*interfaces;	// idem for interfaces
  int 			intQtty;
  }ciscoData;


extern  int     _verbose;
extern char 	_fname[];



// util.c
int processLine(char *org, char *dst, char *pattern);
int getHostNameFromFileName();


// parseCisco.c
int ciscoInit(ciscoData *d);
int processFile();



// db.c
int interfaceToDB(IntDATA *d, char *hostname);

// parseVRF.c
int VRFParse(ciscoData *d, char *line);
int VRFInit(VRFDATA *d);
int VRFAdd(ciscoData *c, VRFDATA *v);
int VRFPrint(ciscoData *d);

// parseINT.c
int INTInit(IntDATA *d);
int INTAdd(ciscoData *c, IntDATA *v);
int INTAddIP(IntDATA *d, char *ip, char *mask, int secondary);
int INTPrint(ciscoData *c);
int INTFree(IntDATA *d);
int INTProcessIP(IntDATA *d, char *dline);
int INTParse(ciscoData *d, char *line);


