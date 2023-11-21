#include "decimal.h"

// ==Convertors and parsers== //

int from_int_to_decimal(int src, decimal *dst) {
    int res = TRUE;
    if (dst) {
        init_dec_bits(dst);
        if (src < 0) {
            src *= -1;
            set_bit_dec(dst, 127, 1);
        }
        dst->bits[0] = src;
        dst->value_type = NORMAL_VALUE;
    } else {
        res = FALSE;
    }
    return res;
}

int from_decimal_to_int(decimal src, int *dst) {
    int res = 1;
    if (&src != NULL && dst != NULL && src.value_type == NORMAL_VALUE) {
        *dst = src.bits[0];
        *dst *= get_bit_dec(src, 127) ? -1 : 1;
        *dst /= (int)pow(10, get_scale(src));
        res = 0;
    }
    return res;
}

int from_float_to_decimal(float src, decimal *dst) {
    int result = FALSE;
    if (&src && dst) {
        dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
        dst->value_type = NORMAL_VALUE;
        int sign = getFloatSign(&src), exp = getFloatExp(&src);

        if (isinf(src) && !sign)
            dst->value_type = _INFINITY;
        else if (isinf(src) && sign)
            dst->value_type = NEGATIVE_INFINITY;
        else if (isnan(src))
            dst->value_type = NAN;

        if (dst && dst->value_type == NORMAL_VALUE && src != 0) {
            double temp = (double)fabs(src);
            int off = 0;
            for (; off < 28 && (int)temp / (int)pow(2, 21) == 0;
                 temp *= 10, off++) {
            }
            temp = round(temp);
            if (off <= 28 && (exp > -94 && exp < 96)) {
                floatbits mant;
                temp = (float)temp;
                for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10) {
                }
                mant.fl = temp;
                exp = getFloatExp(&mant.fl);
                dst->bits[exp / 32] |= 1 << exp % 32;
                for (int i = exp - 1, j = 22; j >= 0; i--, j--)
                    if ((mant.ui & (1 << j)) != 0)
                        dst->bits[i / 32] |= 1 << i % 32;
                dst->bits[3] = (sign << 31) | (off << 16);
                result = TRUE;
            }
        }
    }
    return result;
}

int from_decimal_to_float(decimal src, float *dst) {
    int result = FALSE;
    if (&src != NULL && dst && src.value_type == NORMAL_VALUE) {
        double temp = 0;
        int off = 0;
        for (int i = 0; i < 96; i++)
            if ((src.bits[i / 32] & (1 << i % 32)) != 0) temp += pow(2, i);
        if ((off = (src.bits[3] & ~SIGN) >> 16) > 0) {
            for (int i = off; i > 0; i--) temp /= 10.0;
        }
        *dst = (float)temp;
        *dst *= src.bits[3] >> 31 ? -1 : 1;
        result = TRUE;
    }
    return result;
}

// ==Arithmetic Operators== //

decimal add(decimal dec_a, decimal dec_b) {
    decimal res;
    int sign_a = get_bit_dec(dec_a, 127), sign_b = get_bit_dec(dec_b, 127);
    init_dec_bits(&res);
    if (dec_a.value_type > dec_b.value_type) {
        res.value_type = dec_a.value_type;
    } else if (!dec_a.value_type && !dec_b.value_type &&
               dec_a.value_type != dec_b.value_type) {
        res.value_type = NAN;
    } else {
        res.value_type = dec_b.value_type;
    }
    if (res.value_type == NORMAL_VALUE || res.value_type == ADDCODE) {
        if (!sign_a && !sign_b) {
            if (get_scale(dec_a) != get_scale(dec_b)) {
                norm_scale(&dec_a, &dec_b);
            }
            res = summ(dec_a, dec_b);
            res.bits[3] = dec_a.bits[3];
        } else if (sign_a && !sign_b) {
            set_bit_dec(&dec_a, 127, 0);
            res = sub(dec_b, dec_a);
        } else if (!sign_a && sign_b) {
            set_bit_dec(&dec_b, 127, 0);
            res = sub(dec_a, dec_b);
        } else if (sign_a && sign_b) {
            set_bit_dec(&dec_a, 127, 0);
            set_bit_dec(&dec_b, 127, 0);
            res = add(dec_a, dec_b);
            set_bit_dec(&res, 127, 1);
            if (res.value_type == _INFINITY) {
                res.value_type == NEGATIVE_INFINITY;
                clear_dec(&res);
            }
        }
    }
    if (res.value_type == ADDCODE) {
        res.value_type = NORMAL_VALUE;
    }

    return res;
}

