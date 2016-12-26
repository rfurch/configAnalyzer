

#define	IP_LENGTH		50
#define	NAME_LENGTH		400


typedef struct intIP
  {
  char 		ip[IP_LENGTH];
  char 		mask[IP_LENGTH];
  short		secondary;
  }intIP;



typedef struct intDATA
  {
  char 			name[NAME_LENGTH];
  char 			description[NAME_LENGTH];
  char 			vrf[NAME_LENGTH];  	

  int 			shutdown;				// interface is in shutdown state (ADMIN)	
  int 			ip_qtty;	
  intIP			*ip_data;
  }intDATA;
   
    
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
  
  intDATA		*interfaces;	// idem for interfaces
  int 			intQtty;
  }ciscoData;


extern  int     _verbose;


// util.c
int interfaceInit(intDATA *d);
int interfaceAddIP(intDATA *d, char *ip, char *mask, int secondary);
int interfacePrint(intDATA *d);
int interfaceFree(intDATA *d);
int processLine(char *org, char *dst, char *pattern);

// parseCisco.c
int ciscoInit(ciscoData *d);


// db.c
int interfaceToDB(intDATA *d, char *hostname);

// parseVRF.c
int parseVRF(ciscoData *d, char *line);
int VRFInit(VRFDATA *d);
int VRFAdd(ciscoData *c, VRFDATA *v);
int VRFPrint(ciscoData *d);


