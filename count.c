/* count.c for count24 */

/* lasy */
/* 2011.10.20 */

#include <stdlib.h>

#include "lasy/core_defs.h"
#include "lasy/fraction.h"

#include "count.h"

#define is_equal(a, b) (fraction_compare(&(a), &(b)) == 0)
#define is_equal_int(a, b) (fraction_compare_int(&(a), (b)) == 0)
#define is_zero(num) (is_equal_int((num), 0))
#define is_one(num) (is_equal_int((num), 1) || is_equal_int((num), -1))
#define less_than(a, b) (fraction_compare(&(a), &(b)) < 0)
#define neg_of(num) (fraction_neg(num))
#define rec_of(num) (fraction_rec(num))

static struct fraction __rev_subtraction(struct fraction left, struct fraction right)
{
	return fraction_sub(right, left);
}

static struct fraction __rev_division(struct fraction left, struct fraction right)
{
	return fraction_div(right, left);
}

static struct fraction (*operations[NUM_OP])(struct fraction, struct fraction) = {
	fraction_add,
	fraction_sub,
	__rev_subtraction,
	fraction_mul,
	fraction_div,
	__rev_division
};

static void __init_number(struct number *num, struct fraction value)
{
	num->value = value;
	num->left = num->right = NULL;
	num->op = NOP;
}

static struct number *__new_number(struct fraction value)
{
	struct number *p;
	if ((p = malloc(sizeof(struct number))) == NULL) return NULL;
	__init_number(p, value);
	return p;
}

static struct number *__create_number_tree(struct number *num)
{
	struct number *root, *left, *right;
	if (num == NULL) return NULL;
	if ((root = __new_number(num->value)) == NULL) return root;
	if (is_revert(num->op)) {
		left = num->right;
		right = num->left;
	} else {
		left = num->left;
		right = num->right;
	}
	if (num->op == RSUB) {
		root->op = SUB;
	} else if (num->op == RDIV) {
		root->op = DIV;
	} else {
		root->op = num->op;
	}
	root->left = __create_number_tree(left);
	root->right = __create_number_tree(right);
	return root;
}

static void __release_number_tree(struct number *root)
{
	if (root->left != NULL) __release_number_tree(root->left);
	if (root->right != NULL) __release_number_tree(root->right);
	free(root);
}

static struct number *__rotate_numbers_left(struct number *root)
{
	struct number *left, *right, *right_left, *right_right;
	left = root->left;
	right = root->right;
	right_left = right->left;
	right_right = right->right;
	right->left = left;
	right->right = right_left;
	root->left = right;
	root->right = right_right;
	return root;
}

static void __recount_number(struct number *num)
{
	num->value = operations[num->op](num->left->value, num->right->value);
}

static void __swap_number_children(struct number *num)
{
	struct number *tmp = num->left;
	num->left = num->right;
	num->right = tmp;
}

static struct number *__refine_number_tree(struct number *root)
{
	enum OP_INDEX op, rop;
	if (root == NULL) return NULL;
	while (is_add_sub(op = root->op) && is_add_sub(rop = root->right->op)) {
		__rotate_numbers_left(root);
		root->op = ((op == rop) ? ADD : SUB);
		root->left->op = op;
		__recount_number(root->left);
		__recount_number(root);
	}
	while (is_mul_div(op = root->op) && is_mul_div(rop = root->right->op)) {
		__rotate_numbers_left(root);
		root->op = ((op == rop) ? MUL : DIV);
		root->left->op = op;
		__recount_number(root->left);
		__recount_number(root);
	}
	__refine_number_tree(root->left);
	__refine_number_tree(root->right);
	if (root->op == SUB && is_zero(root->right->value)) root->op = ADD;
	if (root->op == DIV && is_one(root->right->value)) root->op = MUL;
	return root;
}