decimal sub(decimal dec_a, decimal dec_b) {
    decimal res;
    decimal min, max;
    init_dec_bits(&min);
    init_dec_bits(&max);
    init_dec_bits(&res);
    int sign_a = 0;
    int sign_b = 0;
    int fin_scale = 0;
    int fin_sign = 0;
    init_dec_bits(&res);
    if (dec_a.value_type != NORMAL_VALUE) {
        res.value_type = dec_a.value_type;
    } else {
        res.value_type = dec_b.value_type;
    }
    if (res.value_type == NORMAL_VALUE) {
        if (get_scale(dec_a) != get_scale(dec_b)) {
            fin_scale = norm_scale(&dec_a, &dec_b);
            set_dec_scale(&res, fin_scale);
        }
    }
    if (get_bit_dec(dec_a, 127) != get_bit_dec(dec_b, 127)) {
        fin_sign = get_bit_dec(dec_a, 127);
        set_bit_dec(&dec_a, 127, 0);
        set_bit_dec(&dec_b, 127, 0);
        res = add(dec_a, dec_b);
        set_bit_dec(&res, 127, fin_sign);
    } else {
        if (is_equal(dec_a, dec_b) == FALSE) {
            sign_a = get_bit_dec(dec_a, 127);
            sign_b = get_bit_dec(dec_b, 127);
            set_bit_dec(&dec_a, 127, 0);
            set_bit_dec(&dec_b, 127, 0);
            if (is_less(dec_a, dec_b) == FALSE) {
                min = dec_b;
                max = dec_a;
                fin_sign = sign_a;
            } else {
                min = dec_a;
                max = dec_b;
                fin_sign = !sign_b;
            }
            dec_to_addcode(&min);
            res = add(min, max);
            set_bit_dec(&res, 127, fin_sign);
        } else {
            init_dec_bits(&res);
        }
    }
    return res;
}

decimal mul(decimal dec_a, decimal dec_b) {
    decimal res, buf;
    int bit;
    int sign_a = get_bit_dec(dec_a, 127), sign_b = get_bit_dec(dec_b, 127);
    init_dec_bits(&res);
    init_dec_bits(&buf);
    res = prov_type_mul(dec_a, dec_b);
    if (res.value_type == NORMAL_VALUE) {
        int scale = get_scale(dec_a) + get_scale(dec_b);
        while (scale > 28) {
            dec_div_10(&dec_a);
            dec_div_10(&dec_b);
            scale = get_scale(res);
        }
        for (int i = 0; i < 95; i++) {
            init_dec_bits(&buf);
            set_scale(&buf, scale);
            for (int j = 0; j < 95; j++) {
                bit = get_bit_dec(dec_a, j) * get_bit_dec(dec_b, i);
                set_bit_dec(&buf, j, bit);
            }
            shift(&buf, i);
            res = summ(res, buf);
            if ((sign_a && sign_b) || (!sign_a && !sign_b)) {
                set_bit_dec(&res, 127, 0);
            } else if (sign_a || sign_b) {
                set_bit_dec(&res, 127, 1);
            }
        }
        set_dec_scale(&res, scale);
    }
    if (res.value_type == _INFINITY && get_bit_dec(res, 127)) {
        res.value_type = NEGATIVE_INFINITY;
    }
    return res;
}

decimal prov_type_mul(decimal dec_1, decimal dec_2) {
    decimal res;
    decimal zero;
    init_dec_bits(&res);
    init_dec_bits(&zero);
    int it_nan = dec_nan(&dec_1, &dec_2) == TRUE ? 1 : 0;
    int it_inf = dec_inf(&dec_1, &dec_2) != TRUE ? 1 : 0;
    int it_neg_inf = dec_neg_inf(&dec_1, &dec_2) != TRUE ? 1 : 0;
    int it_zero =
        is_equal(dec_1, zero) == TRUE || is_equal(dec_2, zero) ? 1 : 0;
    int norm = (is_greater(dec_1, zero) == TRUE &&
                    dec_1.value_type == NORMAL_VALUE ||
                is_greater(dec_2, zero) == TRUE &&
                    dec_2.value_type == NORMAL_VALUE)
                   ? 1
                   : 0;
    int norm_minus = (is_less(dec_1, zero) == TRUE &&
                          dec_1.value_type == NORMAL_VALUE ||
                      is_less(dec_2, zero) == TRUE &&
                          dec_2.value_type == NORMAL_VALUE)
                         ? 1
                         : 0;
    res.value_type = dec_1.value_type ? dec_1.value_type : dec_2.value_type;
    if (it_nan || (it_zero && it_inf) || (it_zero && it_neg_inf)) {
        res.value_type = NAN;
    } else if ((norm && it_inf) || (norm_minus && it_neg_inf) ||
               (dec_1.value_type == _INFINITY &&
                dec_2.value_type == _INFINITY) ||
               (dec_1.value_type == NEGATIVE_INFINITY &&
                dec_2.value_type == NEGATIVE_INFINITY)) {
        res.value_type = _INFINITY;
    } else if ((norm_minus && it_inf) || (it_inf && it_neg_inf) ||
               (norm && it_neg_inf)) {
        res.value_type = NEGATIVE_INFINITY;
    }
    return res;
}

