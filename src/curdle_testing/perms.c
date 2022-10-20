#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>

int getuids(uid_t *uids) {
  if(getresuid(&uids[0], &uids[1], &uids[2]) == -1) {
    perror("Unable to get UIDs\n");
    return 0;
  }
  return 1;
}

int getgids(gid_t *gids) {
  if(getresgid(&gids[0], &gids[1], &gids[2]) == -1) {
    perror("Unable to get GIDs\n");
    return 0;
  }
  return 1;
}

int main(void) {
  uid_t uids[3];
  gid_t gids[3];

  if(getuids(uids) && getgids(gids)) {
    printf("Real ID: user %d, group %d\n", (int)uids[0], (int)gids[0]);
    printf("Effective ID: user %d, group %d\n", (int)uids[1], (int)gids[1]);
    printf("Set-ID: user %d, group %d\n", (int)uids[2], (int)gids[2]);

    seteuid(uids[0]);
    getuids(uids);
    getgids(gids);
    printf("Effective ID: user %d, group %d\n", (int)uids[1], (int)gids[1]);
  }

  return 0;
}