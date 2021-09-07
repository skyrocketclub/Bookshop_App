// Bookshop_Log_App.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
 *                                                              ABOUT "THE BOOKSHOP APP"
 *
 * This program uses OO programming to create a Bookshop App
 *
 * Features of Version 1 of "The Bookshop App"
 *
 * 1.  Have a digital Inventory of all books currently available in your store (the titles and the number)
 * 2.  Add reviews to each of the books to aid customers make a choice...
 *
 * */
#include <iostream>
#include "Books.h"

using namespace std;

int main() {
    Books lakeside;
    cout << "\t\t\t==============WELCOME TO LAKESIDE BOOKSHOP===============" << endl;
    lakeside.display_menu();
    return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