decimal div(decimal dec_a, decimal dec_b) {
    decimal res;
    int res_scale = 0;
    int res_sign = 0;
    res = prov_type_div(dec_a, dec_b);
    if (dec_a.value_type == NORMAL_VALUE &&
        (dec_b.value_type == _INFINITY ||
         dec_b.value_type == NEGATIVE_INFINITY)) {
        init_dec_bits(&res);
    } else {
        if (res.value_type == NORMAL_VALUE) {
            decimal zero;
            decimal victim;
            decimal remainder;
            decimal tmp;
            decimal ten = {{10, 0, 0, 0}, NORMAL_VALUE};
            decimal border_value = {{-1, -1, -1, 0}, NORMAL_VALUE};
            set_dec_scale(&border_value, 1);
            init_dec_bits(&remainder);
            init_dec_bits(&tmp);
            init_dec_bits(&zero);
            init_dec_bits(&victim);
            int beg_scale = get_scale(dec_a) - get_scale(dec_b);
            int tmp_scale = 0;
            if (get_bit_dec(dec_a, 127) || get_bit_dec(dec_b, 127)) {
                res_sign = 1;
            }
            set_dec_scale(&dec_a, 0);
            set_bit_dec(&dec_a, 127, 0);
            set_dec_scale(&dec_b, 0);
            set_bit_dec(&dec_b, 127, 0);

            tmp = div_only_bit(dec_a, dec_b, &remainder);
            only_bits_equals(tmp, &res);

            for (; tmp_scale <= 27 && is_equal(remainder, zero) == FALSE;) {
                if (is_less(res, border_value) == FALSE) {
                    break;
                }
                remainder = mul(remainder, ten);
                tmp = div_only_bit(remainder, dec_b, &remainder);
                res = mul(res, ten);
                res = add(res, tmp);
                tmp_scale++;
            }
            res_scale = beg_scale + tmp_scale;
            for (; res_scale > 28;) {
                res = div_only_bit(res, ten, &victim);
                res_scale--;
            }
            for (; res_scale < 0;) {
                res = mul(res, ten);
                res_scale++;
            }
        }
        set_dec_scale(&res, res_scale);
        set_bit_dec(&res, 127, res_sign);
    }
    return res;
}

decimal div_only_bit(decimal dec_a, decimal dec_b,
                         decimal *remainder) {
    init_dec_bits(remainder);
    decimal res = {{0, 0, 0, 0}, 0};
    for (int i = last_bit(dec_a); i >= 0; i--) {
        if (get_bit_dec(dec_a, i)) set_bit_dec(remainder, 0, 1);
        if (is_greater_or_equal(*remainder, dec_b) == TRUE) {
            *remainder = sub(*remainder, dec_b);
            if (i != 0) shift(remainder, 1);
            if (get_bit_dec(dec_a, i - 1)) set_bit_dec(remainder, 0, 1);
            shift(&res, 1);
            set_bit_dec(&res, 0, 1);
        } else {
            shift(&res, 1);
            if (i != 0) shift(remainder, 1);
            if ((i - 1) >= 0 && get_bit_dec(dec_a, i - 1))
                set_bit_dec(remainder, 0, 1);
        }
    }
    return res;
}

// ==Comparison Operators== //

int is_less(decimal dec_a, decimal dec_b) {
    int res = is_greater(dec_b, dec_a);
    return res;
}

int is_less_or_equal(decimal dec_a, decimal dec_b) {
    int res = FALSE;
    if (is_less(dec_a, dec_b) == TRUE ||
        is_equal(dec_a, dec_b) == TRUE || dec_inf(&dec_a, &dec_b) == 2 ||
        dec_neg_inf(&dec_a, &dec_b) == 2) {
        res = TRUE;
    }
    return res;
}

