// TODO: run in gitpod
// TODO: change pathname to curdle
// TODO: check that u dont need gnu source

/**
 * Functions for safely amending a player's score in the
 * `/var/lib/curdle/scores` file.
 *
 * Contains the adjust_score_file function, plus supporting data
 * structures and functions used by that function.
 *
 * ### Known bugs
 * \bug No known ones
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include "curdle.h"
#include "adjust_score.h"

/** Size of a field (name or score) in a line of the `scores`
 * file.
 */
const size_t FIELD_SIZE = FIELD_SIZE_;

/** Size of a line in the `scores` file.
 */
const size_t REC_SIZE = REC_SIZE_;

int adjust_score(uid_t uid, const char *player_name, int score_to_add, char **message)
{
  FILE *fp;
  // const char *filename = "/home/siri/cits3007/curdle-skeleton-code/curdle/tests/test-files/good/file1";
  const char *filename = "b2";
  // const char *filename = "/var/lib/curdle/scores";

  char line[REC_SIZE];
  char line_player[FIELD_SIZE];
  char line_score[FIELD_SIZE];

  char newline[REC_SIZE];
  char new_name[FIELD_SIZE];
  char strscore[FIELD_SIZE];
  char new_score[FIELD_SIZE];

  int bytes = 0;
  int count = 0;
  int int_line_score = 0;

  long final_pos = (long)malloc(sizeof(long));

  // Variables to determine status of dropping privileges or gaining privileges
  // int status1;
  // int status2;

  // Setting variables to all \0 so they are 10 in length and padded
  memset(new_name, 0, FIELD_SIZE);
  memset(newline, 0, REC_SIZE);
  memset(new_score, 0, FIELD_SIZE);
  strncpy(new_name, player_name, FIELD_SIZE);

  // parse name

  // get_privs();
  // uid_t ruid, euid, rgid, egid;
  // ruid = getuid();
  // euid = geteuid();
  // rgid = getgid();
  // egid = getegid();
  // printf("ruid, euid, rgid, egid %d %d %d %d\n", ruid, euid, rgid, egid);

  // printf("ruid, euid: %i %i\n", getuid(), getegid());

  /* setuid(geteuid());
  setgid(getegid());
  if (status1 < 0)
  {
    fprintf(stderr, "Couldn't set real user/group ID to effective user/group ID. Privileges were not increased enough.\n");
    exit(status1);
  } */
  fp = fopen(filename, "r+");
  // drop_privs(uid, 1002); // TODO:
  /* setuid(getuid());
  setgid(getgid());
  if (status2 < 0)
  {
    fprintf(stderr, "Couldn't set effective user ID/group ID back to real user/group ID. Privileges not dropped.\n");
    exit(status2);
  }
 */
  // Initialise end of file to find position where new player is added
  fseek(fp, 0, SEEK_END);
  final_pos = ftell(fp);

  // File is empty
  if (ftell(fp) == 0)
  {
    sprintf(strscore, "%i", score_to_add);
    strncpy(new_score, strscore, FIELD_SIZE);

    // Copy both fields into new line and add \n at end
    strncpy(newline, new_name, strlen(new_name));
    sprintf(&newline[10], "%s", strscore);
    sprintf(&newline[20], "%s", "\n");

    fseek(fp, -0, SEEK_END);
    for (size_t i = 0; i < REC_SIZE; i++)
    {
      fputc(newline[i], fp);
    }
    fclose(fp);
    return 0;
  }

  // Reset file pointer to start again
  fseek(fp, 0, SEEK_SET);

  if (fp == NULL)
  {
    printf("%s\n", *message);
    exit(EXIT_FAILURE);
  }

  // Reading every line and parsing it
  while (fgets(line, REC_SIZE + 1, fp) != NULL)
  {

    if (line[20] != '\n')
    {
      printf("here\n");
      // printf("%s\n", *message);
      // exit(EXIT_FAILURE);
    }
    if (line[0] != '\n')
    {
      // Parses the line into the player name and score
      strncpy(line_player, line, FIELD_SIZE);
      strncpy(line_score, &line[10], FIELD_SIZE);

      // If player exists, modifies score
      if (strncmp(player_name, line_player, FIELD_SIZE) == 0) // false
      {
        int_line_score = atoi(line_score);
        printf("bf new_score %s\n", line_score);
        int_line_score += score_to_add;
        sprintf(strscore, "%i", int_line_score); // TODO:
        strncpy(new_score, strscore, FIELD_SIZE);

        printf("af new_score %s\n", new_score);
        printf("tmp %li\n", sizeof(strscore));
        printf("tmp %li\n", strlen(strscore));

        // Copy both fields into new line and add \n at end
        strncpy(newline, new_name, strlen(new_name)); // TODO:
        sprintf(&newline[10], "%s", strscore);
        sprintf(&newline[20], "%s", "\n");

        fseek(fp, bytes, SEEK_SET);

        for (size_t i = 0; i < REC_SIZE; i++)
        {
          fputc(newline[i], fp);
        }
        fclose(fp);
        return 1;
      }

      // If player doesn't exist, put player name and score into new line and add to file
      if (ftell(fp) == final_pos)
      {
        if (ftell(fp) == 20)
        {
          fputc('\n', fp);
        }
        sprintf(strscore, "%i", score_to_add);
        strncpy(new_score, strscore, FIELD_SIZE);

        // Copy both fields into new line and add \n at end
        strncpy(newline, new_name, strlen(new_name));
        sprintf(&newline[10], "%s", strscore);
        sprintf(&newline[20], "%s", "\n");
        printf("newline %s", newline);

        fseek(fp, -0, SEEK_END);
        for (size_t i = 0; i < REC_SIZE; i++)
        {
          fputc(newline[i], fp);
        }

        fclose(fp);
        return 1;
      }
    }

    count++;
    bytes = 21 * count;
  }
  fclose(fp);
  return 1;
}
