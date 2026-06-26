#include "llist.h"
#include <iostream>

struct n {
    int a;
    n* both;
};

int main()
{
    n n1{10, nullptr};
    n n2{20, nullptr};
    n n3{30, nullptr};

    n1.both = &n2;
    n2.both = (&n1) ^ (&n3);

    return 0;
}