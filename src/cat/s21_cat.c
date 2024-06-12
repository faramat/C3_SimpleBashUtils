#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int b;
  int e;
  int v;
  int n;
  int s;
  int t;
} Flag;

void print_file(char *argv[], Flag *flag);
int flag_pars(int argc, char *argv[], Flag *flag);

int main(int argc, char *argv[]) {
  Flag flag_slim = {0};
  int no_flag;
  no_flag = flag_pars(argc, argv, &flag_slim);
  if (no_flag != 1) {
    while (optind < argc) {
      print_file(argv, &flag_slim);
      optind++;
    }
  } else
    fprintf(stderr, "usage: s21_cat [-benstuv] [file ...]\n");

  return 0;
}

void print_file(char *argv[], Flag *flag) {
  FILE *file = fopen(argv[optind], "r");
  if (file != NULL) {
    int line_numb = 0;
    int line_empty = 0;
    int current, previous = '\n';
    while ((current = fgetc(file)) != EOF) {
      if (flag->s == 1 && previous == '\n' && current == '\n') {
        line_empty = line_empty + 1;
        if (line_empty > 1) {
          line_empty = 1;
          continue;
        }

      } else
        line_empty = 0;

      if ((flag->n == 1 && flag->b != 1) && previous == '\n')
        printf("%6d\t", ++line_numb);
      if (flag->b && current != '\n' && previous == '\n')
        printf("%6d\t", ++line_numb);

      if (flag->e == 1 && current == '\n') {
        printf("$");
      }

      if (flag->t == 1 && current == '\t') {
        printf("^");
        current = 'I';
      }
      if (flag->v == 1) {
        if (current <= 31 && current != '\t' && current != '\n') {
          printf("^");
          current += 64;
        } else if (current == 127) {
          printf("^");
          current = '?';
        }
      }
      printf("%c", current);
      previous = current;
    }
    fclose(file);
  } else
    fprintf(stderr, "s21_cat: %s: Not such file or derectory", argv[optind]);
}

int flag_pars(int argc, char *argv[], Flag *flag) {
  struct option Flag_long[] = {{"number-nonblank", 0, 0, 'b'},
                               {"number", 0, 0, 'n'},
                               {"squeeze-blank", 0, 0, 't'},
                               {0, 0, 0, 0}};
  int flag_int, res = 0;

  while ((flag_int = getopt_long(argc, argv, "+bevnstET", Flag_long, NULL)) !=
         -1) {
    switch (flag_int) {
      case 'b':
        flag->b = 1;
        break;
      case 'e':
        flag->e = 1;
        flag->v = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 't':
        flag->t = 1;
        flag->v = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'E':
        flag->e = 1;
        break;
      case 'T':
        flag->t = 1;
        break;
      default:
        res = 1;
    }
  }
  return res;
}
