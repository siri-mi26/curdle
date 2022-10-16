// #define _GNU_SOURCE
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


int adjust_score(uid_t uid, const char *player_name, int score_to_add, char **message);
void drop_privs(uid_t uid, uid_t gid);
void get_privs();