/* core_defs.h */

/* lasy */
/* 2011.02.18 */

#ifndef _LASY_CORE_DEFS_H_
#define _LASY_CORE_DEFS_H_

#ifndef NULL
#define NULL ((void *)0)
#endif

#define	container_of(ptr, type, member) ((type *)((char *)(ptr) - (int)&((type *)0)->member))

#define is_space(ch)    ((ch) == ' ' || (ch) == '\t')
#define is_digit(ch)    ('0' <= (ch) && (ch) <= '9')
#define is_line_end(ch) ((ch) == '\0' || (ch) == '\r' || (ch) == '\n')

#define is_set(x, b)    (((x) & (b)) == (b))
#define clear_bit(x, b) do {(x) &= (~(b)); } while (0)
#define set_bit(x, b)   do {(x) |= (b); } while (0)

#endif
