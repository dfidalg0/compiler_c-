int test(int a) {
    return a + 1;
}

int main(void) {
    int a;
    a = test(10);

    if (a) {
        return 1;
    }

    return a;
}