int is_greater(decimal dec_a, decimal dec_b) {
    int res = -1;
    int prov_inf = dec_inf(&dec_a, &dec_b);
    int prov_neg_inf = dec_neg_inf(&dec_a, &dec_b);
    if ((dec_nan(&dec_a, &dec_b) == TRUE) ||
        (!is_zero_dec(dec_a, dec_b) && !dec_inf(&dec_a, &dec_b) &&
         !dec_neg_inf(&dec_a, &dec_b))) {
        res = FALSE;
    }
    if (res == -1) {
        if (prov_inf == 1) {
            res = TRUE;
        } else if (prov_inf == 2 || prov_inf == -1) {
            res = FALSE;
        }
        if (prov_neg_inf == -1) {
            res = TRUE;
        } else if (prov_neg_inf == 1 || prov_neg_inf == 2) {
            res = FALSE;
        }
        if (res == -1 && (get_scale(dec_a) != get_scale(dec_b))) {
            norm_scale(&dec_a, &dec_b);
        }
    }
    if (compar_func(dec_a, dec_b) == 0 && res == -1) {
        res = TRUE;
    }
    if (res != TRUE) {
        res = FALSE;
    }
    return res;
}

int is_greater_or_equal(decimal dec_a, decimal dec_b) {
    int res = FALSE;
    if (is_greater(dec_a, dec_b) == TRUE ||
        is_equal(dec_a, dec_b) == TRUE) {
        res = TRUE;
    }
    return res;
}

int is_equal(decimal dec_a, decimal dec_b) {
    int res = FALSE;
    int prov_inf = dec_inf(&dec_a, &dec_b);
    int prov_neg_inf = dec_neg_inf(&dec_a, &dec_b);
    if (dec_nan(&dec_a, &dec_b) == TRUE) {
        res = FALSE;
    } else {
        if (((get_scale(dec_a) != get_scale(dec_b)))) {
            norm_scale(&dec_a, &dec_b);
        }
        int comp = compar_func(dec_a, dec_b);
        if (comp != 2) {
            res = FALSE;
        } else {
            res = TRUE;
        }
        if (is_zero_dec(dec_a, dec_b) == TRUE) {
            res = TRUE;
        }
        if (prov_inf == 2) {
            res = TRUE;
        } else if (prov_inf == 1 || prov_inf == -1) {
            res = FALSE;
        }
        if (prov_neg_inf == 2) {
            res = TRUE;
        } else if (prov_neg_inf == 1 || prov_neg_inf == -1) {
            res = FALSE;
        }
        if (res == -1) {
            res = FALSE;
        }
    }
    return res;
}

int is_not_equal(decimal dec_a, decimal dec_b) {
    int res = FALSE;
    if (is_equal(dec_a, dec_b) == FALSE) {
        res = TRUE;
    }
    return res;
}

// ==Another functions== //

decimal floor(decimal src) {
    unsigned f = 0;
    if (!src.value_type) {
        decimal one, tmp;
        from_int_to_decimal(1, &one);
        init_dec_bits(&tmp);
        unsigned scale = get_scale(src), sign = get_bit(src.bits[3], 31);
        for (int i = 0; i < scale; i++) {
            if (dec_div_10_round(&src)) f++;
        }
        if (sign && f) {
            tmp = add(src, one);
            if (tmp.value_type) {
                src.value_type = NEGATIVE_INFINITY;
            } else {
                set_bit(&tmp.bits[3], 31, 1);
                src = tmp;
            }
        }
    }
    return src;
}

void print_dec(decimal dec) {
    // printf("[");
    for (int i = 95; i >= 0; i--) {
        printf("%d", get_bit_dec(dec, i));
        // if (!(i % 32)) {
        // if (i == 0)
        // printf("]");
        // else
        // printf("][");
        // }
    }
    printf("\n");
}

decimal truncate(decimal src) {
    if (!src.value_type) {
        unsigned scale = get_scale(src);
        for (int i = 0; i < scale; i++) dec_div_10(&src);
    }
    return src;
}

decimal negate(decimal src) {
    if (get_bit_dec(src, 127)) {
        set_bit(&src.bits[3], 31, 0);
    } else {
        set_bit(&src.bits[3], 31, 1);
    }
    return src;
}

// ==Dop functions== //

void init_dec_bits(decimal *dst) {
    for (int i = 0; i <= 3; i++) {
        dst->bits[i] = 0;
    }
    dst->value_type = NORMAL_VALUE;
}

void set_bit(int *dst, unsigned bit, unsigned set) {
    if (set == 1)
        *dst |= (1 << bit);
    else
        *dst &= ~(1 << bit);
}

unsigned get_bit(int src, unsigned bit) { return src & (1 << bit) ? 1 : 0; }

unsigned get_bit_dec(decimal dst, unsigned bit) {
    int res = 0;
    if (bit / 32 < 4) {
        unsigned int mask = 1u << (bit % 32);
        res = dst.bits[bit / 32] & mask;
    }
    return !!res;
}

