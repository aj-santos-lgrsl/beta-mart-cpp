#include <iostream>
using namespace std;

int main() {
    //factors of a number
    int n;
    cout << "Enter an integer: ";
    cin >> n;
    cout << "Factors of " << n << " : ";
    int i = 1;
    do {
        if (n % i == 0) {
            cout << i << " ";
        }
        i++;
    } while (i <= n);
    cout << endl;
 
 
    return 0;
}