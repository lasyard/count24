/* count.h for count24 */

/* lasy */
/* 2011.10.20 */

#ifndef _COUNT_H_
#define _COUNT_H_

typedef void solution_output_func(const struct number *num);

enum OP_INDEX {NOP = -1, ADD = 0, SUB, RSUB, MUL, DIV, RDIV, NUM_OP};

#define is_add_sub(op) ((op) == ADD || (op) == SUB || (op) == RSUB)
#define is_mul_div(op) ((op) == MUL || (op) == DIV || (op) == RDIV)
#define is_revert(op) ((op) == RSUB || (op) == RDIV)
#define is_swappable(op) ((op) == ADD || (op == MUL))

struct number {
	struct fraction value;
	struct number *left, *right;
	enum OP_INDEX op;
};

#ifdef __cplusplus
extern "C" {
#endif

void set_solution_output_func(solution_output_func *func);
void set_target_number(int target);
void init_number(struct number *num, int value);
int count(int num_number, struct number *num);

#ifdef __cplusplus
}
#endif

#endif
