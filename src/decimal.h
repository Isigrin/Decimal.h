#ifndef SRC_DECIMAL_H_
#define SRC_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h> //
#include <stdlib.h>
#include <string.h>

#define TRUE 0
#define FALSE 1
#define EPSILON 1e-7
#define SIGN 0x80000000
typedef enum {
  NORMAL_VALUE = 0,
  _INFINITY = 1,
  NEGATIVE_INFINITY = 2,
  _NAN = 3,
  ADDCODE = 4,
} value_type_t;

typedef struct {
  unsigned bits[4];
  value_type_t value_type;
} decimal;

typedef union {
  float f;
  struct {
    unsigned man : 23;
    unsigned exp : 8;
    unsigned sign : 1;
  } raw;
} float_union;

typedef union {
  int ui;
  float fl;
} floatbits;

// ==Convertors and parsers== //

int from_int_to_decimal(int src, decimal *dst);
int from_float_to_decimal(float src, decimal *dst);
int from_decimal_to_int(decimal src, int *dst);
int from_decimal_to_float(decimal src, float *dst);

// ==Arithmetic Operators== //

decimal add(decimal dec_a, decimal dec_b); // Сложение
decimal sub(decimal dec_a, decimal dec_b); // Вычитание
decimal mul(decimal dec_a, decimal dec_b); // Умножение
decimal div(decimal dec_a, decimal dec_b); // Деление
decimal basic_div(decimal a, decimal b);
decimal mod(decimal, decimal); // Взятие по модулю

// ==Comparison Operators== //

int is_less(decimal dec_a, decimal dec_b); // dec_a < dec_b
int is_less_or_equal(decimal dec_a,
                     decimal dec_b);          // dec_a <= dec_b
int is_greater(decimal dec_a, decimal dec_b); // dec_a > dec_b
int is_greater_or_equal(decimal dec_a,
                        decimal dec_b);         // dec_a >= dec_b
int is_equal(decimal dec_a, decimal dec_b);     // dec_a == dec_b
int is_not_equal(decimal dec_a, decimal dec_b); // dec_a != dec_b

// ==Another functions== //

decimal floor(decimal src);
decimal dec_round(decimal src);
decimal truncate(decimal src);
decimal negate(decimal src);

// ==Dop functions== //

void init_dec_bits(decimal *dst);
void set_bit(int *dst, unsigned bit, unsigned set);
unsigned get_bit(int src, unsigned bit);
void set_bit_dec(decimal *dst, unsigned bit, unsigned set);
unsigned get_bit_dec(decimal dst, unsigned bit);
void set_value_type(float num, decimal *dst);
void print_bin(unsigned n); //////////////////////////
unsigned find_scale(float src);
void shift(decimal *src, int offset);
unsigned get_scale(decimal src);
void set_scale(decimal *src, unsigned scale);
void set_dec_scale(decimal *dst, int scale);
decimal summ(decimal par_a, decimal par_b);
unsigned dec_div_10(decimal *src);
int compar_func(decimal dec_a, decimal dec_b);
int norm_scale(decimal *dec_a, decimal *dec_b);
void print_dec(decimal fucking_decimal);
decimal dop_code(decimal *dec_b);
int dec_nan(decimal *dec_a, decimal *dec_b);
int dec_inf(decimal *dec_a, decimal *dec_b);
int dec_neg_inf(decimal *dec_a, decimal *dec_b);
int is_zero_dec(decimal dec_a, decimal dec_b);
void only_bits_equals(decimal src, decimal *dst);
int last_bit(decimal dec);
void dec_to_addcode(decimal *dec);
decimal type_for_add(decimal dec_a, decimal dec_b);
void clear_dec(decimal *dec);
decimal str_to_dec(char binary_number[], decimal *dec, int point_place);
int getFloatSign(float *src);
int getFloatExp(float *src);
unsigned get_last_bit(decimal src);
decimal prov_type_mul(decimal a, decimal b);
decimal prov_type_div(decimal dec_a, decimal dec_b);
decimal div_only_bit(decimal dec_a, decimal dec_b, decimal *remainder);
unsigned dec_div_10_round(decimal *src);
decimal shift_dec(decimal src, int offset);
decimal shift_left(decimal src, unsigned offset);
decimal shift_right(decimal src, unsigned offset);
int ModTen(decimal *dec);
decimal DenTen(decimal *dec);
int len_of_digit(decimal dig);
decimal div_int(decimal a, decimal b, decimal *buf);
int detect_minus_result(decimal *a, decimal *b);

#endif // SRC_DECIMAL_H_
