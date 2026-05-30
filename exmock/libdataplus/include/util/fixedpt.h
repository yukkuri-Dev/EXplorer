#ifndef _FIXEDPTC_H_
#define _FIXEDPTC_H_

/*
 * Signed 32-bit fixed point number library for the 24.8 format.
 * The specific limits are -8388608.999... to 8388607.999... and the
 * most precise number is 0.00390625. In practice, you should not count
 * on working with numbers larger than a million or to the precision
 * of more than 2 decimal places. Make peace with the fact that PI
 * is 3.14 here. :)
 *
 * Since the precision is relatively low, many complex functions (more
 * complex than div & mul) take a large hit on the precision of the
 * end result, because errors in precision accumulate.
 * This loss of precision can be lessened by increasing the number of
 * bits dedicated to the fraction part, but at the loss of range.
 *
 * Adventurous users might utilize this library to build two data types:
 * one which has the range, and one which has the precision, and carefully
 * convert between them (including adding two number of each type to produce
 * a simulated type with a larger range and precision).
 *
 * The ideas and algorithms have been cherry-picked from a large number
 * of previous implementations available on the Internet.
 */

/*-
 * Copyright (c) 2010 Ivan Voras <ivoras@freebsd.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdint.h>

typedef int32_t fixedpt;

/* Actually, you can redefine the FIXEDPT_WBITS constant to support other
 * divisions of the 32-bit integer, but who wants to work with 16-bit integers
 * these days? :)
 *
 * However, extending the base type to 64-bit would require approximately the
 * same work as rewriting this library from scratch: such implementation would
 * have to deal manually with overflows (there is no 128-bit data type in C),
 * and the magic numbers (there are a lot of them!) would have to be
 * recalculated for 64-bit operations.
 */

#define FIXEDPT_BITS	32
#ifndef FIXEDPT_WBITS
#define FIXEDPT_WBITS	16
#endif
#define FIXEDPT_FBITS	(FIXEDPT_BITS - FIXEDPT_WBITS)
#define FIXEDPT_FMASK	((1 << FIXEDPT_FBITS) - 1)

#define fixedpt_rconst(R) (int32_t)(R * (1LL << FIXEDPT_FBITS) + (R >= 0 ? 0.5 : -0.5))
#define fixedpt_fromint(I) ((int64_t)I << FIXEDPT_FBITS)
#define fixedpt_toint(F) (F >> FIXEDPT_FBITS)
#define fixedpt_add(A,B) (A + B)
#define fixedpt_sub(A,B) (A - B)
#define fixedpt_xmul(A,B) (int32_t)(((int64_t)A * (int64_t)B) >> FIXEDPT_FBITS)
#define fixedpt_xdiv(A,B) (int32_t)(((int64_t)A << FIXEDPT_FBITS) / (int64_t)B)
#define fixedpt_fracpart(A) (A & FIXEDPT_FMASK)

#define FIXEDPT_ONE	(int32_t)(1 << FIXEDPT_FBITS)
#define FIXEDPT_ONE_HALF (FIXEDPT_ONE >> 1)
#define FIXEDPT_TWO	(FIXEDPT_ONE + FIXEDPT_ONE)
#define FIXEDPT_PI	fixedpt_rconst(3.14159265)
#define FIXEDPT_TWO_PI	fixedpt_rconst(2*3.14159265)
#define FIXEDPT_HALF_PI	fixedpt_rconst(3.14159265/2)
#define FIXEDPT_E	fixedpt_rconst(2.71828183)

#define fixedpt_abs(A) ((A) < 0 ? -(A) : (A))


fixedpt fixedpt_mul(fixedpt A, fixedpt B);
fixedpt fixedpt_div(fixedpt A, fixedpt B);
void fixedpt_str(fixedpt A, char *str);
char* fixedpt_cstr(fixedpt A);

fixedpt fixedpt_sqrt(fixedpt A);
fixedpt fixedpt_exp(fixedpt fp);
fixedpt fixedpt_ln(fixedpt x);
fixedpt fixedpt_log(fixedpt x, fixedpt base);
fixedpt fixedpt_pow(fixedpt n, fixedpt exp);

fixedpt fixedpt_sin(fixedpt fp);
fixedpt fixedpt_cos(fixedpt A);
fixedpt fixedpt_tan(fixedpt A);

#endif
