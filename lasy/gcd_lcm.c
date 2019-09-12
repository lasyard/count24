/* gcd_lcm.c */

/* lasy */
/* 2008.08.01 */

int gcd(int a, int b)
{
	int r;
	while ((r = a%b) != 0) {
		a = b;
		b = r;
	}
	return b;
}

int lcm(int a, int b)
{
	return a*b/gcd(a, b);
}