void set_bit_dec(decimal *dst, unsigned num_bit, unsigned set) {
    if (set == 1)
        dst->bits[num_bit / 32] |= (1U << num_bit);
    else
        dst->bits[num_bit / 32] &= ~(1U << num_bit);
}

void shift(decimal *src, int offset) {
    unsigned int mask_32;
    unsigned int mask_63;
    int last = last_bit(*src);
    if (last + offset > 95) {
        src->value_type = _INFINITY;
        return;
    }
    for (int i = 0; i < abs(offset); i++) {
        if (offset > 0) {
            mask_32 = get_bit((*src).bits[0], 31);
            mask_63 = get_bit((*src).bits[1], 31);
            src->bits[2] <<= 1;
            src->bits[1] <<= 1;
            src->bits[0] <<= 1;
            if (mask_32) {
                set_bit(&(*src).bits[1], 0, 1);
            }
            if (mask_63) {
                set_bit(&(*src).bits[2], 0, 1);
            }
        } else {
            mask_32 = get_bit((*src).bits[1], 0);
            mask_63 = get_bit((*src).bits[2], 0);
            src->bits[2] >>= 1;
            src->bits[1] >>= 1;
            src->bits[0] >>= 1;
            if (mask_32) {
                set_bit(&(*src).bits[0], 31, 1);
            }
            if (mask_63) {
                set_bit(&(*src).bits[1], 31, 1);
            }
        }
    }
}

unsigned get_scale(decimal src) {
    unsigned scale;
    scale = ((src.bits[3] << 1) >> 17);
    return scale;
}

void set_scale(decimal *dst, unsigned scale) {
    (*dst).bits[3] &= 0x80000000;
    (*dst).bits[3] |= scale;
}

void set_dec_scale(decimal *dst, int scale) {
    char sign = 0;
    if (get_bit(dst->bits[3], 31)) {
        sign = 1;
    }
    dst->bits[3] = 0;
    dst->bits[3] |= (scale << 16);
    if (sign) {
        set_bit_dec(dst, 127, 1);
    }
}

decimal summ(decimal par_a, decimal par_b) {
    decimal res;
    int tmp = 0;
    init_dec_bits(&res);
    for (int i = 0; i < 96; i++) {
        char bit_a = get_bit_dec(par_a, i);
        char bit_b = get_bit_dec(par_b, i);
        if (bit_a && bit_b && tmp) {
            set_bit_dec(&res, i, 1);
            tmp = 1;
        } else if (bit_a && bit_b && !tmp) {
            tmp = 1;
        } else if ((bit_a ^ bit_b) && tmp) {
            tmp = 1;
        } else if ((bit_a ^ bit_b) && !tmp) {
            set_bit_dec(&res, i, 1);
        } else if (!bit_a && !bit_b && tmp) {
            set_bit_dec(&res, i, 1);
            tmp = 0;
        }
        if (i == 95 && tmp && par_a.value_type != ADDCODE &&
            par_b.value_type != ADDCODE) {
            res.value_type = _INFINITY;
        } else {
            res.value_type = NORMAL_VALUE;
        }
        if (dec_inf(&par_a, &par_b) != 0) {
            res.value_type = _INFINITY;
        }
    }
    return res;
}

unsigned dec_div_10(decimal *src) {
    unsigned out = 0, i = 95;
    for (; !get_bit((*src).bits[i / 32], i) && i; i--) {
    }
    decimal ten, buff, divided;
    from_int_to_decimal(10, &ten);
    init_dec_bits(&buff);
    init_dec_bits(&divided);
    set_dec_scale(&divided, get_scale(*src) - 1);
    if (get_bit((*src).bits[3], 31)) set_bit(&divided.bits[3], 31, 1);
    (*src).bits[3] = 0;
    for (; i >= 3; i--) {
        decimal tmp = ten;
        shift(&tmp, i - 3);
        buff = tmp;
        shift(&divided, 1);
        if (is_less(*src, buff)) {
            *src = sub(*src, buff);
            set_bit(&divided.bits[0], 0, 1);
        }
    }
    out = (*src).bits[0];
    *src = divided;
    return out;
}

int compar_func(decimal dec_a, decimal dec_b) {
    int res = -1;
    int sign_a = get_bit_dec(dec_a, 127), sign_b = get_bit_dec(dec_b, 127);
    for (int i = 95; i >= 0; i--) {
        if (!get_bit_dec(dec_a, i) && get_bit_dec(dec_b, i)) {
            res = 1;
            break;
        }
        if (get_bit_dec(dec_a, i) && !get_bit_dec(dec_b, i)) {
            res = 0;
            break;
        }
    }
    if (!sign_a && sign_b) {
        res = 0;
    } else if (sign_a && !sign_b) {
        res = 1;
    }
    if (res == -1 && (sign_a == sign_b)) {
        res = 2;
    }
    if (sign_a && sign_b && res == 0) {
        res = 1;
    } else if (sign_a && sign_b && res == 1) {
        res = 0;
    }
    return res;
}