static int __sort_number_tree_raw(struct number *root)
{
	enum OP_INDEX op, lop;
	struct number *left, *right;
	int need_resort;
	int swapped;
	if (root == NULL) return 0;
	swapped = 0;
	do {
		need_resort = 0;
		op = root->op;
		if (is_add_sub(op)) {
			left = root->left;
			right = root->right;
			lop = left->op;
			if (is_add_sub(lop)) {
				struct fraction v, lv;
				v = ((op == SUB) ? neg_of(right->value) : right->value);
				lv = ((lop == SUB) ? neg_of(left->right->value) : left->right->value);
				if (less_than(lv, v)) {
					root->right = left->right;
					left->right = right;
					root->op = lop;
					left->op = op;
					__recount_number(root->left);
					__recount_number(root);
					swapped = 1;
				}
				need_resort = 1;
			} else if (op == ADD && less_than(left->value, right->value)) {
				__swap_number_children(root);
				swapped = 1;
			}
		} else if (is_mul_div(op)) {
			left = root->left;
			right = root->right;
			lop = left->op;
			if (is_mul_div(lop)) {
				struct fraction v, lv;
				v = ((op == DIV) ? rec_of(right->value) : right->value);
				lv = ((lop == DIV) ? rec_of(left->right->value) : left->right->value);
				if (less_than(lv, v)) {
					root->right = left->right;
					left->right = right;
					root->op = lop;
					left->op = op;
					__recount_number(root->left);
					__recount_number(root);
					swapped = 1;
				}
				need_resort = 1;
			} else if (op == MUL && less_than(left->value, right->value)) {
				__swap_number_children(root);
				swapped = 1;
			}
		}
		need_resort = __sort_number_tree_raw(root->left) && need_resort;
	} while (need_resort);
	__sort_number_tree_raw(root->right);
	return swapped;
}

static struct number *__sort_number_tree(struct number *root)
{
	__sort_number_tree_raw(root);
	return root;
}

static solution_output_func *g_solution_output_func = NULL;

static int g_target = 24;

static int __try_combination(
	int num_number, struct number *num,
	int left, int right,
	enum OP_INDEX op
	)
{
	int i;
	int flag = 0;
	struct number *num_copy;
	if (op == DIV && is_zero(num[right].value)) return flag;
	if (op == RDIV && is_zero(num[left].value)) return flag;
	num_copy = malloc((num_number-1)*sizeof(struct number));
	for (i = 0; i < left; i++) num_copy[i] = num[i];
	for (i = left+1; i < right; i++) num_copy[i] = num[i];
	for (i = right; i < num_number-1; i++) num_copy[i] = num[i+1];
	num_copy[left].op = op;
	num_copy[left].value = operations[op](num[left].value, num[right].value);
	num_copy[left].left = &num[left];
	num_copy[left].right = &num[right];
	flag = count(num_number-1, num_copy) || flag;
	free(num_copy);
	return flag;
}

void set_solution_output_func(solution_output_func *func)
{
	g_solution_output_func = func;
}

void set_target_number(int target)
{
	g_target = target;
}

void init_number(struct number *num, int value)
{
	__init_number(num, int_to_fraction(value));
}

int count(int num_number, struct number *num)
{
	int flag = 0;
	int i, j, k;
	if (num_number == 1) {
		if (is_equal_int(num[0].value, g_target)) {
			struct number *root = __create_number_tree(num);
			if (root != NULL) {
				__refine_number_tree(root);
				__sort_number_tree(root);
				g_solution_output_func(root);
				__release_number_tree(root);
			}
			flag = 1;
		}
	} else {
#define DEDUP
		for (i = 0; i < num_number-1; i++) {
#ifdef DEDUP
			if (i > 0) {
				for (j = 0; j < i && !is_equal(num[j].value, num[i].value); j++);
				if (j < i) continue;
			}
#endif
			for (j = i+1; j < num_number; j++) {
#ifdef DEDUP
				if (i > 0) {
					for (k = 0; k < i && !is_equal(num[k].value, num[j].value); k++);
					if (k < i) continue;
				}
				if (j > i+1) {
					for (k = i+1; k < j && !is_equal(num[k].value, num[j].value); k++);
					if (k < j) continue;
				}
#endif
				for (k = 0; k < NUM_OP; k++) {
					flag = __try_combination(num_number, num, i, j, k) || flag;
				}
			}
		}
	}
	return flag;
}
