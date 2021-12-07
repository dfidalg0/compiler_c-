/* Um programa para calcular mdc */
int gdc (int u, int v)
{
   if (v == 0) return u;
   else return gdc(v,u-u/v*v);
   /* u-u / v*v == u mod v */
}

void main(void)
{
   int r[2];
   r[0] = input();
   r[1] = input();
   output(gdc(r[0], r[1]));
}
