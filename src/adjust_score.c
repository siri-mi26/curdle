// TODO: run in gitpod

/** \file adjust_score.c
 * \brief Functions for safely amending a player's score in the
 * `/var/lib/curdle/scores` file.
 *
 * Contains the \ref adjust_score_file function, plus supporting data
 * structures and functions used by that function.
 *
 * ### Known bugs
 *
 * \bug The \ref adjust_score_file function does not yet work.
 */

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

/** Size of a field (name or score) in a line of the `scores`
 * file.
 */
const size_t FIELD_SIZE = FIELD_SIZE_;

/** Size of a line in the `scores` file.
 */
const size_t REC_SIZE = REC_SIZE_;

// /** Initialize a \ref score_record struct.
//  *
//  * \param rec pointer to a \ref score_record struct to initialize.
//  * \param name player name to be inserted into `rec`.
//  * \param score player score to be inserted into `rec`.
//  */
// void score_record_init(struct score_record *rec, const char *name, int score)
// {
//   // this function is needed to initialize a score_record,
//   // because you can't *assign* to the name member -- it's an array.
//   // so we must copy the name in.
  // memset(rec->name, 0, FIELD_SIZE);
//   strncpy(rec->name, name, FIELD_SIZE);
//   rec->name[FIELD_SIZE - 1] = '\0';
//   rec->score = score;
// }

// /** Return the size of the open file with file descriptor `fd`.
//  * If the size cannot be determined, the function may abort program
//  * execution (optionally after printing an error message).
//  *
//  * `filename` is used for diagnostic purposes only, and may be `NULL`.
//  * If non-NULL, it represent the name of the file path from which
//  * `fd` was obtained.
//  *
//  * \param filename name of the file path from which `fd` was obtained.
//  * \param fd file descriptor for an open file.
//  * \return the size of the file described by `fd`.
//  */
// size_t file_size(const char *filename, int fd)
// {

//   // int fseek(fd, 0L, SEEK_END);

//   // return ftell(fd);
//   return -1;
// }

// /** Parse a \ref score_record struct from an
//  * array of size \ref REC_SIZE.
//  *
//  * If a name and score cannot be found in `rec_buf`,
//  * the function may abort program
//  * execution (optionally after printing an error message).
//  *
//  * \param rec_buf an array of size \ref REC_SIZE.
//  * \return a parsed \ref score_record.
//  */
// struct score_record parse_record(char rec_buf[REC_SIZE])
// {
//   struct score_record rec;
//   // void score_record_init(struct score_record *rec, const char *name, int score)
//   score_record_init(&rec, "foo", 2);

//   // Note that writing the `rec_buf` parameter as `rec_buf[REC_SIZE]`
//   // serves only as documentation of the intended use of the
//   // function - C doesn't prevent arrays of other sizes being passed.
//   //
//   // In fact, nearly any time you use an array type in C,
//   // it "decays" into a pointer -
//   // the C11 standard, sec 6.3.2.1 ("Lvalues, arrays, and
//   // function designators").
//   //
//   // (One significant exception is when you use sizeof() on an
//   // array - in that case, the proper size of the array is
//   // returned.)
//   return rec;
// }

// /** Stores the player name and score in `rec` into a buffer of size
//  * \ref REC_SIZE, representing a line from the scores file.
//  *
//  * The fields of rec should contain values that are valid for the
//  * scores file; if not, the behaviour of the function is undefined.
//  *
//  * If the caller passes a buffer of size less than \ref REC_SIZE,
//  * the behaviour of function is undefined.
//  *
//  * \param buf a `char` array of size \ref REC_SIZE.
//  * \param rec pointer to a player's score record.
//  */
// void store_record(char buf[REC_SIZE], const struct score_record *rec)
// {
// }

// /** search within the open scores file with file descriptor
//  * `fd` for a line containing the score for `player_name`.
//  * If no such line exists, -1 is returned; otherwise, the
//  * offset within the file is returned.
//  *
//  * `filename` is used only for diagnostic purposes.
//  *
//  * \param filename name of the file described by `fd`.
//  * \param fd file descriptor for an open scores file.
//  * \param player_name player name to seek for.
//  * \return position in the file where a record can be found,
//  *   or -1 if no no such record exists.
//  */
// off_t find_record(const char *filename, int fd, const char *player_name)
// {
//   return -1;
// }

// /** Adjust the score for player `player_name` in the open file
//  * with file descriptor `fd`, incrementing it by
//  * `score_to_add`. If no record for a player with that name
//  * is found in the file, then one is created and appended to
//  * the file.
//  *
//  * The `filename` parameter is purely for diagnostic purposes.
//  *
//  * If the file is not a valid "scores" file, or player name is
//  * longer than the allowable length for a score record,
//  * the function may abort program execution.
//  *
//  * \param filename name of the file from which `fd` was obtained.
//  * \param fd file descriptor for an open scores file.
//  * \param player_name name of the player whose score should be incremented.
//  * \param score_to_add amount by which to increment the score.
//  */
// void adjust_score_file(const char *filename, int fd, const char *player_name, int score_to_add)
// {
// }

