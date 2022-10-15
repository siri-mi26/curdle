#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// for off_t
#include <sys/types.h>
#include "curdle.h"
#include "adjust_score.h"

#define _GNU_SOURCE
static uid_t ruid, euid, rgid, egid;

/* Make effecive user ID = the real user ID */
void drop_privs()
{
  return;
  
  int status1;
  int status2;
  // Set effectives to real
  // status1 = seteuid(ruid);
  // status2 = setegid(rgid);
  printf("ruid, euid: %i %i\n", ruid, euid);

  if (status1 < 0)
  {
    fprintf(stderr, "Couldn't set uid.\n");
    exit(status1);
  }
  if (status2 < 0)
  {
    fprintf(stderr, "Couldn't set group uid.\n");
    exit(status2);
  }
  return;
}
/* Make real ID = effective ID
This will increase the privileges of the calling process */
void get_privs()
{

  return;
  int status1;
  int status2;
  // status1 = setuid(euid);
  // status2 = setgid(egid);
  printf("ruid, euid: %i %i\n", ruid, euid);

  if (status1 < 0)
  {
    fprintf(stderr, "Couldn't set uid.\n");
    exit(status1);
  }
  if (status2 < 0)
  {
    fprintf(stderr, "Couldn't set group uid.\n");
    exit(status2);
  }
  return;
}


int main()
{
  
  char *message;
  ruid = getuid();
  euid = geteuid();
  rgid = getgid();
  egid = getegid();

  printf("ruid, euid, rgid, egid %d %d %d %d\n", ruid, euid, rgid, egid);
  

  drop_privs();
  message = "Error message, check main()";
  
  adjust_score(1001, "goni", 10, &message); // in here, privileges are dropped

  return 0;
}