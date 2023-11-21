#include <check.h>

#include "decimal.h"

#define INT_TEST_0 100
#define INT_TEST_1 -100
#define INT_TEST_2 0
#define INT_TEST_3 -999

#define FLT_TEST_0 100.900
#define FLT_TEST_1 -100.900
#define FLT_TEST_2 0.900
#define FLT_TEST_3 -0.900

START_TEST(from_int_to_decimal_1) {
    decimal val;

    from_int_to_decimal(0, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);

    from_int_to_decimal(-128, &val);
    ck_assert_int_eq(val.bits[0], 128);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 2147483648);

    from_int_to_decimal(127, &val);
    ck_assert_int_eq(val.bits[0], 127);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);

    from_int_to_decimal(-2147483648, &val);
    ck_assert_int_eq(val.bits[0], 2147483648);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 2147483648);

    from_int_to_decimal(2147483647, &val);
    ck_assert_int_eq(val.bits[0], 2147483647);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST

START_TEST(from_float_to_decimal_1) {
    decimal val;

    from_float_to_decimal(0.03, &val);
    ck_assert_int_eq(val.bits[0], 3);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 131072);

    from_float_to_decimal(127.1234, &val);
    ck_assert_int_eq(val.bits[0], 1271234);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 262144);
}
END_TEST
START_TEST(from_float_to_decimal_2) {
    decimal val;
    from_float_to_decimal(-128.023, &val);
    ck_assert_int_eq(val.bits[0], 12802299);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    // ck_assert_int_eq(val.bits[3], -2147155968);
}
START_TEST(from_float_to_decimal_3) {
    decimal val;
    from_float_to_decimal(-2.1474836E+09, &val);
    ck_assert_int_eq(val.bits[0], 2147483648);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 2147483648);
}
END_TEST
START_TEST(from_float_to_decimal_4) {
    decimal val;
    from_float_to_decimal(22.14836E+03, &val);
    ck_assert_int_eq(val.bits[0], 2214836);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 131072);
}
END_TEST
START_TEST(from_float_to_decimal_5) {
    decimal val;
    from_float_to_decimal(1.02E+09, &val);
    ck_assert_int_eq(val.bits[0], 1020000000);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST
START_TEST(from_float_to_decimal_6) {
    decimal val;
    from_float_to_decimal(-333.2222, &val);
    ck_assert_int_eq(val.bits[0], 3332222);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    // ck_assert_int_eq(val.bits[3], -2147221504);
}
END_TEST
START_TEST(from_float_to_decimal_7) {
    decimal val;
    float a = 1.0 / 0.0;
    from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST
START_TEST(from_float_to_decimal_8) {
    decimal val;
    float a = -1.0 / 0.0;
    from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST
START_TEST(from_float_to_decimal_9) {
    decimal val;
    float a = 0.0 / 0.0;
    from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST

START_TEST(from_decimal_to_int_1) {
    decimal src;
    int result = 0, number = 0;
    src.value_type = 0;
    src.bits[0] = INT_MAX;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = from_decimal_to_int(src, &number);
    ck_assert_float_eq(number, 2147483647);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(from_decimal_to_int_2) {
    decimal src;
    int result = 0, number = 0;
    src.value_type = 0;
    src.bits[0] = 133141;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, 133141);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(from_decimal_to_int_3) {
    decimal src;
    int result = 0, number = 0;
    src.value_type = 0;
    src.bits[0] = 2147483647;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    set_bit_dec(&src, 127, 1);
    result = from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, -2147483647);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(from_decimal_to_int_4) {
    decimal src;
    int result = 0, number = 0;
    src.value_type = 0;
    src.bits[0] = 123451234;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147483648;
    result = from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, -123451234);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(from_decimal_to_int_5) {
    decimal src;
    int result = 0, number = 0;
    src.value_type = 0;
    src.bits[0] = 18;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147483648;
    result = from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, -18);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(from_decimal_to_int_6) {
    decimal src;
    int result = 0, number = 0;
    src.value_type = 0;
    src.bits[0] = 4294967295;
    src.bits[1] = 4294967295;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = from_decimal_to_int(src, &number);
    ck_assert_int_eq(number, 0xFFFFFFFFFFFFFFFF);
    ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(from_decimal_to_float_1) {
    decimal src;
    int result = 0;
    float number = 0.0;
    src.value_type = 0;
    src.bits[0] = 18122;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147680256;
    result = from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, -18.122);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(from_decimal_to_float_2) {
    decimal src;
    int result = 0;
    float number = 0.0;
    src.value_type = 0;
    src.bits[0] = 1812;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147483648;
    result = from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, -1812);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(from_decimal_to_float_3) {
    decimal src;
    int result = 0;
    float number = 0.0;
    src.value_type = 0;
    src.bits[0] = 0XFFFFFF;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, 16777215);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(from_decimal_to_float_4) {
    decimal src;
    int result = 0;
    float number = 0.0;
    src.value_type = 0;
    src.bits[0] = 23450987;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147745792;
    result = from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, -2345.0987);
    ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(from_decimal_to_float_5) {
    decimal src;
    int result = 0;
    float number = 0.0;
    src.value_type = 0;
    src.bits[0] = 4294967295;
    src.bits[1] = 4294967295;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, 0xFFFFFFFFFFFFFFFF);
    ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(from_int_to_dec) {
    decimal res_dec, correct_result;
    init_dec_bits(&res_dec), init_dec_bits(&correct_result);
    int signed_integer = 0, res_of_conv = 0;

    // check NULL
    res_of_conv = from_int_to_decimal(signed_integer, NULL);
    ck_assert_int_eq(res_of_conv, 1);

    // check 0
    res_of_conv = from_int_to_decimal(signed_integer, &res_dec);
    str_to_dec("0", &correct_result, 0);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_int_eq(is_equal(correct_result, res_dec), 0);

    // check 123456789
    signed_integer = 123456789;
    res_of_conv = from_int_to_decimal(signed_integer, &res_dec);
    str_to_dec("111010110111100110100010101", &correct_result, 0);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_int_eq(is_equal(correct_result, res_dec), 0);

    // check -123456789
    signed_integer = -123456789;
    res_of_conv = from_int_to_decimal(signed_integer, &res_dec);
    str_to_dec("-111010110111100110100010101", &correct_result, 0);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_int_eq(is_equal(correct_result, res_dec), 0);
}
END_TEST

START_TEST(from_float_to_dec) {
    decimal res, correct_result;
    init_dec_bits(&res), init_dec_bits(&correct_result);
    int res_of_conv = 0;
    float fl_val = 0;

    // check NULL
    res_of_conv = from_float_to_decimal(fl_val, NULL);
    ck_assert_int_eq(res_of_conv, 1);

    // check NaN
    fl_val = NAN;
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 1);
    ck_assert_int_eq(res.value_type, NAN);

    // check INF
    fl_val = INFINITY;
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 1);
    ck_assert_int_eq(res.value_type, INFINITY);

    // check -INF
    fl_val = -INFINITY;
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 1);
    ck_assert_int_eq(res.value_type, NEGATIVE_INFINITY);

    // check 0
    fl_val = 0;
    res_of_conv = from_int_to_decimal(fl_val, &res);
    str_to_dec("0", &correct_result, 0);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_int_eq(is_equal(correct_result, res), 0);

    // new tests
    // printf("%.30f\n%.30f\n%.30f\n", fl_val, back_conv, (fl_val - back_conv));
    fl_val = 123.123456;
    float eps = 1e-5;
    // fl_bits fl = {.fl = fl_val};
    float back_conv = 0;
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = pow(2, 95);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    // ck_assert_float_lt((fl_val - back_conv), eps);
    fl_val = pow(2, 68), back_conv = 0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    // ck_assert_float_lt((fl_val - back_conv), eps);
    fl_val = pow(2, 27), back_conv = 0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = 123.777, back_conv = 0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = -123.777, back_conv = 0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = pow(2, 72), back_conv = 0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    // ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = 10000000, back_conv = 0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = 123.456897, back_conv = 0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    // new new test
    fl_val = 0.0, back_conv = 0.0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 1);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = 0.6, back_conv = 0.0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = 0.06, back_conv = 0.0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);

    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = 0.223648, back_conv = 0.0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = 0.000000000000001, back_conv = 0.0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = 10e-28, back_conv = 0.0, init_dec_bits(&res);

    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = 10e-29, back_conv = 0.0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 1);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    // ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = 128935798234578236870459., back_conv = 0.0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    // ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = -7283946982385609.12904580, back_conv = 0.0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = -0.234634573467456868, back_conv = 0.0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    // ck_assert_float_lt((fl_val - back_conv), eps);

    fl_val = -0.344375236702030878974356, back_conv = 0.0, init_dec_bits(&res);
    res_of_conv = from_float_to_decimal(fl_val, &res);
    ck_assert_int_eq(res_of_conv, 0);
    res_of_conv = from_decimal_to_float(res, &back_conv);
    ck_assert_int_eq(res_of_conv, 0);
    // ck_assert_float_lt((fl_val - back_conv), eps);
}
END_TEST

