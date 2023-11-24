#include <stdio.h>
#include "model.h"

const __uint8_t fast_sine_table[91] = {0, 4, 8, 13, 17, 22, 26, 31, 35, 39, 44, 48, 53, 57, 61, 65, 70, 74, 78, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119, 123, 127, 131, 135, 138, 142, 146, 149, 153, 156, 160, 163, 167, 170, 173, 177, 180, 183, 186, 189, 192, 195, 198, 200, 203, 206, 208, 211, 213, 216, 218, 220, 223, 225, 227, 229, 231, 232, 234, 236, 238, 239, 241, 242, 243, 245, 246, 247, 248, 249, 250, 251, 251, 252, 253, 253, 254, 254, 254, 254, 254, 255};
int fast_sin(int degree){
    int answer = 0;
    int needFlip = 0;
    if(degree > 180 || (degree < 0 && degree > -180)){ needFlip = 1; }

    degree = abs(degree%180);
    
    if(degree>=0 && degree <=90){
        answer = fast_sine_table[degree];
    }else if(degree > 90 && degree <= 180){
        answer = fast_sine_table[180-degree];
    }

    if(needFlip){
        answer *= -1;
    }
    return answer;
}

int main () {
    float x  = vertices[0][0];

    printf("%d\n\n", fast_sin(-361));

    return 0;
}