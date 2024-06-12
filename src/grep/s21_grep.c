#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10000
typedef struct {
  int e;  // Шаблон
  int i;  // Игнорирует различия регистра
  int v;  // Инвертирует смысл поиска соответствий
  int c;  // Выводит только количество совпадающих строк
  int l;  // Выводит только совпадающие файлы
  int n;  // Предваряет каждую строку вывода номером строки из файла ввода
  int h;  // Выводит совпадающие строки, не предваряя их именами файлов
  int s;  // Подавляет сообщения об ошибках о несуществующих или нечитаемых
          // файлах
  int f;  // Получает регулярные выражения из файла
  int o;  // Печатает только совпадающие (непустые) части совпавшей строки.
  int flag_null;  // если нет флагов

  int counter_files;  // количество файлов
  int count_line;
  int count_c;  // количество совпадающих строк
  int counter_e, counter_f;
  char temp[N];
} Flags;

void flag_pars(int argc, char *argv[], Flags *flags);
void print_file(char *argv[], int atrgc, Flags *flags);
int counter_file(char *[], int argc);
void temp_is_file(Flags *flags);
void flag_priority(Flags *flags);

// output using flags
void ive_print(Flags *flags, char *line, char *argv[]);
void n_print(Flags *flags, char *line, char *argv[]);
void c_print(Flags *flags, char *argv[]);
void h_print(Flags *flags, char *line);
void o_flag(char *line, regex_t reg, regmatch_t pmatch, char *argv[],
            Flags *flags);
void l_print(char *argv[]);
void flag_work(char *argv[], Flags *flags, FILE *file, regex_t reg,
               regmatch_t pmatch);

int main(int argc, char *argv[]) {
  Flags flags = {0};
  flag_pars(argc, argv, &flags);
  if (flags.e == 0 && flags.f == 0) {
    strcat(flags.temp, argv[optind]);
    optind++;
  }
  if (!flags.e && !flags.i && !flags.v && !flags.c && !flags.l && !flags.n &&
      !flags.h && !flags.s && !flags.f && !flags.o)
    flags.flag_null = 1;
  flags.counter_files = counter_file(argv, argc);
  flag_priority(&flags);
  print_file(argv, argc, &flags);

  return 0;
}

void flag_pars(int argc, char *argv[], Flags *flags) {
  int flag_int;
  while ((flag_int = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) !=
         -1) {
    switch (flag_int) {
      case 'e':
        flags->e = 1;
        if (strlen(flags->temp) > 0) strcat(flags->temp, "|");
        strcat(flags->temp, optarg);
        flags->counter_e++;
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        temp_is_file(flags);
        flags->counter_f++;
        break;
      case 'o':
        flags->o = 1;
        break;
    }
  }
}

void print_file(char *argv[], int argc, Flags *flags) {
  regmatch_t pmatch;
  regex_t reg;
  if (flags->i == 1)
    regcomp(&reg, flags->temp, REG_EXTENDED | REG_ICASE);
  else
    regcomp(&reg, flags->temp, REG_EXTENDED);
  while (optind < argc) {
    FILE *file = fopen(argv[optind], "r");
    if (file == NULL) {
      if (flags->s == 0) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                argv[optind]);
        fclose(file);
      }
    } else {
      flag_work(argv, flags, file, reg, pmatch);
    }
    optind++;
  }
  regfree(&reg);
}

int counter_file(char *[], int argc) {
  int result = 0;
  if (argc > (optind + 1)) result = 1;
  return result;
}

void temp_is_file(Flags *flags) {
  if (strlen(flags->temp) > 0) strcat(flags->temp, "|");
  FILE *file = fopen(optarg, "r");
  if (file == NULL)
    fprintf(stderr, "s21_grep: %s: No such file or directory", optarg);
  else {
    char line_count[N];
    while ((fgets(line_count, N, file)) != NULL) {
      if (line_count[strlen(line_count) - 1] == '\n')
        line_count[strlen(line_count) - 1] = '\0';
      strcat(flags->temp, line_count);
      strcat(flags->temp, "|");
    }
  }
  fclose(file);
  if (flags->temp[strlen(flags->temp) - 1] == '|')
    flags->temp[strlen(flags->temp) - 1] = '\0';
}

