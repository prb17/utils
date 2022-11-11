#include "container.hh"
#include <cassert>

using namespace pbrady::utils::structures;

void testGreaterThan() {
    container<int> c1;
    container<int> c2;
    c1 = 5;
    c2 = 3;
    assert(c1 > c2);
    assert(c1 > -1);

    c2 = 5;
    assert(c1 >= c2);
    assert(c2 >= 5);
    assert(c2 >= -2);

}

void testLessThan() {
    container<int> c1;
    container<int> c2;
    c1 = 1;
    c2 = 3;
    assert(c1 < c2);
    assert(c2 < 10);

    c1 = 3;
    assert(c1 <= c2);
    assert(c1 <= 3);
    assert(c1 <=10);
}

void testEquals() {
    container<int> c1{5};
    container<int> c2{5};
    assert(c1 == c2);

    c2 = 10;
    assert(c1 != c2);
}

int main() {
    testEquals();
    testLessThan();
}