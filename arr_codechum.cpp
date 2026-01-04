#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int main() {
    cout << "Enter an integer: ";
    int n;
    if (!(cin >> n) || n <= 0) {
        cerr << "Invalid input: please enter a positive integer." << endl;
        return 1;
    }

    // multiplication table using vector for safe memory management
    cout << "Multiplication Table" << endl;
    vector<vector<int>> table(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            table[i][j] = (i + 1) * (j + 1);
        }
    }

    // compute column width for neat alignment
    int maxVal = n * n;
    int width = 1;
    while (maxVal >= 10) { width++; maxVal /= 10; }
    width += 1; // extra spacing

    // print header
    cout << setw(width) << "";
    for (int c = 1; c <= n; ++c) {
        cout << setw(width) << c;
    }
    cout << endl;

    // print rows
    for (int i = 0; i < n; ++i) {
        cout << setw(width) << (i + 1);
        for (int j = 0; j < n; ++j) {
            cout << setw(width) << table[i][j];
        }
        cout << endl;
    }

    return 0;
}