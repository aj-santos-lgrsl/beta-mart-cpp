#include <iostream>
using namespace std;

int main() {
    //digit difference from right to left
    int n;
    cout << "Enter a positive integer: ";
    cin >> n;

    int result = 0;
    int place = 1;
    int prevDigit = -1;
    for (; n > 0; n /= 10) {
        int currentDigit = n % 10;
        if (prevDigit != -1) {
            result += (currentDigit - prevDigit) * place;
            place *= 10;
        }
        prevDigit = currentDigit;
    }

    cout << "Result of subtracting digits: " << result << endl;



    return 0;
}