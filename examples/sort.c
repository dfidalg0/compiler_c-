/* int minloc(int a[], int i, int high) { return 0; } */

/**
 * Comentário de várias linhas
 */
void sort(int a[], int low, int high) {
    int i;
    int k;
    i = low;
    while (i < high - 1) {
        int t;
        k = minloc(a, i, high);
        t = a[k];
        a[k] = a[i];
        a[i] = t;
        i = i + 1;
    }
}