int norm_scale(decimal *dec_a, decimal *dec_b) {
    decimal tmp, tmp_1, tmp_2, *min = NULL, *max = NULL;
    init_dec_bits(&tmp);
    init_dec_bits(&tmp_1);
    init_dec_bits(&tmp_2);
    int scale_a = get_scale(*dec_a);
    int scale_b = get_scale(*dec_b);
    int fin_scale = 0;
    if (scale_a > scale_b) {
        max = dec_a;
        min = dec_b;
    } else {
        max = dec_b;
        min = dec_a;
    }
    while (get_scale(*dec_a) != get_scale(*dec_b)) {
        int scale_a1 = get_scale(*dec_a);
        int scale_b1 = get_scale(*dec_b);
        if (tmp.value_type == NORMAL_VALUE && !get_bit_dec(*min, 93) &&
            !get_bit_dec(*min, 94) && !get_bit_dec(*min, 95)) {
            tmp_1 = tmp_2 = *min;
            shift(&tmp_1, 1);
            shift(&tmp_2, 3);
            tmp = summ(tmp_1, tmp_2);
            int min_scale = get_scale(*min);
            if (tmp.value_type == NORMAL_VALUE) {
                only_bits_equals(tmp, min);
                set_dec_scale(min, min_scale + 1);
            }

        } else {
            dec_div_10(max);
        }
    }
    fin_scale = get_scale(*dec_a);
    return fin_scale;
}

int dec_nan(decimal *dec_a, decimal *dec_b) {
    int type_a = dec_a->value_type;
    int type_b = dec_b->value_type;

    return (type_a == NAN || type_b == NAN) ? TRUE : FALSE;
}

int dec_inf(decimal *dec_a, decimal *dec_b) {
    int res = 0;
    int type_a = dec_a->value_type;
    int type_b = dec_b->value_type;

    if (type_a == _INFINITY && type_b != _INFINITY) {
        res = 1;
    } else if (type_a != _INFINITY && type_b == _INFINITY) {
        res = -1;
    } else if (type_a == _INFINITY && type_b == _INFINITY) {
        res = 2;
    }
    return res;
}

int dec_neg_inf(decimal *dec_a, decimal *dec_b) {
    int res = 0;
    int type_a = dec_a->value_type;
    int type_b = dec_b->value_type;

    if (type_a == NEGATIVE_INFINITY && type_b != NEGATIVE_INFINITY) {
        res = 1;
    }
    if (type_a != NEGATIVE_INFINITY && type_b == NEGATIVE_INFINITY) {
        res = -1;
    }
    if (type_a == NEGATIVE_INFINITY && type_b == NEGATIVE_INFINITY) {
        res = 2;
    }

    return res;
}

int is_zero_dec(decimal dec_a, decimal dec_b) {
    int res = FALSE;
    decimal *tmp_a = &dec_a;
    decimal *tmp_b = &dec_b;

    if (tmp_a && tmp_b) {
        if (!dec_a.bits[0] && !dec_b.bits[0] && !dec_a.bits[1] &&
            !dec_b.bits[1] && !dec_a.bits[2] && !dec_b.bits[2])
            res = TRUE;
    }
    return res;
}

void only_bits_equals(decimal src, decimal *dst) {
    dst->bits[0] = src.bits[0];
    dst->bits[1] = src.bits[1];
    dst->bits[2] = src.bits[2];
}

int last_bit(decimal dec) {
    int last = 0;
    int i = 95;
    for (i = 95; i >= 0; i--) {
        if (get_bit_dec(dec, i)) {
            last = i;
            break;
        }
    }
    return i;
}

void dec_to_addcode(decimal *dec) {
    decimal add;
    decimal one;
    init_dec_bits(&one);
    from_int_to_decimal(1, &one);
    dec->bits[0] = ~dec->bits[0];
    dec->bits[1] = ~dec->bits[1];
    dec->bits[2] = ~dec->bits[2];
    add = summ(*dec, one);
    dec->bits[0] = add.bits[0];
    dec->bits[1] = add.bits[1];
    dec->bits[2] = add.bits[2];
    dec->value_type = ADDCODE;
}

void clear_dec(decimal *dec) {
    for (int i = 0; i <= 3; i++) {
        dec->bits[i] = 0;
    }
}

