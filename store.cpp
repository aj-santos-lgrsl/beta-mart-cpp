#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

//STRUCTS
struct Product {
    string name;
    string section;
    double price;
    int stock;
};

struct CartItem {
    Product product;
    int quantity;
};

//UTILITIES
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

//WALLET
void checkWallet(double cash, double ewallet, vector<string>& coupons) {
    cout << "\n--- WALLET ---\n";
    cout << "Cash Balance: " << cash << " PHP\n";
    cout << "E-Wallet Balance: " << ewallet << " PHP\n";

    if (coupons.empty())
        cout << "Coupons: none\n";
    else {
        cout << "Coupons: ";
        for (auto& c : coupons)
            cout << c << " | ";
        cout << endl;
    }
}

//DISPLAY SECTION
vector<int> displaySection(vector<Product>& products, const string& section) {
    cout << "\n--- Section: " << section << " ---\n";

    vector<int> indices;
    int num = 1;

    for (size_t i = 0; i < products.size(); i++) {
        if (products[i].section == section) {
            cout << num++ << ". " << products[i].name
                 << " - " << fixed << setprecision(2)
                 << products[i].price << " PHP"
                 << " (Stock: " << products[i].stock << ")\n";
            indices.push_back(i);
        }
    }

    if (indices.empty())
        cout << "No items available.\n";

    return indices;
}

//SEARCH
void searchProduct(vector<Product>& products) {
    cin.ignore();
    string item;
    cout << "Enter item name to search: ";
    getline(cin, item);

    string key = toLower(item);

    for (auto& p : products) {
        if (toLower(p.name) == key) {
            cout << "Item found in " << p.section
                 << " | Price: " << p.price
                 << " PHP | Stock: " << p.stock << endl;
            return;
        }
    }
    cout << "Item not found.\n";
}

//CART
void viewCart(vector<CartItem>& cart) {
    if (cart.empty()) {
        cout << "Your cart is empty.\n";
        return;
    }

    cout << "\n--- Your Cart ---\n";
    for (auto& c : cart)
        cout << c.product.name << " x" << c.quantity
             << " - " << c.product.price * c.quantity << " PHP\n";
}

//RECEIPT (screen and file)
void printReceipt(vector<CartItem>& cart, double subtotal, double discount,
                  double tax, double total, double cashPaid) {

    ofstream file("receipts.txt", ios::app);

    auto out = [&](ostream& os) {
        os << "\n------------------------------------ RECEIPT -------------------------------------\n";
        os << "\n------------------------------- 500 BETA MART SHOP --------------------------------\n";
        os << "---------------------------- Owned and Operated by Group 2 --------------------------\n";
        os << "------------------- 1CSIT Inc., Colegio de San Juan de Letran Calamba -----------------\n\n";
        os << "Date: 2026\n\n";

        os << left << setw(30) << "Item & Qty."
           << right << setw(10) << "Price\n";

        for (auto& c : cart)
            os << left << setw(30)
               << c.product.name + " x" + to_string(c.quantity)
               << right << setw(10)
               << c.product.price * c.quantity << endl;

        os << "Tax: " << tax << " PHP\n";
        os << "\nDiscount: " << discount << " PHP\n";
        os << "Total: " << total << " PHP\n";

        if (cashPaid > 0) {
            os << "Cash: " << cashPaid << " PHP\n";
            os << "Change: " << cashPaid - total << " PHP\n";
        }

        os << "\n------------------ Thank you for shopping at 500 BETA MART SHOP! ---------------------\n";
    };

    out(cout);
    out(file);
    file.close();
}

