#include <iostream>
using namespace std;

int main() {
    int n;

    cout << "Enter a positive integer: ";
    cin >> n;

    //digit counter
    int count = 0;
    int temp = n;
    while (temp != 0) {
        temp /= 10;
        count++;
    }
    cout << "Number of digits: " << count << endl;

    return 0;
}