// output using flags
void ive_print(Flags *flags, char *line, char *argv[]) {
  if (flags->l == 0 && flags->c == 0 && flags->n == 0 && flags->h == 0 &&
      flags->o == 0) {
    if (flags->counter_files == 0)
      printf("%s\n", line);
    else {
      printf("%s:", argv[optind]);
      printf("%s\n", line);
    }
  }
}

void c_print(Flags *flags, char *argv[]) {
  if (flags->counter_files == 0 || flags->h == 1) {
    if (flags->l && flags->count_c > 0) {
      printf("1");
      printf("\n");
      printf("%s", argv[optind]);
      printf("\n");
    } else {
      printf("%d", flags->count_c);
      printf("\n");
    }
  }

  else {
    if (flags->l && flags->count_c > 0) {
      printf("%s:", argv[optind]);
      printf("1");

      printf("\n");
      printf("%s", argv[optind]);
      printf("\n");
    } else {
      printf("%s:", argv[optind]);
      printf("%d", flags->count_c);
      printf("\n");
    }
  }
}

void n_print(Flags *flags, char *line, char *argv[]) {
  if (flags->o == 0) {
    if (flags->counter_files == 0 || flags->h) {
      printf("%d:", flags->count_line);
      printf("%s", line);
      printf("\n");
    } else {
      printf("%s:", argv[optind]);
      printf("%d:", flags->count_line);
      printf("%s", line);
      printf("\n");
    }
  }
}

void h_print(Flags *flags, char *line) {
  if (flags->n == 0) {
    if (flags->c != 0) {
      printf("%s", line);
      printf("\n");
    } else
      printf("%s", line);
    printf("\n");
  }
}

void o_flag(char *line, regex_t reg, regmatch_t pmatch, char *argv[],
            Flags *flags) {
  if (flags->counter_files == 0) {
    while ((regexec(&reg, line, 1, &pmatch, 0)) == 0) {
      if (flags->n) printf("%d:", flags->count_line);
      for (regoff_t i = 0; i < pmatch.rm_eo - pmatch.rm_so; i++)
        printf("%c", (line + (pmatch.rm_so))[i]);
      printf("\n");
      line = line + pmatch.rm_eo;
    }
  } else {
    while ((regexec(&reg, line, 1, &pmatch, 0)) == 0) {
      if (flags->h == 0) printf("%s:", argv[optind]);
      if (flags->n) printf("%d:", flags->count_line);
      for (regoff_t i = 0; i < pmatch.rm_eo - pmatch.rm_so; i++)
        printf("%c", (line + (pmatch.rm_so))[i]);
      printf("\n");
      line = line + pmatch.rm_eo;
    }
  }
}
void l_print(char *argv[]) {
  printf("%s", argv[optind]);
  printf("\n");
}

void flag_work(char *argv[], Flags *flags, FILE *file, regex_t reg,
               regmatch_t pmatch) {
  size_t len = 0;
  char *line = NULL;
  flags->count_c = flags->count_line = 0;
  int read_line, result;
  size_t n_match = 1;
  while ((read_line = getline(&line, &len, file)) != -1) {
    result = regexec(&reg, line, n_match, &pmatch, 0);
    flags->count_line++;
    if ((result == 0 && flags->v == 0) || (result > 0 && flags->v == 1)) {
      if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
      if ((!flags->e && !flags->i && !flags->v && !flags->c && !flags->l &&
           !flags->n && !flags->h && !flags->s && !flags->f && !flags->o))
        ive_print(flags, line, argv);
      if ((flags->i || flags->v) || flags->f) ive_print(flags, line, argv);
      if (flags->e) ive_print(flags, line, argv);
      if (flags->l && flags->c == 0) {
        l_print(argv);
        break;
      }
      if (flags->c) ++flags->count_c;
      if (flags->n) n_print(flags, line, argv);
      if (flags->h && flags->c == 0 && flags->o == 0) h_print(flags, line);
      if (flags->o) o_flag(line, reg, pmatch, argv, flags);
    }
  }
  if (flags->c) c_print(flags, argv);
  free(line);
  fclose(file);
}

void flag_priority(Flags *flags) {
  if (flags->i || flags->v || flags->c || flags->l || flags->n || flags->h ||
      flags->f) {
    flags->e = 0;
  }
  if (flags->o) {
    flags->e = 0;
  }
  if (flags->v) flags->o = 0;
  if (flags->c && flags->n) flags->n = 0;
  if (flags->c) flags->o = 0;
}
