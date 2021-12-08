/* Um programa para calcular mdc */
int gdc(int u, int v) {
    if (v == 0)
        return u;
    else
        return gdc(v, u - u / v * v);
    /* u-u / v*v == u mod v */
}

int main(void) {
    int r[2];
    r[0] = 10;
    r[1] = 5;
    return gdc(r[0], r[1]);
}
