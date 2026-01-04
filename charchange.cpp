#include <iostream>
#include <string>
using namespace std;

int main() {
    string word = "Hello";
    char choice;
    char oldChar, newChar;

    cout << "Original word: " << word << endl;
    cout << "Replace a letter in the word? (y/n)? ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        cout << "Enter the letter to replace: ";
        cin >> oldChar;
        cout << "Enter the new letter: ";
        cin >> newChar;

        for (size_t i = 0; i < word.length(); ++i) {
            if (word[i] == oldChar) {
                word[i] = newChar;
            } else if (word[i] == toupper(oldChar)) {
                word[i] = toupper(newChar);
            } else if (word[i] == tolower(oldChar)) {
                word[i] = tolower(newChar);
            } else if (newChar != oldChar) {
                // Do nothing, keep the original character
            } else {
                // Do nothing, keep the original character
            }
        } 

        cout << "Updated word: " << word << endl;
    } else {
        cout << "No changes made. Final word: " << word << endl;
        cout << "\nThank you for using the character replacement program!" << endl;
    }
    char repeat;
    cout << "\nRun the program again? (y/n): ";
    cin >> repeat;
    if (repeat == 'y' || repeat == 'Y') {
        cout << endl;
        return main();
    }
    return 0;
}