START_TEST(from_dec_to_int) {
    decimal dec;
    init_dec_bits(&dec);
    int res_of_conv = 0, converted_int = 0;

    // check NULL
    res_of_conv = from_decimal_to_int(dec, NULL);
    ck_assert_int_eq(res_of_conv, 1);

    // check INF
    dec.value_type = INFINITY;
    res_of_conv = from_decimal_to_int(dec, &converted_int);
    ck_assert_int_eq(res_of_conv, 1);

    // check NEG_INF
    dec.value_type = NEGATIVE_INFINITY;
    res_of_conv = from_decimal_to_int(dec, &converted_int);
    ck_assert_int_eq(res_of_conv, 1);

    // check NaN
    dec.value_type = NAN;
    res_of_conv = from_decimal_to_int(dec, &converted_int);
    ck_assert_int_eq(res_of_conv, 1);
    init_dec_bits(&dec);

    // check overflow INT_MAX + 1 = 2147483648
    str_to_dec("10000000000000000000000000000000", &dec, 0);
    res_of_conv = from_decimal_to_int(dec, &converted_int);
    ck_assert_int_eq(res_of_conv, 0);
    init_dec_bits(&dec);

    // // check max
    str_to_dec("1111111111111111111111111111111", &dec, 0);
    res_of_conv = from_decimal_to_int(dec, &converted_int);
    ck_assert_int_eq(converted_int, INT_MAX);
    ck_assert_int_eq(res_of_conv, 0);
    init_dec_bits(&dec);

    // // check min
    str_to_dec("-10000000000000000000000000000000", &dec, 0);
    res_of_conv = from_decimal_to_int(dec, &converted_int);
    ck_assert_int_eq(converted_int, INT_MIN);
    ck_assert_int_eq(res_of_conv, 0);
    init_dec_bits(&dec);

    // // check float 999999.875
    str_to_dec("111011100110101100100110000011", &dec, 3);
    res_of_conv = from_decimal_to_int(dec, &converted_int);
    ck_assert_int_eq(converted_int, 999999);
    ck_assert_int_eq(res_of_conv, 0);
    init_dec_bits(&dec);
}
END_TEST

START_TEST(from_dec_to_float) {
    decimal dec;
    init_dec_bits(&dec);
    int res_of_conv = 0;
    float converted_fl = 0, correct_result = 0;

    // check NULL
    res_of_conv = from_decimal_to_float(dec, NULL);
    ck_assert_int_eq(res_of_conv, 1);

    // check INF
    dec.value_type = INFINITY;
    res_of_conv = from_decimal_to_float(dec, &converted_fl);
    ck_assert_int_eq(res_of_conv, 1);
    // ck_assert_int_eq(1, isinf(converted_fl));

    // check NEG_INF
    dec.value_type = NEGATIVE_INFINITY;
    res_of_conv = from_decimal_to_float(dec, &converted_fl);
    ck_assert_int_eq(res_of_conv, 1);
    // ck_assert_int_eq(1, isinf(-converted_fl));

    // check NaN
    dec.value_type = NAN;
    res_of_conv = from_decimal_to_float(dec, &converted_fl);
    ck_assert_int_eq(res_of_conv, 1);
    // ck_assert_int_eq(1, isnan(converted_fl));

    init_dec_bits(&dec);
    // check max 79228162514264337593543950335
    correct_result = 79228162514264337593543950335.;
    str_to_dec(
        "1111111111111111111111111111111111111111111111111111111111111111111111"
        "11111111111111111111111111",
        &dec, 0);
    res_of_conv = from_decimal_to_float(dec, &converted_fl);
    // ck_assert_float_eq(converted_fl, correct_result);
    ck_assert_int_eq(res_of_conv, 0);
    init_dec_bits(&dec);

    // check min 1e-28
    correct_result = 1e-28;
    str_to_dec("1", &dec, 28);
    res_of_conv = from_decimal_to_float(dec, &converted_fl);
    ck_assert_float_eq(converted_fl, correct_result);
    ck_assert_int_eq(res_of_conv, 0);
    init_dec_bits(&dec);

    // check precise 1.234567
    correct_result = 1.234567;
    str_to_dec("100101101011010000111", &dec, 6);
    res_of_conv = from_decimal_to_float(dec, &converted_fl);
    ck_assert_float_eq(converted_fl, correct_result);
    ck_assert_int_eq(res_of_conv, 0);
    init_dec_bits(&dec);
}
END_TEST

START_TEST(compare_less) {
    decimal dst1, dst2;

    from_float_to_decimal(1.2345, &dst1);
    from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(is_less(dst1, dst2), FALSE);  // 1.2345 < 1.2
    ck_assert_int_eq(is_less(dst2, dst1), TRUE);   // 1.2 < 1.2345

    dst1 = negate(dst1);
    dst2 = negate(dst2);
    ck_assert_int_eq(is_less(dst1, dst2), TRUE);   // -1.2345 < -1.2
    ck_assert_int_eq(is_less(dst2, dst1), FALSE);  // -1.2 < -1.2345

    dst1 = negate(dst1);
    ck_assert_int_eq(is_less(dst1, dst2),
                     FALSE);                          //  1.2345 < -1.2 fail
    ck_assert_int_eq(is_less(dst2, dst1), TRUE);  //  -1.2 < 1.2345 fail

    from_float_to_decimal(0.0, &dst1);
    from_float_to_decimal(0.0, &dst2);
    ck_assert_int_eq(is_less(dst1, dst2), FALSE);  // 0 < 0

    dst1.value_type = INFINITY;
    dst2.value_type = NORMAL_VALUE;
    ck_assert_int_eq(is_less(dst1, dst2), FALSE);  // +INFINITY < 0
    ck_assert_int_eq(is_less(dst2, dst1), TRUE);   // 0 < +INFINITY

    dst1.value_type = NEGATIVE_INFINITY;
    dst2.value_type = NORMAL_VALUE;
    ck_assert_int_eq(is_less(dst1, dst2), TRUE);   // -INFINITY < 0
    ck_assert_int_eq(is_less(dst2, dst1), FALSE);  // 0 < -INFINITY
}
END_TEST

START_TEST(compare_greater) {
    decimal dst1, dst2;

    from_float_to_decimal(1.2345, &dst1);
    from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(is_greater(dst1, dst2), TRUE);   // 1.2345 > 1.2
    ck_assert_int_eq(is_greater(dst2, dst1), FALSE);  // 1.2 > 1.2345

    dst1 = negate(dst1);
    dst2 = negate(dst2);
    ck_assert_int_eq(is_greater(dst1, dst2), FALSE);  // -1.2345 > -1.2
    ck_assert_int_eq(is_greater(dst2, dst1), TRUE);   // -1.2 > 1.2345

    dst1 = negate(dst1);
    ck_assert_int_eq(is_greater(dst1, dst2), TRUE);   //  1.2345 > -1.2
    ck_assert_int_eq(is_greater(dst2, dst1), FALSE);  //  -1.2 > 1.2345

    from_float_to_decimal(0.0, &dst1);
    from_float_to_decimal(0.0, &dst2);
    ck_assert_int_eq(is_greater(dst1, dst2), FALSE);  // 0 > 0

    dst1.value_type = INFINITY;
    dst2.value_type = NORMAL_VALUE;
    ck_assert_int_eq(is_greater(dst1, dst2), TRUE);   // +INFINITY > 0
    ck_assert_int_eq(is_greater(dst2, dst1), FALSE);  // 0 > +INFINITY

    dst1.value_type = NEGATIVE_INFINITY;
    dst2.value_type = NORMAL_VALUE;
    ck_assert_int_eq(is_greater(dst1, dst2), FALSE);  // -INFINITY > 0
    ck_assert_int_eq(is_greater(dst2, dst1), TRUE);   // 0 > -INFINITY
}
END_TEST

START_TEST(compare_less_or_equal) {
    decimal dst1, dst2;

    from_float_to_decimal(1.2345, &dst1);
    from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(is_less_or_equal(dst1, dst2),
                     FALSE);  // 1.2345 <= 1.2
    ck_assert_int_eq(is_less_or_equal(dst2, dst1),
                     TRUE);  // 1.2 <= 1.2345

    dst1 = negate(dst1);
    dst2 = negate(dst2);
    ck_assert_int_eq(is_less_or_equal(dst1, dst2),
                     TRUE);  // -1.2345 <= -1.2
    ck_assert_int_eq(is_less_or_equal(dst2, dst1),
                     FALSE);  // -1.2 <= 1.2345

    dst1 = negate(dst1);
    ck_assert_int_eq(is_less_or_equal(dst1, dst2),
                     FALSE);  //  1.2345 <= -1.2 // fail
    ck_assert_int_eq(is_less_or_equal(dst2, dst1),
                     TRUE);  //  -1.2 <= 1.2345

    from_float_to_decimal(0.0, &dst1);
    from_float_to_decimal(0.0, &dst2);
    ck_assert_int_eq(is_less_or_equal(dst1, dst2), TRUE);  // 0 <= 0

    dst1.value_type = INFINITY;
    dst2.value_type = NORMAL_VALUE;
    ck_assert_int_eq(is_less_or_equal(dst1, dst2),
                     FALSE);  // +INFINITY <= 0
    ck_assert_int_eq(is_less_or_equal(dst2, dst1),
                     TRUE);  // 0 <= +INFINITY

    dst1.value_type = INFINITY;
    dst2.value_type = INFINITY;
    ck_assert_int_eq(is_less_or_equal(dst2, dst1),
                     TRUE);  // +INFINITY <= +INFINITY

    dst1.value_type = NEGATIVE_INFINITY;
    dst2.value_type = NORMAL_VALUE;
    ck_assert_int_eq(is_less_or_equal(dst1, dst2),
                     TRUE);  // -INFINITY <= 0
    ck_assert_int_eq(is_less_or_equal(dst2, dst1),
                     FALSE);  // 0 <= -INFINITY

    from_float_to_decimal(1726.73, &dst1);
    from_float_to_decimal(1726.73, &dst2);
    ck_assert_int_eq(is_less_or_equal(dst1, dst2),
                     TRUE);  // 1726.73 <= 1726.73
}
END_TEST

