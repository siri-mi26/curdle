#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

// for off_t

#include "curdle.h"
#include "adjust_score.h"
uid_t ruid, euid, rgid, egid;

// /* Make effective user ID = the real user ID provided */
// void drop_privs(uid_t uid, uid_t gid)
// {
//   int status1;
//   int status2;
//   // Set effective IDs to real
//   status2 = setegid(gid);
//   status1 = seteuid(uid);

//   printf("ruid, euid: %i %i\n", ruid, euid);

//   if (setegid(1001) < 0)
//   {
//     perror("setgid");
//     fprintf(stderr, "Couldn't set group uid.\n");
//   }
//   printf("ruid, euid, rgid, egid %d %d %d %d\n", getuid(), geteuid(), getgid(), getegid());

//   if (status1 < 0)
//   {
//     // fprintf(stderr, "Couldn't set uid.\n");
//     perror("setuid");
//     exit(status1);
//   }
//   if (status2 < 0)
//   {
//     fprintf(stderr, "Couldn't set group uid.\n");
//     exit(status2);
//   }
//   return;
// }
/* Make real ID = effective ID
This will increase the privileges of the calling process */
// void get_privs()
// {

//   return;
//   int status1;
//   int status2;
  
//   status2 = setgid(getgid());
//   status1 = setuid(getuid());
  

//   if (status1 < 0)
//   {
//     fprintf(stderr, "Couldn't set uid.\n");

//     exit(status1);
//   }
//   if (status2 < 0)
//   {
//     fprintf(stderr, "Couldn't set group uid.\n");
//     exit(status2);
//   }
//   return;
// }

int main()
{

  char *message;

  // printf("ruid, euid, rgid, egid %d %d %d %d\n", getuid(), geteuid(), getgid(), getegid());

  // drop_privs( 1001,  1001);

  // message = "Error message, check main()";

  adjust_score(1001, "alice", 9999999999, &message);
  printf("tmp %s\n", message);
  // get_privs(1000,1000);

  return 0;
}