#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Item {
    string name;
    float price;
};

struct CartItem {
    string name;
    float price;
    int quantity;
};

int main() {
    srand(time(0));

    bool isStudent = false;
    bool hasCoupon = false;
    bool goCheckout = false;

    float wallet = 500;
    float taxRate = 0.12;

    CartItem cart[20];
    int cartCount = 0;

    //to edit -- essentials, drinks, foods, school, clothing
    Item essentials[] = {
        {"Toothpaste", 80},
        {"Soap", 40},
        {"Shampoo", 120}
    };

    Item school[] = {
        {"Notebook", 50},
        {"Ballpen", 15},
        {"Backpack", 450}
    };

    Item clothing[] = {
        {"T-Shirt", 250},
        {"Jeans", 600},
        {"Jacket", 900}
    };

    cout << "Welcome to Beta Mart!\n";
    cout << "Are you a student? (Y/N): ";
    char ch;
    cin >> ch;

    if (ch == 'Y' || ch == 'y') {
        isStudent = true;
        cout << "Student discount available!\n";
    }

    // Random coupon chance
    if (rand() % 2 == 0) {
        hasCoupon = true;
        cout << "🎉 Congratulations! You received a 20% School Supplies Coupon!\n";
    }

    int choice;
    do {
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Shop for necessities\n";
        cout << "2. Buy school supplies\n";
        cout << "3. Head to clothing section\n";
        cout << "4. Check wallet\n";
        cout << "5. Proceed to Checkout\n";
        cout << "6. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        Item* selected = nullptr;
        int size = 0;

        switch (choice) {
            case 1:
                selected = essentials;
                size = 3;
                cout << "\nCart heading to... Section 1 – Oral Care & Hygiene\n";
                break;

            case 2:
                selected = school;
                size = 3;
                cout << "\nCart heading to... Section 2 – School Supplies\n";
                break;

            case 3:
                selected = clothing;
                size = 3;
                cout << "\nCart heading to... Section 3 – Clothing\n";
                break;

            case 4:
                cout << "\nMoney: ₱" << wallet << endl;
                cout << "Coupon: " << (hasCoupon ? "Available" : "None") << endl;
                continue;

            case 5:
                if (cartCount == 0) {
                    cout << "Your cart is empty. Add items before checkout.\n";
                } else {
                    goCheckout = true;
                }
                break;

            case 6:
                cout << "Come again!\n";
                return 0;

            default:
                cout << "Invalid choice.\n";
                continue;
        }

        if (choice >= 1 && choice <= 3) {
            cout << "\nAvailable Items:\n";
            for (int i = 0; i < size; i++) {
                cout << i + 1 << ". " << selected[i].name
                     << " - ₱" << selected[i].price << endl;
            }

            int pick, qty;
            cout << "Select item (0 to cancel): ";
            cin >> pick;

            if (pick > 0 && pick <= size) {
                cout << "Quantity: ";
                cin >> qty;

                cart[cartCount++] = {
                    selected[pick - 1].name,
                    selected[pick - 1].price,
                    qty
                };

                cout << "Item added to cart.\n";
            }
        }

    } while (!goCheckout);

    // -------- CHECKOUT --------
    float subtotal = 0;
    for (int i = 0; i < cartCount; i++) {
        subtotal += cart[i].price * cart[i].quantity;
    }

    float discount = 0;
    if (isStudent) discount += subtotal * 0.10;
    if (hasCoupon) discount += subtotal * 0.10;

    float tax = (subtotal - discount) * taxRate;
    float total = subtotal - discount + tax;

    cout << "\n--- CASHIER ---\n";
    cout << "Subtotal: ₱" << subtotal << endl;
    cout << "Discount: ₱" << discount << endl;
    cout << "Tax: ₱" << tax << endl;
    cout << "TOTAL: ₱" << total << endl;

    if (wallet < total) {
        cout << "Insufficient funds. Please remove items.\n";
        return 0;
    }

    float payment;
    cout << "Enter payment amount: ";
    cin >> payment;

    if (payment < total) {
        cout << "Not enough cash.\n";
        return 0;
    }

    float change = payment - total;

    // -------- RECEIPT --------
    ofstream receipt("receipts.txt", ios::app);
    receipt << "\n--- BETA MART RECEIPT ---\n";
    for (int i = 0; i < cartCount; i++) {
        receipt << cart[i].name << " x" << cart[i].quantity << endl;
    }
    receipt << "Tax: ₱" << tax << endl;
    receipt << "Total: ₱" << total << endl;
    receipt << "Change: ₱" << change << endl;
    receipt << "------------------------\n";
    receipt.close();

    cout << "\nThank you for shopping with Beta Mart!\n";
    cout << "You received a 10% Beta Meals Discount Coupon for your next visit! Come again soon!\n";

//checkout glitch
//fix later
//insufficient funds feature
//item removal feature
//receipt formatting
//add more items to cart
//buy promos or use coupons feature


    return 0;
}