START_TEST(compare_greater_or_equal) {
    decimal dst1, dst2;

    from_float_to_decimal(1.2345, &dst1);
    from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(is_greater_or_equal(dst1, dst2),
                     TRUE);  // 1.2345 >= 1.2
    ck_assert_int_eq(is_greater_or_equal(dst2, dst1),
                     FALSE);  // 1.2 >= 1.2345

    dst1 = negate(dst1);
    dst2 = negate(dst2);
    ck_assert_int_eq(is_greater_or_equal(dst1, dst2),
                     FALSE);  // -1.2345 >= -1.2
    ck_assert_int_eq(is_greater_or_equal(dst2, dst1),
                     TRUE);  // -1.2 >= 1.2345

    dst1 = negate(dst1);
    ck_assert_int_eq(is_greater_or_equal(dst1, dst2),
                     TRUE);  //  1.2345 >= -1.2 // fail
    ck_assert_int_eq(is_greater_or_equal(dst2, dst1),
                     FALSE);  //  -1.2 >= 1.2345

    from_float_to_decimal(0.0, &dst1);
    from_float_to_decimal(0.0, &dst2);
    ck_assert_int_eq(is_greater_or_equal(dst1, dst2), TRUE);  // 0 >= 0

    dst1.value_type = INFINITY;
    dst2.value_type = NORMAL_VALUE;
    ck_assert_int_eq(is_greater_or_equal(dst1, dst2),
                     TRUE);  // +INFINITY >= 0
    ck_assert_int_eq(is_greater_or_equal(dst2, dst1),
                     FALSE);  // 0 >= +INFINITY

    dst1.value_type = NEGATIVE_INFINITY;
    dst2.value_type = NORMAL_VALUE;
    ck_assert_int_eq(is_greater_or_equal(dst1, dst2),
                     FALSE);  // -INFINITY >= 0
    ck_assert_int_eq(is_greater_or_equal(dst2, dst1),
                     TRUE);  // 0 >= -INFINITY
}
END_TEST

START_TEST(compare_equal) {
    decimal dst1, dst2;

    from_float_to_decimal(1.2345, &dst1);
    from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(is_equal(dst1, dst2), FALSE);  // 1.2345 = 1.2

    from_float_to_decimal(1.2, &dst1);
    from_float_to_decimal(1.2, &dst2);
    dst1.value_type = NEGATIVE_INFINITY;
    dst2.value_type = NEGATIVE_INFINITY;
    ck_assert_int_eq(is_equal(dst1, dst2), TRUE);  // 1.2 = 1.2

    from_float_to_decimal(-234.17, &dst1);
    from_float_to_decimal(234.17, &dst2);
    ck_assert_int_eq(is_equal(dst1, dst2), FALSE);

    from_float_to_decimal(234.17, &dst1);
    from_float_to_decimal(234.17, &dst2);
    ck_assert_int_eq(is_equal(dst1, dst2), TRUE);

    dst1.value_type = INFINITY;
    dst2.value_type = INFINITY;
    ck_assert_int_eq(is_less_or_equal(dst2, dst1),
                     TRUE);  // +INFINITY = +INFINITY
}
END_TEST

START_TEST(compare_equal_null) {
    decimal dst1, dst2;
    from_int_to_decimal(0, &dst1);  // -0 == 0
    from_int_to_decimal(0, &dst2);
    dst1.bits[3] = SIGN;
    ck_assert_int_eq(is_equal(dst1, dst2), TRUE);
    from_int_to_decimal(0, &dst1);  // NAN == NAN
    from_int_to_decimal(0, &dst2);
    dst1.value_type = NAN;
    dst2.value_type = NAN;
    ck_assert_int_eq(is_equal(dst1, dst2), FALSE);
}
END_TEST

START_TEST(compare_not_equal) {
    decimal dst1, dst2;

    from_float_to_decimal(1.2345, &dst1);
    from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(is_not_equal(dst1, dst2), TRUE);  // 1.2345 = 1.2

    from_float_to_decimal(1.2, &dst1);
    from_float_to_decimal(1.2, &dst2);
    ck_assert_int_eq(is_not_equal(dst1, dst2), FALSE);  // 1.2 = 1.2

    from_float_to_decimal(-234.17, &dst1);
    from_float_to_decimal(234.17, &dst2);
    dst1.value_type = NEGATIVE_INFINITY;
    dst2.value_type = NEGATIVE_INFINITY;
    ck_assert_int_eq(is_not_equal(dst1, dst2), FALSE);

    from_float_to_decimal(234.17, &dst1);
    from_float_to_decimal(234.17, &dst2);
    ck_assert_int_eq(is_not_equal(dst1, dst2), FALSE);
}
END_TEST

START_TEST(eq_funcs_test) {
    decimal dec1, dec2;
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    dec1.bits[0] = 1325411;
    dec2.bits[0] = 1325411;
    dec1.bits[1] = 121;
    dec2.bits[1] = 121;
    dec1.bits[2] = 213;
    dec2.bits[2] = 213;

    ck_assert_int_eq(is_equal(dec1, dec2), 0);

    ck_assert_int_eq(is_not_equal(dec1, dec2), 1);
    set_bit_dec(&dec2, 127, 1);
    ck_assert_int_eq(is_equal(dec1, dec2), 1);
    ck_assert_int_eq(is_not_equal(dec1, dec2), 0);

    set_bit_dec(&dec1, 127, 1);

    ck_assert_int_eq(is_equal(dec1, dec2), 0);
    ck_assert_int_eq(is_not_equal(dec1, dec2), 1);

    dec2.bits[0] = 132541;

    ck_assert_int_eq(is_equal(dec1, dec2), 1);
    ck_assert_int_eq(is_not_equal(dec1, dec2), 0);

    dec1.bits[0] = 2147483647;
    dec2.bits[0] = 2147483647;
    dec1.bits[1] = 0;
    dec2.bits[1] = 2147483647;
    dec1.bits[2] = 2147483647;
    dec2.bits[2] = 2147483647;

    ck_assert_int_eq(is_equal(dec1, dec2), 1);
    ck_assert_int_eq(is_not_equal(dec1, dec2), 0);

    str_to_dec("-1010011110", &dec1, 1);
    str_to_dec("-1000011", &dec2, 0);
    ck_assert_int_eq(is_equal(dec1, dec2), TRUE);
    ck_assert_int_eq(is_not_equal(dec1, dec2), FALSE);

    // new_test
    str_to_dec(
        "1001111110010000011011001100101110000011110001011000110110100100000011"
        "110101000001000100100011",
        &dec1, 12);
    str_to_dec(
        "1001111110010000011011001100101110000011110001011000110110100100000011"
        "110101000001000100100011",
        &dec2, 12);
    ck_assert_int_eq(is_not_equal(dec1, dec2), FALSE);

    dec2.value_type = INFINITY;
    ck_assert_int_eq(is_not_equal(dec1, dec2), TRUE);

    dec1.value_type = INFINITY;
    ck_assert_int_eq(is_not_equal(dec1, dec2), FALSE);
}
END_TEST

START_TEST(greater_funcs_test) {
    decimal dec1, dec2;
    init_dec_bits(&dec1);
    dec1.value_type = NORMAL_VALUE;
    init_dec_bits(&dec2);
    dec2.value_type = NORMAL_VALUE;
    dec1.bits[0] = 1325411;
    dec2.bits[0] = 1325411;
    dec1.bits[1] = 121;
    dec2.bits[1] = 121;
    dec1.bits[2] = 213;
    dec2.bits[2] = 213;

    // dec1 == dec2
    ck_assert_int_eq(is_greater(dec1, dec2), 1);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), 0);

    dec2.bits[0] = 15;
    // dec1 > dec2
    ck_assert_int_eq(is_greater(dec1, dec2), 0);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), 0);

    set_bit_dec(&dec1, 127, 1);
    // dec1 < dec2
    ck_assert_int_eq(is_greater(dec1, dec2), 1);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), 1);

    set_bit_dec(&dec2, 127, 1);
    // dec1 < dec2
    ck_assert_int_eq(is_greater(dec1, dec2), 1);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), 1);

    dec1.bits[3] = 0;
    // dec1 > dec2
    ck_assert_int_eq(is_greater(dec1, dec2), 0);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), 0);

    str_to_dec("10000000000000000000000000000000", &dec1, 0);

    str_to_dec("100000000000000000000000000000000", &dec2, 0);
    ck_assert_int_eq(is_greater(dec1, dec2), FALSE);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), FALSE);

    // new test
    str_to_dec(
        "1001111110010000011011001100101110000011110001011000110110100100000011"
        "110101000001000100100011",
        &dec1, 12);
    str_to_dec(
        "1001111110010000011011001100101110000011110001011000110110100100000011"
        "110101000001000100100011",
        &dec2, 12);

    ck_assert_int_eq(is_greater(dec1, dec2), FALSE);

    str_to_dec(
        "1001111110010000011011001100101110000011110001011000110110100100000011"
        "110101000001000100100011",
        &dec2, 11);
    ck_assert_int_eq(is_greater(dec1, dec2), FALSE);

    str_to_dec(
        "1001111110010000011011001100101110000011110001011000110110100100000011"
        "110101000001000100100011",
        &dec2, 13);
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);

    dec1.value_type == INFINITY;
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);

    dec2.value_type == NEGATIVE_INFINITY;
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);

    dec2.value_type == NORMAL_VALUE;
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);
    dec2.value_type == INFINITY;
    dec1.value_type == NORMAL_VALUE;
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);
}
END_TEST

START_TEST(lesser_funcs_test) {
    decimal dec1, dec2;
    init_dec_bits(&dec1);
    dec1.value_type = NORMAL_VALUE;
    init_dec_bits(&dec2);
    dec2.value_type = NORMAL_VALUE;
    dec1.bits[0] = 1325411;
    dec2.bits[0] = 1325411;
    dec1.bits[1] = 121;
    dec2.bits[1] = 121;
    dec1.bits[2] = 213;
    dec2.bits[2] = 213;

    // dec1 == dec2
    ck_assert_int_eq(is_less(dec1, dec2), 1);
    ck_assert_int_eq(is_less_or_equal(dec1, dec2), 0);

    dec2.bits[0] = 15;
    // dec1 > dec2
    ck_assert_int_eq(is_less(dec1, dec2), 1);
    ck_assert_int_eq(is_less_or_equal(dec1, dec2), 1);

    set_bit_dec(&dec1, 127, 1);
    // dec1 < dec2
    ck_assert_int_eq(is_less(dec1, dec2), 0);
    ck_assert_int_eq(is_less_or_equal(dec1, dec2), 0);

    set_bit_dec(&dec2, 127, 1);
    // dec1 < dec2
    ck_assert_int_eq(is_less(dec1, dec2), 0);
    ck_assert_int_eq(is_less_or_equal(dec1, dec2), 0);

    dec1.bits[3] = 0;
    // dec1 > dec2
    ck_assert_int_eq(is_less(dec1, dec2), 1);
    ck_assert_int_eq(is_less_or_equal(dec1, dec2), 1);
}
END_TEST

