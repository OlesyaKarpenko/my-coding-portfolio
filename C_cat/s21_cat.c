#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void s21_cat(FILE *file, cat_flags *cf);

int main(int argc, char **argv) {
  cat_flags cf;
  set_false(&cf);
  get_flags(&cf, argc, argv);
  do_flags(&cf, argc, argv);
  return 0;
}

void set_false(cat_flags *cf) {
  cf->flag_b = false;
  cf->flag_e = false;
  cf->flag_n = false;
  cf->flag_s = false;
  cf->flag_t = false;
  cf->flag_v = false;
}

void get_flags(cat_flags *cf, int argc, char **argv) {
  int res, opt_id;
  // opterr = 0;
  const struct option l_opt[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
  };
  while ((res = getopt_long(argc, argv, "+bensvtET", l_opt, &opt_id)) != -1) {
    switch (res) {
      case 'b': {
        cf->flag_b = 1;
        break;
      }
      case 'e': {
        cf->flag_e = 1;
        cf->flag_v = 1;
        break;
      }
      case 'n': {
        cf->flag_n = 1;
        break;
      }
      case 's': {
        cf->flag_s = 1;
        break;
      }
      case 'v': {
        cf->flag_v = 1;
        break;
      }
      case 't': {
        cf->flag_t = 1;
        cf->flag_v = 1;
        break;
      }
      case 'E': {
        cf->flag_e = 1;
        break;
      }
      case 'T': {
        cf->flag_t = 1;
        break;
      }
      default: {
        // fprintf(stderr, "%s: illegal option %s\n", argv[0], argv[opt_id]);
        fprintf(stderr, "usage: cat [-benstuv] [file ...]");
        exit(1);
      }
    }
  }
}

void do_flags(cat_flags *cf, int argc, char **argv) {
  FILE *file;
  bool no_file = true;

  for (int i = 1; i < argc; ++i) {
    file = fopen(argv[i], "r");
    if (file) {
      no_file = false;
      s21_cat(file, cf);
      fclose(file);
    }
  }
  if (no_file) {
    fprintf(stderr, "s21_cat: No files available to open");
    exit(1);
  }
}

void s21_cat(FILE *file, cat_flags *cf) {
  char file_c = fgetc(file);
  int line_cnt = 0;
  bool got_next = false, merged = false;
  bool no_empty = (file_c != '\n'), first_line = (file_c == '\n');

  if (file_c != EOF) try_out_line_number(cf, no_empty, &line_cnt);

  while (file_c != EOF) {
    if (file_c == '\n') {
      if (!merged) out_end_of_line(cf);

      file_c = fgetc(file);
      no_empty = (file_c != '\n' && file_c != EOF);

      if (no_empty) merged = false;

      if (cf->flag_s && file_c == '\n')
        try_flag_s(cf, no_empty, first_line, &merged, &line_cnt);

      if (file_c != EOF && !merged)
        try_out_line_number(cf, no_empty, &line_cnt);

      got_next = true;
      no_empty = false;
      first_line = false;
    } else {
      if ((cf->flag_t) && file_c == '\t') {
        printf("^I");
      } else {
        if (cf->flag_v) {
          try_flag_v(file_c);
        } else {
          printf("%c", file_c);
        }
      }
      got_next = false;
      merged = false;
    }
    if (!got_next) file_c = fgetc(file);
  }
}

void try_out_line_number(cat_flags *cf, bool no_empty, int *line_cnt) {
  if (((cf->flag_b) && no_empty) || (!(cf->flag_b) && (cf->flag_n)))
    printf("%6d\t", ++(*line_cnt));
}

void out_end_of_line(cat_flags *cf) {
  if (cf->flag_e) printf("$");
  printf("\n");
}

void try_flag_s(cat_flags *cf, bool no_empty, bool first_line, bool *merged,
                int *line_cnt) {
  if (first_line) *merged = true;
  if (!(*merged)) {
    try_out_line_number(cf, no_empty, line_cnt);
    out_end_of_line(cf);
    (*merged) = true;
  }
}

void try_flag_v(char file_c) {
  if ((int)(unsigned char)file_c < 32 && file_c != '\t') {
    printf("^%c", (int)file_c + 64);
  } else if ((int)(unsigned char)file_c == 127) {
    printf("^%c", (int)file_c - 64);
  } else if ((int)(unsigned char)file_c > 127 &&
             (int)(unsigned char)file_c < 160) {
    printf("M-^%c", (int)file_c - 64);
  } else if ((int)(unsigned char)file_c >= 160 &&
             (int)(unsigned char)file_c < 255) {
    printf("M-%c", (int)file_c - 128);
  } else {
    printf("%c", file_c);
  }
}
