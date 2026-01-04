#include <iostream>
using namespace std;

int main() {
    int n;

   //sum of digits
    cout << "Enter a positive integer: ";
    cin >> n;
    int sum = 0;
    int temp = n;
    while (temp != 0) {
        sum += temp % 10;
        temp /= 10;
    }

    cout << "Sum of digits: " << sum << endl;

    return 0;
}