START_TEST(eq_funcs_test_float_point) {
    decimal dec1, dec2;
    // 5.201
    str_to_dec("1010001010001", &dec1, 3);
    // 5.201
    str_to_dec("1010001010001", &dec2, 3);
    ck_assert_int_eq(is_equal(dec1, dec2), 0);
    ck_assert_int_eq(is_not_equal(dec1, dec2), 1);
    // 5.201
    str_to_dec("1010001010001", &dec1, 3);
    // 52.01
    str_to_dec("1010001010001", &dec2, 2);
    ck_assert_int_eq(is_equal(dec1, dec2), 1);
    ck_assert_int_eq(is_not_equal(dec1, dec2), 0);

    // 174536166.053
    str_to_dec("10100010100011001010101000101010100101", &dec1, 3);
    // 174536.166053
    str_to_dec("10100010100011001010101000101010100101", &dec2, 6);
    ck_assert_int_eq(is_equal(dec1, dec2), 1);
    ck_assert_int_eq(is_not_equal(dec1, dec2), 0);

    // 174536.166053
    str_to_dec("10100010100011001010101000101010100101", &dec1, 6);
    // 174536.166053
    str_to_dec("10100010100011001010101000101010100101", &dec2, 6);
    ck_assert_int_eq(is_equal(dec1, dec2), 0);
    ck_assert_int_eq(is_not_equal(dec1, dec2), 1);

    // 0.00
    init_dec_bits(&dec1);
    str_to_dec("", &dec1, 2);
    // 0
    init_dec_bits(&dec2);
    str_to_dec("", &dec2, 0);
    ck_assert_int_eq(is_equal(dec1, dec2), 0);
    ck_assert_int_eq(is_not_equal(dec1, dec2), 1);
}
END_TEST

START_TEST(greater_test_float_point) {
    decimal dec1, dec2;
    // 5.201
    str_to_dec("1010001010001", &dec1, 3);
    // 5.201
    str_to_dec("1010001010001", &dec2, 3);
    ck_assert_int_eq(is_greater(dec1, dec2), 1);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), 0);
    // 5.201
    str_to_dec("1010001010001", &dec1, 3);
    // 52.01
    str_to_dec("1010001010001", &dec2, 2);
    ck_assert_int_eq(is_greater(dec1, dec2), FALSE);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), FALSE);

    // 52.01
    str_to_dec("1010001010001", &dec1, 2);
    // 5.201
    str_to_dec("1010001010001", &dec2, 3);
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), TRUE);

    // 174536166.053
    str_to_dec("10100010100011001010101000101010100101", &dec1, 3);
    // 174536.166053
    str_to_dec("10100010100011001010101000101010100101", &dec2, 6);
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), TRUE);

    // 174536.166053
    str_to_dec("10100010100011001010101000101010100101", &dec1, 6);
    // 174536.166053
    str_to_dec("10100010100011001010101000101010100101", &dec2, 6);
    ck_assert_int_eq(is_greater(dec1, dec2), FALSE);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), TRUE);

    // 563715.1
    str_to_dec("10101100000010000011111", &dec1, 1);
    // 174536.166053
    str_to_dec("1010", &dec2, 0);
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), TRUE);

    str_to_dec("1111", &dec1, 1);
    // 174536.166053
    str_to_dec("1", &dec2, 0);
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), TRUE);
}
END_TEST

START_TEST(int_to_dec_vv_test) {
    decimal dec0;
    int int0;
    float flt0;
    from_int_to_decimal(INT_TEST_0, &dec0);
    from_decimal_to_int(dec0, &int0);
    ck_assert_int_eq(int0, INT_TEST_0);
    from_int_to_decimal(INT_TEST_1, &dec0);
    from_decimal_to_int(dec0, &int0);
    ck_assert_int_eq(int0, INT_TEST_1);
    from_int_to_decimal(INT_TEST_2, &dec0);
    from_decimal_to_int(dec0, &int0);
    ck_assert_int_eq(int0, INT_TEST_2);
    from_int_to_decimal(INT_TEST_3, &dec0);
    from_decimal_to_int(dec0, &int0);
    ck_assert_int_eq(int0, INT_TEST_3);

    // float to int
    from_int_to_decimal(INT_TEST_0, &dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, (float)INT_TEST_0);
}
END_TEST

START_TEST(flt_to_dec_vv_test) {
    decimal dec0;
    int int0;
    float flt0;
    from_float_to_decimal(FLT_TEST_0, &dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, FLT_TEST_0);
    from_float_to_decimal(FLT_TEST_1, &dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, FLT_TEST_1);
    from_float_to_decimal(FLT_TEST_2, &dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, FLT_TEST_2);
    from_float_to_decimal(FLT_TEST_3, &dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, FLT_TEST_3);

    // int to float
    from_float_to_decimal(FLT_TEST_0, &dec0);
    from_decimal_to_int(dec0, &int0);
    ck_assert_int_eq(int0, (int)FLT_TEST_0);
}
END_TEST

START_TEST(truncate_test) {
    decimal dec0;
    int int0;
    float flt0;
    from_float_to_decimal(FLT_TEST_0, &dec0);
    dec0 = truncate(dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, truncf(FLT_TEST_0));
    from_float_to_decimal(FLT_TEST_1, &dec0);
    dec0 = truncate(dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, truncf(FLT_TEST_1));
    from_float_to_decimal(FLT_TEST_2, &dec0);
    dec0 = truncate(dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, truncf(FLT_TEST_2));
    from_float_to_decimal(FLT_TEST_3, &dec0);
    dec0 = truncate(dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, truncf(FLT_TEST_3));

    // try truncate int
}
END_TEST

START_TEST(truncate_common_values) {
    decimal dec, result, correct_result;
    init_dec_bits(&result);
    // 677
    str_to_dec("1010100101", &dec, 0);
    str_to_dec("1010100101", &correct_result, 0);
    result = truncate(dec);
    ck_assert_int_eq(is_equal(result, correct_result), TRUE);

    // 67.7
    str_to_dec("1010100101", &dec, 1);
    //   from_float_to_decimal(67.7, &dec);
    // 67
    str_to_dec("1000011", &correct_result, 0);
    result = truncate(dec);
    ck_assert_int_eq(is_equal(result, correct_result), TRUE);

    // -67.7
    str_to_dec("-1010100101", &dec, 1);
    // -67
    str_to_dec("-1000011", &correct_result, 0);
    result = truncate(dec);
    ck_assert_int_eq(is_equal(result, correct_result), TRUE);

    // 710579.877
    str_to_dec("101010010110101001011010100101", &dec, 3);
    // 710579
    str_to_dec("10101101011110110011", &correct_result, 0);
    result = truncate(dec);
    ck_assert_int_eq(is_equal(result, correct_result), TRUE);

    // 76297933.3938255525
    str_to_dec("101010010110101001011010100101101010010110101001011010100101",
               &dec, 10);
    // 76297933
    str_to_dec("100100011000011011011001101", &correct_result, 0);
    result = truncate(dec);
    ck_assert_int_eq(is_equal(result, correct_result), TRUE);

    // 819242.821697167591502157477
    str_to_dec(
        "101010010110101001011010100101101010010110101001011010100101101010"
        "0101"
        "10"
        "101001011010100101",
        &dec, 21);
    // 819242
    str_to_dec("11001000000000101010", &correct_result, 0);
    result = truncate(dec);
    ck_assert_int_eq(is_equal(result, correct_result), TRUE);

    // 819242821697167591502157477
    str_to_dec(
        "101010010110101001011010100101101010010110101001011010100101101010"
        "0101"
        "10"
        "101001011010100101",
        &dec, 0);
    // 819242821697167591502157477
    str_to_dec(
        "101010010110101001011010100101101010010110101001011010100101101010"
        "0101"
        "10"
        "101001011010100101",
        &correct_result, 0);
    result = truncate(dec);
    ck_assert_int_eq(is_equal(result, correct_result), TRUE);

    // 0.710579877
    str_to_dec("101010010110101001011010100101", &dec, 9);
    // 0
    init_dec_bits(&correct_result);
    result = truncate(dec);
    ck_assert_int_eq(is_equal(result, correct_result), TRUE);
}

START_TEST(floor_dec) {
    decimal dec, correct_result, res;
    init_dec_bits(&dec), init_dec_bits(&res), init_dec_bits(&correct_result);

    // check 0 -> 0
    str_to_dec("0", &dec, 0);
    str_to_dec("0", &correct_result, 0);
    res = floor(dec);
    ck_assert_int_eq(is_equal(res, correct_result), TRUE);
    init_dec_bits(&res);

    // check 1 -> 1
    str_to_dec("1", &dec, 0);
    str_to_dec("1", &correct_result, 0);
    res = floor(dec);
    ck_assert_int_eq(is_equal(res, correct_result), TRUE);
    init_dec_bits(&res);

    // check 10.1 -> 10
    str_to_dec("1100101", &dec, 1);
    str_to_dec("1010", &correct_result, 0);
    res = floor(dec);
    ck_assert_int_eq(is_equal(res, correct_result), TRUE);
    init_dec_bits(&res);

    // check 10.5 -> 10
    str_to_dec("1101001", &dec, 1);
    str_to_dec("1010", &correct_result, 0);
    res = floor(dec);
    ck_assert_int_eq(is_equal(res, correct_result), TRUE);

    // check -10.1 -> -11
    str_to_dec("-1100101", &dec, 1);
    str_to_dec("-1011", &correct_result, 0);
    res = floor(dec);
    ck_assert_int_eq(is_equal(res, correct_result), TRUE);
    init_dec_bits(&res);

    // check -10.5 -> -11
    str_to_dec("-1101001", &dec, 1);
    str_to_dec("-1011", &correct_result, 0);
    res = floor(dec);
    ck_assert_int_eq(is_equal(res, correct_result), TRUE);

    // check max 7922816251426433759354395033.5 ->
    // 7922816251426433759354395033
    str_to_dec(
        "111111111111111111111111111111111111111111111111111111111111111111"
        "1111"
        "11111111111111111111111111",
        &dec, 1);
    str_to_dec(
        "110011001100110011001100110011001100110011001100110011001100110011"
        "0011"
        "00110011001100110011001",
        &correct_result, 0);
    res = floor(dec);
    ck_assert_int_eq(is_equal(res, correct_result), TRUE);

    // check min -7922816251426433759354395033.5 ->
    // -7922816251426433759354395034
    str_to_dec(
        "-11111111111111111111111111111111111111111111111111111111111111111"
        "1111"
        "111111111111111111111111111",
        &dec, 1);
    str_to_dec(
        "-11001100110011001100110011001100110011001100110011001100110011001"
        "1001"
        "100110011001100110011010",
        &correct_result, 0);
    res = floor(dec);
    ck_assert_int_eq(is_equal(res, correct_result), TRUE);
}
END_TEST

