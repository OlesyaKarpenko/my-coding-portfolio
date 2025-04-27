#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

typedef enum bool { false, true } bool;
typedef struct cat_flags {
  bool flag_b, flag_e, flag_n, flag_s, flag_t, flag_v;
} cat_flags;

void do_flags(cat_flags *cf, int argc, char **argv);
void get_flags(cat_flags *cf, int argc, char **argv);
void out_end_of_line(cat_flags *cf);
void set_false(cat_flags *cf);
void try_flag_s(cat_flags *cf, bool no_empty, bool first_line, bool *merged,
                int *line_cnt);
void try_flag_v(char file_c);
void try_out_line_number(cat_flags *cf, bool no_empty, int *line_cnt);

#endif  // SRC_CAT_S21_CAT_H_
