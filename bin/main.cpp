#include "lib/ccircularbuffer.h"
#include "lib/ccircularbufferext.h"

int main() {
    CCircularBuffer<int> buffer(2);
    buffer.push_back(1);
    buffer.push_front(0);
    buffer.push_front(-1);

    for (int i = 0; i < 3; ++i) {
        std::cout << buffer[i] << ' ';
    }
}