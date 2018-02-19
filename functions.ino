#include <typedef.h>

 bool debounce(int pin, int debounceLimit, int previous) {

     static int debounceLimitIncrement;
     int current = digitalRead(pin);
        if(previous != current) {
            debounceLimitIncrement = 0;
            previous = current;
        }
        if(previous == current) {
            debounceLimitIncrement++;
        }
        if(debounceLimitIncrement >= debounceLimit) {
            return true;
        } else {
            debounce(pin,debounceLimit,previous);
        }
 }
