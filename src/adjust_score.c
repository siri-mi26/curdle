// TODO: run in gitpod
// TODO: change pathname to curdle

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
#include <limits.h>
#include "curdle.h"

// #include "adjust_score.h"

/** Size of a field (name or score) in a line of the `scores`
 * file.
 */
const size_t FIELD_SIZE = FIELD_SIZE_;

/** Size of a line in the `scores` file.
 */
const size_t REC_SIZE = REC_SIZE_;

/* Set effective user ID to the real user ID provided, dropping privileges. */
int drop_privs(uid_t uid, char **message)
{
  int status1;
  int status2;

  status2 = setegid(getgid());
  status1 = seteuid(uid);

  printf("ruid, euid, rgid, egid %d %d %d %d\n", getuid(), geteuid(), getgid(), getegid());

  if (status1 < 0)
  {
    char *error_message = malloc(250 * sizeof(char)); // 250 character string perhaps
    error_message = "Couldn't set user ID.";
    *message = error_message;
    exit(EXIT_FAILURE);
  }
  if (status2 < 0)
  {
    char *error_message = malloc(250 * sizeof(char));
    error_message = "Couldn't set group ID.";
    *message = error_message;
    exit(EXIT_FAILURE);
  }
  return 1;
}

/* Set the real user ID to the effective, gaining privileges. */
int get_privs(char **message)
{
  int status1;
  int status2;

  status2 = setgid(getgid());
  status1 = setuid(getuid());

  if (status1 < 0)
  {
    char *error_message = malloc(250 * sizeof(char));
    error_message = "Couldn't set user ID.";
    *message = error_message;
    exit(EXIT_FAILURE);
  }
  if (status2 < 0)
  {
    char *error_message = malloc(250 * sizeof(char));
    error_message = "Couldn't set group ID.";
    *message = error_message;
    exit(EXIT_FAILURE);
  }
  return 1;
}

int adjust_score(uid_t uid, const char *player_name, int score_to_add, char **message)
{
  FILE *fp;
  // const char *filename = "/home/siri/cits3007/curdle-skeleton-code/curdle/tests/test-files/good/file1";
  // const char *filename = "boo.txt";
  const char *filename = "/var/lib/curdle/scores";

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

  printf("tmp %d\n", uid);

  long final_pos = (long)malloc(sizeof(long));

  // Setting variables to all \0 so they are 10 in length and padded
  memset(new_name, 0, FIELD_SIZE);
  memset(newline, 0, REC_SIZE);
  memset(new_score, 0, FIELD_SIZE);
  memset(strscore, 0, FIELD_SIZE);
  strncpy(new_name, player_name, FIELD_SIZE);

  printf("tmp %d\n", INT_MAX);
  printf("tmp %d\n", INT_MIN);

  // get_privs(message);
  fp = fopen(filename, "r+");
  // drop_privs(ruid, message);

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
    char *error_message = malloc(250 * sizeof(char));
    error_message = "File unable to be opened or doesn't exist.";
    *message = error_message;
    exit(EXIT_FAILURE);
  }

  // Reading every line and parsing it
  while (fgets(line, REC_SIZE + 1, fp) != NULL)
  {
    if (ftell(fp) < 21)
    {
      char *error_message = malloc(250 * sizeof(char));
      error_message = "File invalid. Line too short.";
      *message = error_message;
      exit(EXIT_FAILURE);
    }

    if (line[20] != '\n')
    {
      char *error_message = malloc(250 * sizeof(char));
      error_message = "File invalid. Line too long.";
      *message = error_message;
      exit(EXIT_FAILURE);
    }

    if (line[0] == '\n')
    {

      char *error_message = malloc(250 * sizeof(char));
      error_message = "File invalid. Blank lines in between.";
      *message = error_message;
      exit(EXIT_FAILURE);
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

        int_line_score += score_to_add;

        sprintf(strscore, "%i", int_line_score);
        memcpy(new_score, strscore, FIELD_SIZE + 1);

        // Copy both fields into new line and add \n at end
        strncpy(newline, new_name, strlen(new_name));
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
        return 1;
      }
    }

    count++;
    bytes = 21 * count;
  }
  fclose(fp);
  return 1;
}