//CHECKOUT
void checkout(vector<CartItem>& cart, double& cash, double& ewallet,
              bool isStudent, vector<string>& coupons) {

    if (cart.empty()) {
        cout << "Your cart is empty.\n";
        return;
    }

    double subtotal = 0;
    for (auto& c : cart)
        subtotal += c.product.price * c.quantity;

    double discount = isStudent ? subtotal * 0.15 : 0;
    double tax = (subtotal - discount) * 0.10;
    double total = subtotal - discount + tax;

    cout << "\n-- Checkout --\n";
    cout << "Subtotal: " << subtotal << " PHP\n";
    cout << "Tax: " << tax << " PHP\n";
    cout << "Discount: " << discount << " PHP\n";
    cout << ">>>Total: " << total << " PHP\n";
    cout << "\n1. Cash\n2. E-Wallet\nChoice: ";
    int mode;
    cin >> mode;

    if (mode == 1) {
        if (cash < total) {
            cout << "Insufficient cash.\n";
            return;
        }

        double paid;
        cout << "Enter cash amount: ";
        cin >> paid;

        if (paid < total) {
            cout << "Not enough cash.\n";
            return;
        }
        cash -= total;
        cout << "Cash payment successful. Enjoy this Beta Meals Coupon!\n";
        printReceipt(cart, subtotal, tax, discount, total, paid);
    }
    else if (mode == 2) {
        if (ewallet < total) {
            cout << "Insufficient e-wallet balance.\n";
            return;
    }
        string number, pin;
        cout << "Enter 11-digit E-Wallet number: ";
        cin >> number;
        cout << "Enter 4-digit PIN: ";
        cin >> pin;
        if (number.length() != 11 || pin.length() != 4) {
            cout << "Invalid E-Wallet credentials.\n";
            return;
        }
    // apply Beta Meals 10% coupon if present (applies only to items in "Beta Meals")
    auto it = find(coupons.begin(), coupons.end(), "Beta Meals 10% Discount");
    if (it != coupons.end()) {
        double betaSubtotal = 0.0;
        for (auto& c : cart) {
            if (c.product.section == "Beta Meals")
                betaSubtotal += c.product.price * c.quantity;
        }
        if (betaSubtotal > 0.0) {
            double couponDiscount = betaSubtotal * 0.10; // 10% off Beta Meals items
            discount += couponDiscount;                 // include in total discount
            tax = (subtotal - discount) * 0.10;         // recompute tax on new taxable base
            total = subtotal - discount + tax;          // recompute total
            cout << "Coupon applied: Beta Meals 10% Discount (-" << fixed << setprecision(2)
                 << couponDiscount << " PHP)\n";
            coupons.erase(it); // consume the coupon
        } else {
            cout << "Coupon available but no Beta Meals items in cart. Coupon not applied.\n";
        }
    }
        ewallet -= total;
        cout << "E-Payment confirmed. Thank you for your purchase. Have your Beta Meals Coupon!\n";
        printReceipt(cart, subtotal, tax, discount, total, 0);
    }

    coupons.push_back("Beta Meals 10% Discount");
    cart.clear();
}

