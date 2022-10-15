// TODO: run in gitpod
// TODO: change pathname to curdle

/** \file adjust_score.c
 * \brief Functions for safely amending a player's score in the
 * `/var/lib/curdle/scores` file.
 *
 * Contains the adjust_score_file function, plus supporting data
 * structures and functions used by that function.
 *
 * ### Known bugs
 * \bug privileges are not used when opening file
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
  const char *filename = "/home/siri/cits3007/curdle-skeleton-code/curdle/tests/test-files/good/file1";
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

  // Setting variables to all \0 so they are 10 in length and padded
  memset(new_name, 0, FIELD_SIZE);
  memset(newline, 0, REC_SIZE);
  memset(new_score, 0, FIELD_SIZE);
  strncpy(new_name, player_name, FIELD_SIZE);

  // parse name
  get_privs();
  fp = fopen(filename, "r+");
  drop_privs();

  // Initialise end of file to find position where new player is added
  fseek(fp, 0, SEEK_END);
  final_pos = ftell(fp);
  // printf("final_pos: %li\n", final_pos);

  // Reset file pointer to start again
  fseek(fp, 0, SEEK_SET);

  if (fp == NULL)
  {
    printf("exit\n");
    printf("Error message: %s", *message);
    exit(EXIT_FAILURE);
  }
  // Reading every line and parsing it
  while (fgets(line, REC_SIZE + 1, fp) != NULL)
  {

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

        // copy both fields into new line and add \n at end
        strncpy(newline, new_name, strlen(new_name)); // TODO:
        sprintf(&newline[10], "%s", strscore);
        sprintf(&newline[20], "%s", "\n");
        // for (size_t i = 0; i < REC_SIZE; i++)
        // {
        //   printf("newline after adding strscore[i] %c\n", newline[i]);
        // }
        fseek(fp, bytes, SEEK_SET);

        for (size_t i = 0; i < REC_SIZE; i++)
        {
          fputc(newline[i], fp);
        }
        fclose(fp);
        return 0;
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
          // printf("newline after adding strscore[i] %c\n", newline[i]);
          fputc(newline[i], fp);
        }

        fclose(fp);
        return 0;
      }
    }
    count++;
    bytes = 21 * count;
  }
  fclose(fp);
  return 0;
}
