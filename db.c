
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include <arpa/inet.h>

#include <mysql/mysql.h>

#include "cfgana.h"

char    *server = "127.0.0.1";
char    *user = "nocuser";
char    *password = "AguasaguaS";
char    *database = "topology";

extern  int     _verbose;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int interfaceToDB(intDATA *d, char *hostname)
{
MYSQL       *conn;
//MYSQL_RES   *res;
//MYSQL_ROW   row;
char        querystring[2000];
int 		i=0;

conn = mysql_init(NULL);

// Connect to database
if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
  {
  fprintf(stderr, "%s\n", mysql_error(conn));
  sleep(1);
  return(0);
  }
    
for (i=0 ; i<d->ip_qtty ; i++)
  {
  sprintf(querystring,"INSERT INTO topology.m2_admin_ips_interfaces (dev_id, interface, vrf, description, ip, mask, \
  secondary, verified)  SELECT id, '%s', '%s', '%s', '%s', '%s', %i, NOW() FROM devices b WHERE b.nombre LIKE '%s' \
  ON DUPLICATE KEY UPDATE description='%s', mask='%s', secondary=%i, verified=NOW();", \
  d->name, d->vrf, d->description, d->ip_data[i].ip, d->ip_data[i].mask, d->ip_data[i].secondary, hostname, 
  d->description, d->ip_data[i].mask, d->ip_data[i].secondary);
                        
  // send SQL query
  mysql_query(conn, querystring);
  }

if (_verbose > 3)
     printf("\n |%s| \n", querystring);

mysql_close(conn);
return(1);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