decimal str_to_dec(char binary_number[], decimal *dec,
                       int point_place) {
    if (dec == NULL) {
        decimal res;
        dec = &res;
    }
    init_dec_bits(dec);
    char copy[100];
    int shift = 0;
    if (binary_number[0] == '-') {
        set_bit_dec(dec, 127, 1);
        strncpy(copy, binary_number + 1, 97);
    } else {
        strncpy(copy, binary_number, 98);
    }
    dec->bits[3] += point_place << 16;
    int len_number = strlen(copy);
    for (int i = shift; i < len_number; i++) {
        int bit_place = (i - shift) % 32;
        int int_place = (i - shift) / 32;
        int bit = copy[len_number - 1 - i + shift] - '0';
        dec->bits[int_place] += bit << bit_place;
    }

    return *dec;
}

int getFloatSign(float *src) { return *(int *)src >> 31; }

int getFloatExp(float *src) { return ((*(int *)src & ~SIGN) >> 23) - 127; }

unsigned get_last_bit(decimal src) {
    unsigned i;
    for (i = 95; i > 0; i--) {
        if (get_bit(src.bits[i / 32], i % 32)) break;
    }
    return i;
}

decimal prov_type_div(decimal dec_a, decimal dec_b) {
    decimal res;
    decimal zero;
    init_dec_bits(&zero);
    init_dec_bits(&res);
    int dec_b_norm = is_greater(dec_b, zero) == TRUE &&
                             dec_b.value_type == NORMAL_VALUE
                         ? 1
                         : 0;
    int dec_b_norm_minus =
        is_less(dec_b, zero) == TRUE && dec_b.value_type == NORMAL_VALUE
            ? 1
            : 0;
    if (dec_nan(&dec_a, &zero) == TRUE) {
        res.value_type = NAN;
    } else if (is_equal(dec_b, zero) == TRUE &&
               is_less(dec_a, zero) == TRUE) {
        res.value_type = NEGATIVE_INFINITY;
    } else if (is_equal(dec_b, zero) == TRUE &&
               is_greater(dec_a, zero) == TRUE) {
        res.value_type = _INFINITY;
    } else if (is_equal(dec_b, zero) == TRUE &&
               is_equal(dec_a, zero) == TRUE) {
        res.value_type = NAN;
    } else if ((dec_a.value_type == _INFINITY ||
                dec_a.value_type == NEGATIVE_INFINITY) &&
               (dec_b.value_type == _INFINITY ||
                dec_b.value_type == NEGATIVE_INFINITY)) {
        res.value_type = NAN;
    } else if ((dec_a.value_type == _INFINITY && dec_b_norm) ||
               (dec_a.value_type == NEGATIVE_INFINITY &&
                dec_b_norm_minus)) {
        res.value_type = _INFINITY;
    } else if ((dec_a.value_type == _INFINITY && dec_b_norm_minus) ||
               (dec_a.value_type == NEGATIVE_INFINITY && dec_b_norm)) {
        res.value_type = NEGATIVE_INFINITY;
    }
    return res;
}

decimal shift_dec(decimal src, int offset) {
    int last_bit = get_last_bit(src);
    if (offset && (last_bit + offset) < 96 && (last_bit + offset) >= 0) {
        decimal (*shift)(decimal, unsigned);
        if (offset > 0)
            shift = shift_left;
        else
            shift = shift_right;
        src = shift(src, abs(offset));
    } else if ((last_bit + offset) >= 96) {
        src.value_type = _INFINITY;
    } else if ((last_bit + offset) < 0) {
        src.value_type = NEGATIVE_INFINITY;
    }
    return src;
}

decimal shift_left(decimal src, unsigned offset) {
    decimal out;
    unsigned last_bit = get_last_bit(src);
    init_dec_bits(&out);
    while (offset) {
        for (int i = 0; i < last_bit + 1 && i < 95; i++) {
            set_bit(&out.bits[(i + 1) / 32], (i + 1) % 32,
                    get_bit(src.bits[i / 32], i % 32));
        }
        src = out;
        offset--;
        last_bit++;
    }
    return out;
}

decimal shift_right(decimal src, unsigned offset) {
    decimal out;
    unsigned last_bit = get_last_bit(src);
    init_dec_bits(&out);
    while (offset) {
        for (int i = last_bit + 1; i > 0; i--) {
            set_bit(&out.bits[(i - 1) / 32], (i - 1) % 32,
                    get_bit(src.bits[i / 32], i % 32));
        }
        src = out;
        offset--;
        last_bit--;
    }
    return out;
}