static uid_t ruid, euid, rgid, egid;

/* Make effecive user ID = the real user ID */
void drop_privs()
{

  return;
  int status1;
  int status2;
  status1 = seteuid(ruid);
  status2 = setegid(rgid);
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
  status1 = setuid(euid);
  status2 = setgid(egid);
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

/** Adjust the score for player `player_name`, incrementing it by
 * `score_to_add`. The player's current score (if any) and new score
 * are stored in the scores file at `/var/lib/curdle/scores`.
 * The scores file is owned by user ID `uid`, and the process should
 * use that effective user ID when reading and writing the file.
 * If the score was changed successfully, the function returns 1; if
 * not, it returns 0, and sets `*message` to the address of
 * a string containing an error message. It is the caller's responsibility
 * to free `*message` after use.
 *
 * \todo implement the function.
 *
 * \param uid user ID of the owner of the scores file.
 * \param player_name name of the player whose score should be incremented.
 * \param score_to_add amount by which to increment the score.
 * \param message address of a pointer in which an error message can be stored.
 * \return 1 if the score was successfully changed, 0 if not.
 */
int adjust_score(uid_t uid, const char *player_name, int score_to_add, char **message)
{

  ruid = uid;
  FILE *fp;
  char line[REC_SIZE];
  char line_player[FIELD_SIZE];
  char line_score[10];
  char new_name[FIELD_SIZE];
  const char *filename = "/home/siri/cits3007/curdle-skeleton-code/curdle/tests/test-files/good/file1";
  int offset = 0;
  char newline[REC_SIZE];
  char strscore[FIELD_SIZE];
  // const char *filename = "/var/lib/curdle/scores";

  // parse name
  strncpy(new_name, player_name, 10);
  int playerlen = strlen(player_name);
  if (playerlen < 10)
  {
    int remaining = 10 - strlen(player_name);
    for (int i = 0; i < remaining; i++)
    {
      int tmp = playerlen + i; // 8+0=8, 8+1=9
      new_name[tmp] = ' ';
    }
  }
  new_name[10] = '\0';

  printf("new_name %s\n", new_name);
  printf("strlen new_name %li\n", strlen(new_name));

  get_privs();
  fp = fopen(filename, "r+");

  drop_privs();

  if (fp == NULL)
  {
    printf("exit\n");
    exit(EXIT_FAILURE);
  }

  while (fgets(line, REC_SIZE, fp) != NULL)
  {

    if (line[0] != '\n')
    {
      strncpy(line_player, line, FIELD_SIZE);
      strncpy(line_score, &line[10], FIELD_SIZE);

      line_player[10] = 0;
      // printf("line_player %s\n",line_player);

      // printf("line_player %s\n", line_player);
      // printf("strlen line_player %li\n", strlen(line_player));
      // printf("player_name %s\n", player_name);
      // printf("strlen player_name %li\n", strlen(player_name));

      int ret = strncmp(player_name, line_player, FIELD_SIZE);
      // printf("ret %i\n",ret);

      if (ret == 0)
      {

        int int_line_score = atoi(line_score);
        printf("bf line_score %i\n", int_line_score);
        int_line_score += score_to_add;

        sprintf(strscore, "%i", int_line_score);
        printf("af line_score %i\n", int_line_score);
        char new_score[FIELD_SIZE];

        // pad score
        strncpy(new_score, strscore, 10);
        printf("new_score %s\n", new_score);
        int scorelen = strlen(strscore);
        if (scorelen < 10)
        {
          int remaining = 10 - scorelen;
          for (int i = 0; i < remaining; i++)
          {
            int tmp = scorelen + i; // 8+0=8, 8+1=9
            new_score[tmp] = ' ';
          }
        }
        new_score[10] = '\0';

        printf("strlen new_score %li\n", strlen(new_score));
        printf("new_score %s\n", new_score);
        strcat(newline, new_name);
        strcat(newline, strscore);
        
        printf("offset %i\n",offset);
        fseek(fp, offset+1, SEEK_SET);
        
        printf("newline %s\n",newline);
        
        fputs(newline, fp);
      }

      // printf("line: %s\n", line);
      // printf("line player:%s | line score:%s\n", line_player, line_score);
    }
    // printf("strlen line %li\n", strlen(line));
    offset += 21;
    // printf("offset %i\n", offset);
  }

  // increase privileges and then drop them

  // struct score_record rec = parse_record(REC_SIZE);
  fclose(fp);

  return 0;
}

int main()
{
  char *message;
  ruid = getuid();
  euid = geteuid();
  rgid = getgid();
  egid = getegid();

  drop_privs();
  adjust_score(1001, "nzqf", 20, &message); // in here, privileges are dropped

  return 0;
}