//MAIN
int main() {

    vector<Product> products = {
        {"Toothpaste", "Basic Essentials", 50, 20},
        {"Shampoo Sachet", "Basic Essentials", 10, 15},
        {"Mouthwash Bottle", "Basic Essentials", 100, 25},
        {"Alcohol", "Basic Essentials", 45, 30},
        {"Tissue", "Basic Essentials", 20, 50},
        {"Cotton Buds", "Basic Essentials", 10, 100},
        {"Sanitary Napkins", "Basic Essentials", 30, 50},
        {"Bath Soap", "Basic Essentials", 25, 75},
        {"Cotton Balls", "Basic Essentials", 15, 100},
        {"Face Mask", "Basic Essentials", 10, 100},
        {"Hand Sanitizer", "Basic Essentials", 50, 30},
        {"Rexona Deodorant Sachet", "Basic Essentials", 12, 20},

        {"Coke 1.5L", "Drinks", 40, 50},
        {"Sprite 1.5L", "Drinks", 40, 50},
        {"Royal 1.5L", "Drinks", 40, 50},
        {"Gatorade No Sugar Citrus", "Drinks", 50, 30},
        {"Yakult", "Drinks", 15, 20},
        {"Mineral Water", "Drinks", 20, 100},
        {"Dutch Mill", "Drinks", 25, 50},
        {"C2 Green Tea", "Drinks", 30, 40},
        {"C2 Red Tea", "Drinks", 30, 40},
        {"Cobra", "Drinks", 35, 30},
        {"Kopiko Bottle", "Drinks", 25, 50},

        {"San Marino Corned Tuna", "Foods", 40, 50},
        {"Century Tuna", "Foods", 40, 25},
        {"Argentina Beef Loaf", "Foods", 30, 25},
        {"Mega Sardines", "Foods", 25, 100},
        {"Hekkaido", "Foods", 40, 30},
        {"Purefoods Luncheon Meat", "Foods", 60, 40},
        {"CDO Carne Norte", "Foods", 50, 30},
        {"Pancit Canton", "Foods", 20, 50},
        {"Lucky Me! Instant Noodles", "Foods", 15, 100},
        {"Piattos", "Foods", 18, 100},
        {"Chips Ahoy!", "Foods", 25, 100},
        {"Oreo", "Foods", 30, 100},
        {"Muncher", "Foods", 20, 100},
        {"Nissin Wafers", "Foods", 15, 100},
        {"Skyflakes", "Foods", 10, 100},
        {"Rebisco", "Foods", 10, 100},

        {"Notebook", "School Supplies", 30, 50},
        {"Pen", "School Supplies", 10, 100},
        {"Pencil", "School Supplies", 8, 100},
        {"Yellow Pad", "School Supplies", 30, 100},
        {"Eraser", "School Supplies", 5, 100},
        {"Glue", "School Supplies", 25, 100},
        {"Scissors", "School Supplies", 35, 100},
        {"Tape", "School Supplies", 15, 100},
        {"Correction Tape", "School Supplies", 25, 100},

        {"T-Shirt", "Clothing", 150, 25},
        {"Jeans", "Clothing", 300, 10},
        {"Jacket", "Clothing", 450, 5},
        {"Towel", "Clothing", 40, 50},
        {"Socks", "Clothing", 20, 100},
        {"Cap", "Clothing", 100, 20},

        {"Tipid Siomai Rice", "Beta Meals", 60, 20},
        {"Hot Dog Giant", "Beta Meals", 60, 20},
        {"Siopao Sarap", "Beta Meals", 60, 20},
    };

    vector<CartItem> cart;
    vector<string> coupons;

    double cash = 500, ewallet = 500;
    bool isStudent = false;

    cout << "Welcome to 500 BETA MART SHOP!\n";

    char s;
    cout << "Are you a student? (Y/N): ";
    cin >> s;

    if (s == 'Y' || s == 'y') {
        string id;
        cout << "Enter 7-digit Student ID Number: ";
        cin >> id;

        if (id.length() == 7) {
            isStudent = true;
            cout << "Congratulations! 15% Student Discount available.\n";
        } else {
            cout << "Invalid Student ID. No discount will be applied.\n";
        }
    }

    int choice;
    do {
        cout << "\n--- MAIN MENU ---\n";
        cout << "SECTION 1. Basic Essentials\n";
        cout << "SECTION 2. Drinks\n";
        cout << "SECTION 3. Foods\n";
        cout << "SECTION 4. School Supplies\n";
        cout << "SECTION 5. Clothing\n";
        cout << "SECTION 6. Beta Meals\n";
        cout << "7. Search Item\n";
        cout << "8. Check Wallet\n";
        cout << "9. View Cart\n";
        cout << "10. Remove Item from Cart\n";
        cout << "11. Checkout\n";
        cout << "12. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 6) {
            string section =
                (choice == 1) ? "Basic Essentials" :
                (choice == 2) ? "Drinks" :
                (choice == 3) ? "Foods" :
                (choice == 4) ? "School Supplies" :
                (choice == 5) ? "Clothing" :
                                "Beta Meals";

            auto valid = displaySection(products, section);

            int pick, qty;
            cout << "Enter item number (0 to cancel): ";
            cin >> pick;

            if (pick > 0 && pick <= (int)valid.size()) {
                cout << "Quantity: ";
                cin >> qty;

                int idx = valid[pick - 1];
                if (qty <= products[idx].stock) {
                    products[idx].stock -= qty;
                    cart.push_back({products[idx], qty});
                } else {
                    cout << "Not enough stock.\n";
                }
            }
        }
        else if (choice == 7) searchProduct(products);
        else if (choice == 8) checkWallet(cash, ewallet, coupons);
        else if (choice == 9) viewCart(cart);
        else if (choice == 10) {
            // Remove Item from cart
            if (cart.empty()) {
                cout << "\nYour cart is empty. Nothing to remove.\n";
            } else {
                cout << "\n--- Remove Item from Cart ---\n";
                for (size_t i = 0; i < cart.size(); ++i) {
                    cout << (i + 1) << ". " << cart[i].product.name
                         << " x" << cart[i].quantity
                         << " - " << cart[i].product.price * cart[i].quantity << " PHP\n";
                }
                cout << "Enter item number to remove (0 to cancel): ";
                int rem;
                cin >> rem;
                if (rem > 0 && rem <= (int)cart.size()) {
                    // restore stock in products list if matching product found
                    string removeName = cart[rem - 1].product.name;
                    int removeQty = cart[rem - 1].quantity;
                    bool restored = false;
                    for (auto& p : products) {
                        if (p.name == removeName && p.section == cart[rem - 1].product.section) {
                            p.stock += removeQty;
                            restored = true;
                            break;
                        }
                    }
                    cart.erase(cart.begin() + (rem - 1));
                    cout << "\nItem removed from cart";
                    if (restored) cout << " and stock restored.\n";
                    else cout << ".\n";
                } else if (rem == 0) {
                    cout << "Canceling removal.\n";
                } else {
                    cout << "Invalid selection.\n";
                }
            }
        }
        else if (choice == 11) checkout(cart, cash, ewallet, isStudent, coupons);

    } while (choice != 12);

    cout << "\nThank you for shopping at 500 BETA MART SHOP!\n";
    cout << "You received a 10% Beta Meals Discount Coupon for your next visit! Come again soon!\n";
    return 0;
}