unsigned dec_div_10_round(decimal *src) {
    unsigned out = 0, i = get_last_bit(*src);
    decimal ten, buff, divided;
    from_int_to_decimal(10, &ten);
    init_dec_bits(&buff);
    init_dec_bits(&divided);
    set_scale(&divided, get_scale(*src) - 1);
    set_bit(&divided.bits[3], 31, get_bit((*src).bits[3], 31));
    (*src).bits[3] = 0;
    for (; i >= 3; i--) {
        buff = shift_dec(ten, i - 3);
        divided = shift_dec(divided, 1);
        if (is_less(*src, buff)) {
            *src = sub(*src, buff);
            set_bit(&divided.bits[0], 0, 1);
        }
    }
    out = (*src).bits[0];
    *src = divided;
    return out;
}

decimal mod(decimal a, decimal b) {
    decimal mod = {{0, 0, 0, 0}, 0};
    if (last_bit(b) != 0) {
        mod.value_type = a.value_type ?: b.value_type;
        if (!mod.value_type) {
            char sca = get_scale(a);
            char scb = get_scale(b);
            int sign = detect_minus_result(&a, &b);
            if (is_greater_or_equal(a, b)) {
                mod = a;
            } else {
                char scmax = 0;
                if (!is_equal(a, b)) sign = 0;
                if (sca >= scb)
                    scmax = sca;
                else
                    scmax = scb;
                norm_scale(&a, &b);
                a.bits[3] = 0;
                b.bits[3] = 0;
                div_int(a, b, &mod);
                set_dec_scale(&mod, scmax);
            }
            if (sign) mod.bits[3] ^= 0x80000000;
        }
    } else {
        init_dec_bits(&mod);
        mod.value_type = NAN;
    }
    return mod;
}

decimal dec_round(decimal dec) {
    decimal round = dec;
    int exp = get_scale(dec);
    if (dec.value_type == NORMAL_VALUE && exp > 0) {
        decimal one = {{1, 0, 0, 0}, NORMAL_VALUE};
        int last = 0;
        while (exp > 0) {
            if (exp == 1) last = ModTen(&round);
            round = DenTen(&round);
            exp--;
        }
        set_dec_scale(&round, exp);
        if (get_bit_dec(dec, 127) == 1) set_bit_dec(&one, 127, 1);
        if (last > 5) {
            round = add(round, one);
        } else if (last == 5) {
            last = ModTen(&round);
            if (last % 2 == 1) round = add(round, one);
        }
    }
    return round;
}

int ModTen(decimal *dec) {
    int num = 0;
    for (int i = 0; i < 96; i++) {
        num = num * 2 + get_bit_dec(*dec, 95 - i);
        if (num >= 10) {
            num = num - 10;
        }
    }
    return num;
}

decimal DenTen(decimal *dec) {
    decimal res = {{0, 0, 0, 0}, NORMAL_VALUE};
    unsigned int num = 0;
    for (int i = 0; i < 96; i++) {
        num = num * 2 + get_bit_dec(*dec, 95 - i);
        if (num < 10) {
            set_bit_dec(&res, 95 - i, 0u);
        } else {
            num = num - 10;
            set_bit_dec(&res, 95 - i, 1u);
        }
    }
    set_dec_scale(&res, get_scale(*dec));
    set_bit_dec(&res, 127, get_bit_dec(*dec, 127));
    return res;
}

decimal div_int(decimal a, decimal b, decimal *buf) {
    decimal res = {{0, 0, 0, b.bits[3]}, 0};
    int len_a = len_of_digit(a);
    int i = len_a - 1;
    while (i >= 0) {
        if (get_bit_dec(a, i)) set_bit_dec(buf, 0, 1);
        if (!is_greater_or_equal(*buf, b)) {
            *buf = sub(*buf, b);
            if (i != 0) shift(buf, 1);
            if (get_bit_dec(a, i - 1)) set_bit_dec(buf, 0, 1);
            shift(&res, 1);
            set_bit_dec(&res, 0, 1);
        } else {
            shift(&res, 1);
            if (i != 0) shift(buf, 1);
            if ((i - 1) >= 0 && get_bit_dec(a, i - 1)) set_bit_dec(buf, 0, 1);
        }
        i--;
    }
    return res;
}

int len_of_digit(decimal dig) {
    int res = 0;
    for (int i = 95; i >= 0 && !res; i--)
        if (get_bit_dec(dig, i)) res = i + 1;
    return res;
}

int detect_minus_result(decimal *a, decimal *b) {
    int sign = 0;
    if (get_bit_dec(*a, 127)) {
        a->bits[3] ^= 0x80000000;
        b->bits[3] ^= 0x80000000;
        sign = 1;
    }
    return sign;
}
