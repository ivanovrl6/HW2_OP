#include <iostream>
#include "hw_2_h.h"
int main() {
    ImageProcess fir("hw2.txt");
    fir.dilatation(1);
    return 0;
}
