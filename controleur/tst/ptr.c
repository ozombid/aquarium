#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>

struct sed {
    int val;
};

void change(struct sed * x)
{
    x->val++;
}

int main()
{
    struct sed n = {10};
    struct sed * a = &n;
    struct sed * b = a;
    printf(" n: %d, a: %d, b: %d \n",n.val, a->val, b->val);
    change(&n);
    printf(" n: %d, a: %d, b: %d \n",n.val, a->val, b->val);
    change(a);
    printf(" n: %d, a: %d, b: %d \n",n.val, a->val, b->val);
    change(b);
    printf(" n: %d, a: %d, b: %d \n",n.val, a->val, b->val);
    return 0;
}