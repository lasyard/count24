/* fraction.h */

/* lasy */
/* 2008.08.01 */

#ifndef _LASY_FRACTION_H_
#define _LASY_FRACTION_H_

struct fraction {
	int nume;
	int deno;
};

#ifdef __cplusplus
extern "C" {
#endif

void fraction_reduce(struct fraction *a);

struct fraction fraction_neg(struct fraction a);
struct fraction fraction_rec(struct fraction a);

struct fraction fraction_add(struct fraction a, struct fraction b);
struct fraction fraction_sub(struct fraction a, struct fraction b);
struct fraction fraction_mul(struct fraction a, struct fraction b);
struct fraction fraction_div(struct fraction a, struct fraction b);

struct fraction int_to_fraction(int n);
int fraction_to_int(const struct fraction *a);

int fraction_compare(const struct fraction *a, const struct fraction *b);
int fraction_compare_int(const struct fraction *a, int b);

#ifdef __cplusplus
}
#endif

#endif