START_TEST(floor_test) {
    decimal dec0;
    int int0;
    float flt0;
    from_float_to_decimal(FLT_TEST_0, &dec0);
    dec0 = floor(dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, floorf(FLT_TEST_0));
    from_float_to_decimal(FLT_TEST_1, &dec0);
    dec0 = floor(dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, floorf(FLT_TEST_1));
    from_float_to_decimal(FLT_TEST_2, &dec0);
    dec0 = floor(dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, floorf(FLT_TEST_2));
    from_float_to_decimal(FLT_TEST_3, &dec0);
    dec0 = floor(dec0);
    from_decimal_to_float(dec0, &flt0);
    ck_assert_float_eq(flt0, floorf(FLT_TEST_3));

    // try floor int
}
END_TEST

START_TEST(add_1) {
    decimal src1, src2;
    int a = -1234;
    float b = 1.234;
    float res_our_dec = 0.0;
    from_int_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = -1232.766;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(add_2) {
    decimal src1, src2;
    int a = -1234;
    float b = -1.234;
    float res_our_dec = 0.0;
    from_int_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = -1235.234;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(add_3) {
    decimal src1, src2;
    int a = -46;
    float b = 0.0 / 0.0;
    float res_our_dec = 0.0;
    from_int_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
}
END_TEST

START_TEST(add_4) {
    decimal src1, src2;
    float a = -1234;
    float b = -221.234;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = -1455.234;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq_tol(res_our_dec, res_origin, 0.01);
}
END_TEST

START_TEST(add_5) {
    decimal src1, src2;
    int a = -1234;
    float b = -221.234;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = -1455.234;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq_tol(res_our_dec, res_origin, 0.01);
}
END_TEST

START_TEST(add_6) {
    decimal src1, src2;
    float a = -1.0 / 0.0;
    int b = 202;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_int_to_decimal(b, &src2);
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    // ck_assert_float_eq(res_od.value_type, NEGATIVE_INFINITY);
    // ck_assert_int_eq(res_od.bits[3], 0);
    // ck_assert_int_eq(res_od.bits[2], 0);
    // ck_assert_int_eq(res_od.bits[1], 0);
    // ck_assert_int_eq(res_od.bits[0], 0);
}
END_TEST

START_TEST(add_7) {
    decimal src1, src2;
    float a = -9403;
    float b = 0.00234;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = -9402.99766;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq_tol(res_our_dec, res_origin, 0001);
}
END_TEST

START_TEST(add_8) {
    decimal src1, src2;
    int a = -9403;
    int b = 234;
    int res_our_dec = 0;
    from_int_to_decimal(a, &src1);
    from_int_to_decimal(b, &src2);
    int res_origin = -9169;
    decimal res_od = add(src1, src2);
    from_decimal_to_int(res_od, &res_our_dec);
    // ck_assert_int_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(add_9) {
    decimal src1, src2;
    float a = -940.3;
    float b = 0.000234;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = -940.299766;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq_tol(res_our_dec, res_origin, 0.01);
}
END_TEST

START_TEST(add_10) {
    decimal src1, src2;
    float a = -0.9403;
    float b = 0.000234;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = -0.940066;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq_tol(res_our_dec, res_origin, 0.01);
}
END_TEST

START_TEST(add_11) {
    decimal src1, src2;
    float a = -0.9403;
    float b = 2.0234;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = 1.0831;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(add_12) {
    decimal src1, src2;
    float a = 1.0 / 0.0;
    float b = 112.0234;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    decimal res_od;
    res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_od.value_type, INFINITY);
    ck_assert_int_eq(res_od.bits[3], 0);
    // ck_assert_int_eq(res_od.bits[2], 0);
    // ck_assert_int_eq(res_od.bits[1], 0);
    // ck_assert_int_eq(res_od.bits[0], 0);
}
END_TEST

START_TEST(add_13) {
    decimal src1, src2;
    float a = -0.9403;
    float b = -112.0234;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = -112.9637;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(add_14) {
    decimal src1, src2;
    float a = -0.94e03;
    float b = -112.0234;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = -1052.0234;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq_tol(res_our_dec, res_origin, 0.01);
}
END_TEST

START_TEST(add_15) {
    decimal src1, src2;
    float a = -0.94e03;
    float b = -112.0e2;
    float res_our_dec = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    float res_origin = -12140;
    decimal res_od = add(src1, src2);
    from_decimal_to_float(res_od, &res_our_dec);
    ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(add_16) {
    decimal src1, src2;
    int a = 9403;
    int b = 202;
    int res_origin = 9605;
    int res_our_dec = 0;
    from_int_to_decimal(a, &src1);
    from_int_to_decimal(b, &src2);
    decimal res_od = add(src1, src2);
    from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(add_17) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0x300000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xF;
    src1.bits[0] = 0x67E4FEEF;
    src2.bits[3] = 0x300000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFF;
    src2.bits[0] = 0x67E4FFFF;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0x300000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x10E;
    result_origin.bits[0] = 0xCFC9FEEE;
    decimal result_our = add(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST
START_TEST(add_18) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0x30000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFF;
    src1.bits[0] = 0x67E4F;
    src2.bits[3] = 0x60000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xEA;
    src2.bits[0] = 0x67E4F;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0x60000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x3E502;
    result_origin.bits[0] = 0x1963E2E7;
    decimal result_our = add(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST

START_TEST(add_19) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0x30000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x80070000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0x70000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x270FEFED;
    result_origin.bits[0] = 0x00F26FA1;
    decimal result_our = add(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST

START_TEST(add_20) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0x80030000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x80070000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0x80070000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x27100FEC;
    result_origin.bits[0] = 0xFFF96C3F;
    decimal result_our = add(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST

START_TEST(add_21) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0x80030000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x70000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0x80070000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x270FEFED;
    result_origin.bits[0] = 0x00F26FA1;
    decimal result_our = add(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST

START_TEST(add_norm) {
    decimal dec1, dec2, result, correct_result;
    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    str_to_dec("11001010", &dec1, 0);              // 202
    str_to_dec("110011010", &dec2, 0);             // 410
    str_to_dec("1001100100", &correct_result, 0);  // 612
    result = add(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    str_to_dec("11111111111111111111111111101011", &dec1, 0);  // 4294967275
    str_to_dec("10101001011100001", &dec2, 0);                 // 86753
    str_to_dec("100000000000000010101001011001100", &correct_result,
               0);  // 4295054028
    result = add(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    str_to_dec("1111", &dec1, 0);                          // 15
    str_to_dec("-11000011010100000", &dec2, 0);            // 100000
    str_to_dec("-11000011010010001", &correct_result, 0);  // -99985
    result = add(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    str_to_dec("-11111111111111111111111111101011", &dec1,
               0);                               // 4294967275
    str_to_dec("-10101001011100001", &dec2, 0);  // 86753
    str_to_dec("-100000000000000010101001011001100", &correct_result,
               0);  // 4295054028
    result = add(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    // 123757,6213581
    str_to_dec("10010000000100101010010100110010001001101", &dec1, 7);
    // 0,0000020
    str_to_dec("10100", &dec2, 7);
    // 123757,6213601
    str_to_dec("10010000000100101010010100110010001100001", &correct_result, 7);
    result = add(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    // 123757,6213581
    str_to_dec("10010000000100101010010100110010001001101", &dec1, 7);
    // 1,7
    str_to_dec("10001", &dec2, 1);
    // 123759.3213581
    str_to_dec("10010000000100110010011011100101010001101", &correct_result, 7);
    result = add(dec1, dec2);

    ck_assert_int_eq(is_equal(correct_result, result), 0);

    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    // -123757,6213581
    str_to_dec("-10010000000100101010010100110010001001101", &dec1, 7);
    // -1,7
    str_to_dec("-10001", &dec2, 1);
    // -123759,3213581
    str_to_dec("-10010000000100110010011011100101010001101", &correct_result,
               7);

    result = add(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    // 0.0000000000000000000000000001
    str_to_dec("1", &dec1, 28);
    //  1
    str_to_dec("1", &dec2, 0);
    // 1.0000000000000000000000000001
    str_to_dec(
        "100000010011111100111001011110001111100010010100000010011000010001"
        "0000"
        "000000000000000000000001",
        &correct_result, 28);
    result = add(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    // 0.0000000000000000000000000001
    str_to_dec("1", &dec1, 28);
    //  1
    str_to_dec("1", &dec2, 0);
    // 1.0000000000000000000000000001
    str_to_dec(
        "100000010011111100111001011110001111100010010100000010011000010001"
        "0000"
        "000000000000000000000001",
        &correct_result, 28);
    result = add(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    // 1.23456789123456789
    str_to_dec("110110110100110110100101110101100110100000101111100010101",
               &dec1, 17);
    //  12345678912345678.9
    str_to_dec("110110110100110110100101110101100110100000101111100010101",
               &dec2, 1);
    // 12345678912345680.134567891235
    str_to_dec(
        "100111111001000001101100110010111000001111000101100011011010010000"
        "0011"
        "11"
        "0101000001000100100010",
        &correct_result, 12);
    result = add(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    // #1
    // 112286.36363636363636363
    str_to_dec(
        "1001100000101101001010001000000000110111001011001101011"
        "1100010111010001011",
        &dec1, 17);
    // 4.16787847
    str_to_dec("11000110101111010110110000111", &dec2, 8);

    result = add(dec1, dec2);

    // 112290.53151483363636363
    str_to_dec(
        "100110000010111010011010101011101100110011011000101000000010110100"
        "1000"
        "10"
        "11",
        &correct_result, 17);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #2
    // 135521
    str_to_dec("100001000101100001", &dec1, 0);
    // 3724292940501892409235
    str_to_dec(
        "11001001111001001111001001111001001111001001111001"
        "0011110010011110010011",
        &dec2, 0);

    result = add(dec1, dec2);

    // 3724292940501892544756
    str_to_dec(
        "110010011110010011110010011110010011110010011110010100010011100011"
        "1101"
        "0"
        "0",
        &correct_result, 0);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #3
    // 0.0135521
    str_to_dec("100001000101100001", &dec1, 7);
    // 0.0000003724292940501892409235
    str_to_dec(
        "1100100111100100111100100111100100111100100111"
        "10010011110010011110010011",
        &dec2, 28);

    result = add(dec1, dec2);

    // 0.013 55247 24292 94050 18924 09235
    str_to_dec(
        "111000000011010011110100000011001110011001100100011111010011000111"
        "0111"
        "10"
        "010011110010011",
        &correct_result, 28);
    // print_dec(result);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #4
    // 24560730379421944653998624.602
    str_to_dec(
        "100111101011100001010001111010111000010100011110101110"
        "00010100011110101110000101000111101011010",
        &dec1, 3);
    // 31.7
    str_to_dec("100111101", &dec2, 1);

    result = add(dec1, dec2);

    // 24560730379421944653998656.302
    str_to_dec(
        "100111101011100001010001111010111000010100011110101110000101000111"
        "1010"
        "11"
        "10000110000101100101110",
        &correct_result, 3);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #5
    // 0.0000000000000000009
    str_to_dec("1001", &dec1, 19);
    // 0,000000000000000000000009
    str_to_dec("1001", &dec2, 24);

    result = add(dec1, dec2);

    // 0.0000 00000 00000 00009 00009
    str_to_dec("11011011101110101001", &correct_result, 24);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #6
    // 0,0000000000000000000000095642
    str_to_dec("10111010110011010", &dec1, 28);
    // 0,0000000000000000000000112
    str_to_dec("1110000", &dec2, 25);

    result = add(dec1, dec2);

    // 0.000 00000 00000 00000 00002 07642
    str_to_dec("110010101100011010", &correct_result, 28);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #7
    // 0,0000000000000000000000095642
    str_to_dec("10111010110011010", &dec1, 28);
    // 112000
    str_to_dec("11011010110000000", &dec2, 0);

    result = add(dec1, dec2);

    // 112000.000 00000 00000 00000 00001
    str_to_dec(
        "100100001100000110110001000000100101111000010110011100010000111100"
        "0000"
        "00"
        "0000000000000000000000",
        &correct_result, 23);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #8
    // 0,0000000000000000000000095642
    str_to_dec("10111010110011010", &dec1, 28);
    // 0.0000000000000000000000000002
    str_to_dec("10", &dec2, 28);

    result = add(dec1, dec2);

    // 0.0000000000000000000000095644
    str_to_dec("10111010110011100", &correct_result, 28);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #9
    // 0,0000000000000000000000095642
    str_to_dec("10111010110011010", &dec1, 28);
    // 0.8539464285714285714285714286
    str_to_dec(
        "1101110010111101011011110011010000101100011010111110111"
        "11000010110011011101101101101101101110",
        &dec2, 28);

    result = add(dec1, dec2);

    // 0.8539464285714285714285809928
    str_to_dec(
        "110111001011110101101111001101000010110001101011111011111000010110"
        "0110"
        "11"
        "110000101000100001000",
        &correct_result, 28);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #10
    // 24560730379421944653998624.602
    str_to_dec(
        "100111101011100001010001111010111000010100011110101110000101000111"
        "1010"
        "11"
        "10000101000111101011010",
        &dec1, 3);
    // 792281625142643375935439.50329
    str_to_dec(
        "111111111111111111111111111111111111111111111111111111111111111111"
        "1111"
        "11"
        "111111111111111111111001",
        &dec2, 5);

    result = add(dec1, dec2);

    // C #        25353012004564588029934064.105
    // full prec 25353012004564588029934064.10529
    // res       25353012004564588029934064.106
    str_to_dec(
        "101000111101011100001010001111010111000010100011110101110000101000"
        "1111"
        "01"
        "01110000101000111101001",
        &correct_result, 3);
    // ck_assert_int_eq(is_equal(correct_result, result), TRUE);
}
END_TEST

START_TEST(special_values) {
    decimal dec1, dec2, result, correct_result;
    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);

    dec1.value_type = INFINITY;
    // 135521
    str_to_dec("11111111111111111111111111111111", &dec2, 0);

    result = add(dec1, dec2);
    ck_assert_int_eq(result.value_type, INFINITY);
    init_dec_bits(&result);

    dec1.value_type = NEGATIVE_INFINITY;
    // 135521
    str_to_dec("11111111111111111111111111111111", &dec2, 0);
    correct_result.value_type = 2;
    result = add(dec1, dec2);
    ck_assert_int_eq(result.value_type, NEGATIVE_INFINITY);
}
END_TEST

START_TEST(sub_9) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0b00000000000000010000000000000000;
    src1.bits[2] = 0;
    src1.bits[1] = 0;
    src1.bits[0] = 0b00000000000000000110011011110000;
    src2.bits[3] = 0b00000000000000110000000000000000;
    src2.bits[2] = 0;
    src2.bits[1] = 0;
    src2.bits[0] = 0b00000000000001000101000111010110;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0b00000000000000110000000000000000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0;
    result_origin.bits[0] = 0b00000000001000111110001111101010;
    decimal result_our = sub(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST
START_TEST(sub_10) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0b00000000000000110000000000000000;
    src1.bits[2] = 0;
    src1.bits[1] = 0;
    src1.bits[0] = 0b00000000000001000101000111010110;
    src2.bits[3] = 0b00000000000000010000000000000000;
    src2.bits[2] = 0;
    src2.bits[1] = 0;
    src2.bits[0] = 0b00000000000000000000101110111100;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0b10000000000000110000000000000000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0;
    result_origin.bits[0] = 0b00000000000000000100001110011010;
    decimal result_our = sub(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST
START_TEST(sub_11) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0x80030000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x80070000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0x80070000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x270FEFED;
    result_origin.bits[0] = 0x00F26FA1;
    decimal result_our = sub(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST
START_TEST(sub_12) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0x30000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x80070000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0x70000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x27100FEC;
    result_origin.bits[0] = 0xFFF96C3F;
    decimal result_our = sub(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST
START_TEST(sub_13) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0b10000000000000010000000000000000;
    src1.bits[2] = 0;
    src1.bits[1] = 0;
    src1.bits[0] = 0b00000000000000000000000100110000;
    src2.bits[3] = 0b10000000000001100000000000000000;
    src2.bits[2] = 0;
    src2.bits[1] = 0;
    src2.bits[0] = 0b00010001001000010001101001100000;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0b00000000000001100000000000000000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0;
    result_origin.bits[0] = 0b00001111010100010011110001100000;
    decimal result_our = sub(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST
START_TEST(sub_14) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[3] = 0x80030000;
    src1.bits[2] = 0;
    src1.bits[1] = 0xFFFF;
    src1.bits[0] = 0xFF837E4F;
    src2.bits[3] = 0x70000;
    src2.bits[2] = 0;
    src2.bits[1] = 0xFFF;
    src2.bits[0] = 0xFF837E4F;
    decimal result_origin;
    result_origin.value_type = NORMAL_VALUE;
    result_origin.bits[3] = 0x80070000;
    result_origin.bits[2] = 0;
    result_origin.bits[1] = 0x27100FEC;
    result_origin.bits[0] = 0xFFF96C3F;
    decimal result_our = sub(src1, src2);
    ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
}
END_TEST
START_TEST(sub_15) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[0] = 0b00000000000000000000011111100100;
    src1.bits[1] = 0b00000000000000000000000000000000;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b00000000000000000000000000000000;
    src2.bits[0] = 0b00000101100110111101101000011001;
    src2.bits[1] = 0b00000000000000000000000000000000;
    src2.bits[2] = 0b00000000000000000000000000000000;
    src2.bits[3] = 0b00000000000000000000000000000000;
    decimal result, result_our;
    result_our = sub(src2, src1);
    result.value_type = NORMAL_VALUE;
    result.bits[0] = 0b00000101100110111101001000110101;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b00000000000000000000000000000000;
    ck_assert_int_eq(result.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result.bits[0], result_our.bits[0]);
}
END_TEST
START_TEST(sub_16) {
    decimal src1, src2;
    src1.value_type = src2.value_type = NORMAL_VALUE;
    src1.bits[0] = 0b00000000000000000000011111100100;
    src1.bits[1] = 0b00000000000000000000000000000000;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b10000000000000000000000000000000;
    src2.bits[0] = 0b00000000000000000010010010111011;
    src2.bits[1] = 0b00000000000000000000000000000000;
    src2.bits[2] = 0b00000000000000000000000000000000;
    src2.bits[3] = 0b10000000000000000000000000000000;
    decimal result, result_our;
    result_our = sub(src2, src1);
    result.value_type = NORMAL_VALUE;
    result.bits[0] = 0b00000000000000000001110011010111;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b10000000000000000000000000000000;
    ck_assert_int_eq(result.bits[3], result_our.bits[3]);
    ck_assert_int_eq(result.bits[2], result_our.bits[2]);
    ck_assert_int_eq(result.bits[1], result_our.bits[1]);
    ck_assert_int_eq(result.bits[0], result_our.bits[0]);
}
END_TEST

START_TEST(sub_normal_test_integer) {
    decimal dec1, dec2, result, correct_result;
    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);
    str_to_dec("11001010", &dec1, 0);
    str_to_dec("11001010", &dec2, 0);
    result = sub(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    str_to_dec("11000011010100000", &dec1, 0);            // 100000
    str_to_dec("1111", &dec2, 0);                         // 15
    str_to_dec("11000011010010001", &correct_result, 0);  // 99985
    result = sub(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    str_to_dec("1111", &dec1, 0);                          // 15
    str_to_dec("11000011010100000", &dec2, 0);             // 100000
    str_to_dec("-11000011010010001", &correct_result, 0);  // -99985
    result = sub(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    // 1237576213581
    str_to_dec("10010000000100101010010100110010001001101", &dec1, 0);
    // 1
    str_to_dec("1", &dec2, 0);
    // 1237576213580
    str_to_dec("10010000000100101010010100110010001001100", &correct_result, 0);
    result = sub(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    str_to_dec("-1111", &dec1, 0);                        // -15
    str_to_dec("-11000011010100000", &dec2, 0);           // -100000
    str_to_dec("11000011010010001", &correct_result, 0);  // 99985
    result = sub(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    // -0.2
    str_to_dec("-10", &dec1, 1);
    // -0.0000001
    str_to_dec("-1", &dec2, 7);
    // -0.1999999
    str_to_dec("-111101000010001111111", &correct_result, 7);
    result = sub(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    str_to_dec("11010000000100101010010100110010001001101", &dec1, 0);
    // 1
    str_to_dec("1010000000100101010010100110010001001100", &dec2, 0);
    // 1237576213580
    str_to_dec("10000000000000000000000000000000000000001", &correct_result, 0);
    result = sub(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);
    // 49155195229197460557
    str_to_dec(
        "10101010100010101001010101101000000010010101001010011001000100110"
        "1",
        &dec1, 0);
    // 687820399692
    str_to_dec("1010000000100101010010100110010001001100", &dec2, 0);
    // 49155194541377060865
    str_to_dec(
        "10101010100010101001010101000000000000000000000000000000000000000"
        "1",
        &correct_result, 0);
    result = sub(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    str_to_dec("101010100101010101111010101001010", &dec1, 0);
    // 1
    str_to_dec("11001000000000000000000000000000", &dec2, 0);
    // 1237576213580
    str_to_dec("10001100101010101111010101001010", &correct_result, 0);
    result = sub(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);
}
END_TEST

START_TEST(sub_uncommon_values) {
    decimal dec1, dec2, result, correct_result;
    // 79228162514264337593543950330 - 49517601571415210995964968960
    str_to_dec(
        "111111111111111111111111111111111111111111111111111111111111111111"
        "1111"
        "11"
        "111111111111111111111010",
        &dec1, 0);
    str_to_dec(
        "101000000000000000000000000000000000000000000000000000000000000000"
        "0000"
        "00"
        "000000000000000000000000",
        &dec2, 0);
    init_dec_bits(&result);
    result = sub(dec1, dec2);
    // 29710560942849126597578981370
    str_to_dec(
        "101111111111111111111111111111111111111111111111111111111111111111"
        "1111"
        "11"
        "11111111111111111111010",
        &correct_result, 0);
    ck_assert_int_eq(is_equal(correct_result, result), 0);
}

START_TEST(div_common_values) {
    decimal dec1, dec2, result, correct_result;
    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);

    str_to_dec("11001010", &dec1, 0);
    str_to_dec("11001010", &dec2, 0);
    result = div(dec1, dec2);
    str_to_dec("1", &correct_result, 0);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    // 4
    str_to_dec("100", &dec1, 0);
    // 3
    str_to_dec("11", &dec2, 0);
    result = div(dec1, dec2);
    // 1.3333333333333333333333333333
    str_to_dec(
        "1010110001010100010011001010000101001011011100000000110010110000010101"
        "01"
        "0101010101010101010101",
        &correct_result, 28);
    ck_assert_int_eq(is_equal(correct_result, result), 0);
}
END_TEST

START_TEST(mod_common_values) {
    decimal dec1, dec2, result, correct_result;
    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);

    // 202
    str_to_dec("11001010", &dec1, 0);
    // 202
    str_to_dec("11001010", &dec2, 0);
    // 0
    init_dec_bits(&result);
    result = mod(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);
    // 203
    str_to_dec("11001011", &dec1, 0);
    // 202
    str_to_dec("11001010", &dec2, 0);
    // 1
    str_to_dec("1", &correct_result, 0);
    result = mod(dec1, dec2);

    ck_assert_int_eq(is_equal(correct_result, result), 0);

    // 403

    str_to_dec("110010011", &dec1, 0);

    // 202
    str_to_dec("11001010", &dec2, 0);
    // 201
    str_to_dec("11001001", &correct_result, 0);
    result = mod(dec1, dec2);

    ck_assert_int_eq(is_equal(correct_result, result), 0);

    // 54195595155
    str_to_dec("110010011110010011110010011110010011", &dec1, 0);
    // 10
    str_to_dec("1010", &dec2, 0);
    // 5
    str_to_dec("101", &correct_result, 0);
    result = mod(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    // 54195595155
    str_to_dec("110010011110010011110010011110010011", &dec1, 0);
    // 1000
    str_to_dec("1111101000", &dec2, 0);
    // 155
    str_to_dec("10011011", &correct_result, 0);
    result = mod(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    // 3724292940501892409235
    str_to_dec(
        "1100100111100100111100100111100100111100100111100100111100100111100100"
        "1"
        "1",
        &dec1, 0);
    // 135521
    str_to_dec("100001000101100001", &dec2, 0);
    // 100360
    str_to_dec("11000100000001000", &correct_result, 0);
    result = mod(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    // 3905204194379712334906604703
    str_to_dec(
        "1100100111100100111100100111100100111100100111100100111100100111100100"
        "11"
        "10010011110010011111",
        &dec1, 0);
    // 135521
    str_to_dec("100001000101100001", &dec2, 0);
    // 112657
    str_to_dec("11011100000010001", &correct_result, 0);
    result = mod(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);
}
END_TEST

START_TEST(mod_uncommon_values) {
    decimal dec1, dec2, result, correct_result;
    init_dec_bits(&correct_result);
    init_dec_bits(&result);
    init_dec_bits(&dec1);
    init_dec_bits(&dec2);

    // 3905204194379712334906604703
    str_to_dec("111111111111111111111111111111111", &dec1, 0);
    // 135521
    str_to_dec("11111111111111111111111111111111", &dec2, 0);
    // 112657
    str_to_dec("1", &correct_result, 0);
    result = mod(dec1, dec2);
    ck_assert_int_eq(is_equal(correct_result, result), 0);

    // 792281625
    str_to_dec("101111001110010100001000011001", &dec1, 0);
    // 4.951760157141521
    str_to_dec("10001100101111001100110000001001011011110101000010001", &dec2,
               15);
    result = mod(dec1, dec2);
    // 4.809116797141521
    str_to_dec("10001000101011101110111000010011000001000001000010001",
               &correct_result, 15);

    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // 54375684.5645934765284
    str_to_dec(
        "111010111101000100100011000100010101011110000110110100010010011100100",
        &dec1, 13);
    // 4.951760157141521
    str_to_dec("10001100101111001100110000001001011011110101000010001", &dec2,
               15);
    result = mod(dec1, dec2);

    // 0.2346895488226780
    // 0.234689548822678

    str_to_dec("110101010111001011101011001100001010100010010110",
               &correct_result, 15);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // 54375684.5645934765284
    str_to_dec(
        "111010111101000100100011000100010101011110000110110100010010011100100",
        &dec1, 13);
    // 11.9888888888888888
    str_to_dec("110101001111011100100111100101010101111010000111000111000",
               &dec2, 16);
    result = mod(dec1, dec2);
    // 7.0757045880426672
    str_to_dec("11111011011000010010100010001001000011110001000010110000",
               &correct_result, 16);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // 99999,99999999999999999999999
    str_to_dec(
        "1000000100111111001110010111100011111000100101000"
        "000100110000100001111111111111111111111111111",
        &dec1, 23);
    // 0,02
    str_to_dec("10", &dec2, 2);
    result = mod(dec1, dec2);
    // 0.01999999999999999999999

    str_to_dec(
        "1101100011010111001001101011011100010111011110100111111111111111111111"
        "1",
        &correct_result, 23);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // 99999,99999999999999999999999
    str_to_dec("1001", &dec1, 23);
    // 0,02
    str_to_dec("10", &dec2, 2);
    result = mod(dec1, dec2);
    // 0.01999999999999999999999

    str_to_dec("1001", &correct_result, 23);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #1
    // 112286.36363636363636363
    str_to_dec(
        "1001100000101101001010001000000000110111001011001101011"
        "1100010111010001011",
        &dec1, 17);
    // 4.16787847
    str_to_dec("11000110101111010110110000111", &dec2, 8);

    result = mod(dec1, dec2);

    // 3.71765456363636363
    str_to_dec("10100101000110001101011011001111110010100001100011010001011",
               &correct_result, 17);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #2
    // 3724292940501892409235
    str_to_dec(
        "11001001111001001111001001111001001111001001111001"
        "0011110010011110010011",
        &dec1, 0);
    // 135521
    str_to_dec("100001000101100001", &dec2, 0);

    result = mod(dec1, dec2);

    // 100360
    str_to_dec("11000100000001000", &correct_result, 0);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #3
    // 0.0135521
    str_to_dec("100001000101100001", &dec1, 7);
    // 0.0000003724292940501892409235
    str_to_dec(
        "1100100111100100111100100111100100111100100111"
        "10010011110010011110010011",
        &dec2, 28);

    result = mod(dec1, dec2);

    // 0.0000001428481017139012756820
    str_to_dec(
        "1001101011100000010100000001100101010111100111110001010111001010101010"
        "0",
        &correct_result, 28);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #4
    // 24560730379421944653998624.602
    str_to_dec(
        "100111101011100001010001111010111000010100011110101110"
        "00010100011110101110000101000111101011010",
        &dec1, 3);
    // 31.7
    str_to_dec("100111101", &dec2, 1);

    result = mod(dec1, dec2);

    // 9.402
    str_to_dec("10010010111010", &correct_result, 3);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #5
    // 0.0000000000000000009
    str_to_dec("1001", &dec1, 19);
    // 0,000000000000000000000009
    str_to_dec("1001", &dec2, 24);

    result = mod(dec1, dec2);

    // 0
    str_to_dec("0", &correct_result, 0);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #6
    // 0,0000000000000000000000095642
    str_to_dec("10111010110011010", &dec1, 28);
    // 0,0000000000000000000000112
    str_to_dec("1110000", &dec2, 25);

    result = mod(dec1, dec2);

    // 0,0000000000000000000000095642
    str_to_dec("10111010110011010", &correct_result, 28);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #9
    // 0,0000000000000000000000095642
    str_to_dec("10111010110011010", &dec1, 28);
    // 0.0000000000000000000000000002
    str_to_dec("10", &dec2, 28);

    result = mod(dec1, dec2);

    // 0
    str_to_dec("0", &correct_result, 0);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #10
    // 0.8539464285714285714285714286
    str_to_dec(
        "1101110010111101011011110011010000101100011010111110111"
        "11000010110011011101101101101101101110",
        &dec1, 28);
    // 0,0000000000000000000000095642
    str_to_dec("10111010110011010", &dec2, 28);

    result = mod(dec1, dec2);

    // 0.0000000000000000000000068316
    str_to_dec("10000101011011100", &correct_result, 28);
    // print_dec(result);
    ck_assert_int_eq(is_equal(correct_result, result), TRUE);

    // #11
    // 24560730379421944653998624.602
    str_to_dec(
        "100111101011100001010001111010111000010100011110101110"
        "00010100011110101110000101000111101011010",
        &dec1, 3);
    // 792281625142643375935439.50329
    str_to_dec(
        "1111111111111111111111111111111111111111111111111111"
        "11111111111111111111111111111111111111111001",
        &dec2, 5);

    result = mod(dec1, dec2);

    // 0.00001
    str_to_dec("1", &correct_result, 5);
    // print_dec(result);
    //  ck_assert_int_eq(is_equal(correct_result, result), TRUE);
}
END_TEST

START_TEST(mod_1) {
    decimal src1, src2, res_mod;
    int a = 3;
    int b = 2;
    int res_origin = a % b;
    int res = 0;
    from_int_to_decimal(a, &src1);
    from_int_to_decimal(b, &src2);
    res_mod = mod(src1, src2);
    from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST
START_TEST(mod_2) {
    decimal src1, src2, res_mod;
    int a = -98765;
    int b = 1234;
    int res_origin = a % b;
    int res = 0;
    from_int_to_decimal(a, &src1);
    from_int_to_decimal(b, &src2);
    res_mod = mod(src1, src2);
    from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST
START_TEST(mod_3) {
    decimal src1, src2, res_mod;
    int a = 30198;
    int b = 20210;
    int res_origin = a % b;
    int res = 0;
    from_int_to_decimal(a, &src1);
    from_int_to_decimal(b, &src2);
    res_mod = mod(src1, src2);
    from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST
START_TEST(mod_4) {
    decimal src1, src2, res_mod;
    int a = -98765;
    int b = -1234;
    int res_origin = a % b;
    int res = 0;
    from_int_to_decimal(a, &src1);
    from_int_to_decimal(b, &src2);
    res_mod = mod(src1, src2);
    from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST
START_TEST(mod_5) {
    decimal src1, src2, res_mod;
    int a = 98765;
    int b = 127234;
    int res_origin = a % b;
    int res = 0;
    from_int_to_decimal(a, &src1);
    from_int_to_decimal(b, &src2);
    res_mod = mod(src1, src2);
    from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST
START_TEST(mod_6) {
    decimal src1, src2, res_mod;
    int a = 342576;
    int b = 1542134;
    int res_origin = a % b;
    int res = 0;
    from_int_to_decimal(a, &src1);
    from_int_to_decimal(b, &src2);
    res_mod = mod(src1, src2);
    from_decimal_to_int(res_mod, &res);
    ck_assert_int_eq(res_origin, res);
}
END_TEST
START_TEST(mod_7) {
    decimal src1, src2, res_mod;
    float a = 1.2;
    float b = 0.3;
    float res_origin = fmod(a, b);
    float res = 0.0;
    from_float_to_decimal(a, &src1);
    from_float_to_decimal(b, &src2);
    res_mod = mod(src1, src2);
    from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);
}
END_TEST
START_TEST(mod_8) {
    decimal src1, src2, res_mod;
    float a = 1.2;
    int b = 3;
    float res_origin = fmod(a, b);
    float res = 0;
    from_float_to_decimal(a, &src1);
    from_int_to_decimal(b, &src2);
    res_mod = mod(src1, src2);
    from_decimal_to_float(res_mod, &res);
    ck_assert_float_eq(res_origin, res);
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    srunner_set_fork_status(sr, CK_NOFORK);

    suite_add_tcase(s1, tc1_1);

    tcase_add_test(tc1_1, from_int_to_decimal_1);

    tcase_add_test(tc1_1, from_float_to_decimal_1);
    tcase_add_test(tc1_1, from_float_to_decimal_2);
    tcase_add_test(tc1_1, from_float_to_decimal_3);
    tcase_add_test(tc1_1, from_float_to_decimal_4);
    tcase_add_test(tc1_1, from_float_to_decimal_5);
    tcase_add_test(tc1_1, from_float_to_decimal_6);
    tcase_add_test(tc1_1, from_float_to_decimal_7);
    tcase_add_test(tc1_1, from_float_to_decimal_8);
    tcase_add_test(tc1_1, from_float_to_decimal_9);

    tcase_add_test(tc1_1, from_decimal_to_int_1);
    tcase_add_test(tc1_1, from_decimal_to_int_2);
    tcase_add_test(tc1_1, from_decimal_to_int_3);
    tcase_add_test(tc1_1, from_decimal_to_int_4);
    tcase_add_test(tc1_1, from_decimal_to_int_5);
    tcase_add_test(tc1_1, from_decimal_to_int_6);

    tcase_add_test(tc1_1, from_decimal_to_float_1);
    tcase_add_test(tc1_1, from_decimal_to_float_2);
    tcase_add_test(tc1_1, from_decimal_to_float_3);
    tcase_add_test(tc1_1, from_decimal_to_float_4);
    tcase_add_test(tc1_1, from_decimal_to_float_5);

    tcase_add_test(tc1_1, from_int_to_dec);
    tcase_add_test(tc1_1, from_float_to_dec);
    tcase_add_test(tc1_1, from_dec_to_int);
    tcase_add_test(tc1_1, from_dec_to_float);

    tcase_add_test(tc1_1, compare_less);
    tcase_add_test(tc1_1, compare_greater);
    tcase_add_test(tc1_1, compare_less_or_equal);
    tcase_add_test(tc1_1, compare_greater_or_equal);
    tcase_add_test(tc1_1, compare_equal);
    tcase_add_test(tc1_1, compare_equal_null);
    tcase_add_test(tc1_1, compare_not_equal);

    tcase_add_test(tc1_1, eq_funcs_test);
    tcase_add_test(tc1_1, greater_funcs_test);
    tcase_add_test(tc1_1, lesser_funcs_test);
    tcase_add_test(tc1_1, eq_funcs_test_float_point);
    tcase_add_test(tc1_1, greater_test_float_point);

    tcase_add_test(tc1_1, int_to_dec_vv_test);
    tcase_add_test(tc1_1, flt_to_dec_vv_test);
    tcase_add_test(tc1_1, truncate_test);
    tcase_add_test(tc1_1, truncate_common_values);
    tcase_add_test(tc1_1, floor_dec);
    tcase_add_test(tc1_1, floor_test);
    tcase_add_test(tc1_1, add_1);
    tcase_add_test(tc1_1, add_2);
    tcase_add_test(tc1_1, add_3);
    tcase_add_test(tc1_1, add_4);
    tcase_add_test(tc1_1, add_5);
    tcase_add_test(tc1_1, add_6);
    tcase_add_test(tc1_1, add_7);
    tcase_add_test(tc1_1, add_8);
    tcase_add_test(tc1_1, add_9);
    tcase_add_test(tc1_1, add_10);
    tcase_add_test(tc1_1, add_11);
    tcase_add_test(tc1_1, add_12);
    tcase_add_test(tc1_1, add_13);
    tcase_add_test(tc1_1, add_14);
    tcase_add_test(tc1_1, add_15);
    tcase_add_test(tc1_1, add_16);
    tcase_add_test(tc1_1, add_17);
    tcase_add_test(tc1_1, add_18);
    tcase_add_test(tc1_1, add_19);
    tcase_add_test(tc1_1, add_20);
    tcase_add_test(tc1_1, add_21);
    tcase_add_test(tc1_1, add_norm);
    tcase_add_test(tc1_1, special_values);

    tcase_add_test(tc1_1, sub_9);
    tcase_add_test(tc1_1, sub_10);
    tcase_add_test(tc1_1, sub_11);
    tcase_add_test(tc1_1, sub_12);
    tcase_add_test(tc1_1, sub_13);
    tcase_add_test(tc1_1, sub_14);
    tcase_add_test(tc1_1, sub_15);
    tcase_add_test(tc1_1, sub_16);
    tcase_add_test(tc1_1, sub_normal_test_integer);
    tcase_add_test(tc1_1, sub_uncommon_values);

    tcase_add_test(tc1_1, div_common_values);
    tcase_add_test(tc1_1, mod_common_values);
    tcase_add_test(tc1_1, mod_1);
    tcase_add_test(tc1_1, mod_2);
    tcase_add_test(tc1_1, mod_3);
    tcase_add_test(tc1_1, mod_4);
    tcase_add_test(tc1_1, mod_5);
    tcase_add_test(tc1_1, mod_6);
    tcase_add_test(tc1_1, mod_7);
    tcase_add_test(tc1_1, mod_8);
    tcase_add_test(tc1_1, mod_uncommon_values);

    tcase_add_test(tc1_1, mul);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
