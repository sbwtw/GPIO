
#include <iostream>
#include <unistd.h>

#include "gpio.h"

int main() {

    // g1 for output
    GPIO g1(2, GPIO::OUT);
    // g1 for input
    GPIO g2(3, GPIO::IN);

    // set g1 to low
    g1.low();

    // read g2 status
    bool status_g2 = g2.value();

    // write 1 times high->low change
    g1.down();

    // write 50 times low->high change
    g1.up(50);

    return 0;
}
