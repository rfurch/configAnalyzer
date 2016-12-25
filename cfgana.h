

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
   
    
    




// util.c
int interfaceInit(intDATA *d);
int interfaceAddIP(intDATA *d, char *ip, char *mask, int secondary);
int interfacePrint(intDATA *d);
int interfaceFree(intDATA *d);


// db.c
int interfaceToDB(intDATA *d, char *hostname);
