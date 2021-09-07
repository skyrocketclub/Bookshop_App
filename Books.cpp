#include "Books.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

/*
* Order of things to work on
* 
* 2. Let the book titles be displayed in an alphabetical order
* 1. Work on developing the search book function, the buy book, and the view review and the add review
* 3. Work on the option where the program shows the similar book titles upon search, where the user can easily select book of choice
* 
* 
        IMPLEMENTATION OF THE DATABASE USING FSTREAM FOR FILE HANDLING
    The fucntions where files will be accessed are as follows:
    
   (1) add_books-->( In this place you are editing the record
   (2) search_book 
   (3a)    Buy a Book--> You are editing the record in this place too(number of books)
   (3b)   Add a comment for a book--> You are also editing the record in this case...
   (4) list_books-->Here you simply display all books...

    Order in which info is stored in the file:

    std::vector<std::string> title{ " " };
    std::vector<std::string> author{ " " };
    std::vector<std::string> reviews{ " " };
    std::vector<std::string> publisher{ " " };
    std::vector<double>price{ 0.00 };
    std::vector<int> copies{ 0 };
    std::vector<int> position{ 0 };
*/
//All the methods that were defined in the public section are expounded upon here...
//This method is the key to the other methods and it displays the possible operations in the home screen anytime it is called

bool Books::check_book(string title) {

    string title_val{""};
    string author{""};
    string review{ "" };
    string publisher{ "" };
    string price{};
    string copies{};
    bool status = false;
    string line;

    ifstream in_file;
    in_file.open("record.txt");
    if (!in_file) {
        cerr << "Error Opening file" << endl;
    }


    while (std::getline(in_file, line)) {

        stringstream s_stream(line);
        string substr;
        vector<string> result;

        while (s_stream.good()) {
            std::getline(s_stream, substr, ',');
            result.push_back(substr);
        }
            title_val = result.at(0);
            author = result.at(1);
            review = result.at(2);
            publisher = result.at(3);
            price = result.at(4);
            copies = result.at(5);

            if (title_val == title) {
                status = true; //Means the book is avilable...
                break;
            }
    }

    in_file.close();
    return status;
}

void Books::update_copies(string title) {
    string title_val;
    string author;
    string review;
    string publisher;
    string line;
    string price;
    string copies;
    int copies_to_add{};


    ifstream in_file;

    in_file.open("record.txt");
    if (!in_file) {
        cerr << "Error Opening file" << endl;
    }

    while (std::getline(in_file, line)) {

        stringstream s_stream(line);
        string substr;
        vector<string> result;

        while (s_stream.good()) {
            std::getline(s_stream, substr, ',');
            result.push_back(substr);
        }
        title_val = result.at(0);
        author = result.at(1);
        review = result.at(2);
        publisher = result.at(3);
        price = result.at(4);
        copies = result.at(5);

        //Now the user has told you how many books that he wants to add...
        if (title_val == title) {
            std::cout << copies << " copies of " << title_val << " are in stock..." << endl;
            std::cout << "How many copies would you like to add?: ";
            std::cin >> copies_to_add;
            break;
        }
    }

    ofstream out_file("temp.txt");
    if (!out_file) {
        cerr << "Error opening file" << endl;
    }

    in_file.seekg(0,ios::beg);

    while (std::getline(in_file, line)) {

        stringstream s_stream(line);
        string substr;
        vector<string> result;

        while (s_stream.good()) {
            std::getline(s_stream, substr, ',');
            result.push_back(substr);
        }
        title_val = result.at(0);
        author = result.at(1);
        review = result.at(2);
        publisher = result.at(3);
        price = result.at(4);
        copies = result.at(5);

        if (title_val != title) {
            out_file << line << endl;
        }
        else {
            for (int i{ 0 }; i < 5;i++) {
                out_file << result.at(i) << ",";
            }
            int copy = stoi(copies);
            copy += copies_to_add;
            out_file << copy << endl;
        }  
    }
    std::cout << copies_to_add << " have been added successfully!" << endl;
   
    in_file.close();
    out_file.close();
    std::remove("record.txt");
    std::rename("temp.txt", "record.txt");
}

void Books::display_menu() {
    if (option != 'q') {
        std::cout << "\na -- Add Books\nl -- Get List of books in stock\ns -- Search for a book (Buy & Review Book)\nq -- Quit Application\n";
        std::cout << "Option: ";
        std::cin >> option;
        system("CLS");
        std::cout << endl;

        if (option != 'q') {
            switch (option) {
                case 'l':
                {
                    list_books();

                }
                break;

                case 'a':
                {
                    add_books();

                }
                break;

                case 's':
                {
                    search_books();

                }
                break;

                default:
                {
                    std::cout << "Please enter a valid option (In lower case)" << endl;
                    display_menu();

                }
            }
        }
        else {
            std::cout << "GOODBYE" << std::endl;
        }
       
    }
    else {
        quit();
    }
}

//THis method simpply gets a list of all the books currently in stock. It also states how many books are in stock
void Books::list_books() {
    
    string line, temp;
    vector <string> lines;
    string title_val, author, review, publisher, price, copies;
    ifstream in_file("record.txt");
    if (!in_file) {
        cerr << "Error opening file" << endl;
    }

    cout << setw(10) << "BOOK TITLE" << setw(21) << "AUTHOR" << setw(33) << "COPIES IN STOCK" << endl;
    std::cout << "================================================================="<<std::endl;
                  
                  
    while (std::getline(in_file, line)) {
        lines.push_back(line);
    }

    size_t l = lines.size();
    //The strings are arranged in a vector ready to be outputed
    for (size_t i{ 0 };i < l; i++) {
        for (size_t j{ i + 1 };j < l; j++) {
            if (lines.at(i) > lines.at(j)) {
                temp = lines.at(i);
                lines.at(i) = lines.at(j);
                lines.at(j) = temp;
            }
        }
    }

    for (size_t k{ 0 }; k < lines.size(); k++) {
        stringstream s_stream(lines.at(k));
        string substr;
        vector<string> result;

        while (s_stream.good()) {
            std::getline(s_stream, substr, ',');
            result.push_back(substr);
        }
        std::cout << std::setw(25) << left << result.at(0) << "by " << std::setw(15) << std::left << result.at(1) << setw(4) << left << result.at(5) << "  copies in stock\n";
    }
       /* string substr;
        vector<string> result;
     
        while (s_stream.good()) {
            std::getline(s_stream, substr, ',');
            result.push_back(substr);
        }
        std::cout << std::setw(25) << left << result.at(0) << "by " << std::setw(15) << std::left << result.at(1) << setw(4)<<left << result.at(5) << "  copies in stock\n";  
    */
    in_file.close();
    display_menu();
}

//This method when called opens up to part of the code that allows you to add new books or more copies of already existing books to the collection
void Books::add_books() {
    bool instock{};
    int num{ 0 }, copies_val{ 0 }, counts{ 0 }, copies_to_add{ 0 };
    double price_val{ 0.0 };
    string title_val{}, author_val{}, publisher_val{};
    size_t q{ 0 };

    ofstream out_file("record.txt",ios::app);
    if (!out_file) {
        cerr << "error opening file" << endl;
    }
    
    std::cout << "How many books will you like to add:  ";
    std::cin >> num;
    std::cin.ignore(1, '\n');
    total_num += num;


    //The first for loop enables the user the opportunity of num inputs...

    for (int i{ 0 }; i < num; i++) {
        std::cout << "\n\t\tBook " << i + 1;

        std::cout << endl;
        std::cout << "Title (Ensure complete and accurate input): ";
        std::getline(std::cin, title_val);
        title_val = capitalise(title_val);
        instock = check_book(title_val);


        if (instock==true) { //It means Book is already in stock... (do this in second place)
            out_file.close();
            update_copies(title_val);
            out_file.open("record.txt", ios::app);
            continue;
        }

        else {
            title.push_back(title_val);
            out_file << title_val << ","; //one

            std::cout << "Author (Surname first): ";
            std::getline(std::cin, author_val);
            author_val = capitalise(author_val);
            author.push_back(author_val);
            out_file << author_val <<",";//two

            reviews.push_back("No reviews yet... ");
            out_file << "No reviews yet " << ","; //three

            std::cout << "Publisher: ";
            std::getline(std::cin, publisher_val);
            publisher.push_back(publisher_val);
            out_file << publisher_val << ","; //four

            std::cout << "Selling Price: N ";
            std::cin >> price_val;
            price.push_back(price_val);
            std::cin.ignore(1, '\n');
            out_file << price_val <<",";//five


            std::cout << "How many copies are you adding?: ";
            std::cin >> copies_val;
            copies.push_back(copies_val);
            std::cin.ignore(1, '\n');
            std::cout << endl;
            out_file << copies_val << endl;//six
        }
        std::cout << title_val << " successfully added!" << std::endl;
    }
    out_file.close();
    display_menu();
}

//This is the most elaborated method in this code, although it is subject to review in the second version...
//In this section, you can buy a book, or even add reviews to books...



void Books::buy_book(std::string title) {
    string line{};
    string title_val{}, author{}, review{}, publisher{}, price{}, copies{};
    int copy{}, quantity{}, remainder{}, book_price{};
    double total_cost{};
    char buy{};

    ifstream in_file("record.txt");
    if (!in_file) {
        std::cerr << "Error opening file" << std::endl;
    }

    while (std::getline(in_file, line)) {

        stringstream s_stream(line);
        string substr;
        vector<string> result;

        while (s_stream.good()) {
            std::getline(s_stream, substr, ',');
            result.push_back(substr);
        }
        title_val = result.at(0);
        author = result.at(1);
        review = result.at(2);
        publisher = result.at(3);
        price = result.at(4);
        copies = result.at(5);

        //You arrive at the book you want to buy.
        if (title_val == title) {
            copy = stoi(copies);
            book_price = stoi(price);
            std::cout << "There are " << copies << " copies of " << title_val << " available" << std::endl;
            std::cout << "How many copies would you like to buy?\nQuantity: ";
            std::cin >> quantity;
            total_cost = quantity * book_price;

            if (copy >= quantity) {
            
               
                std::cout << "Cost: N " <<std::showpoint<< total_cost << std::endl;
                std::cout << "WOULD YOU LIKE TO CONTINUE WITH THIS TRANSACTION?\n1 - Yes\n0 - No\nOption: ";
                std::cin >> buy;
                if (buy == '1') {
                    remainder  = copy - quantity;
                    std::cout << "Thanks for your patronage!" << std::endl;
                }
                else if (buy == '0') {
                    remainder = copy;
                    
                }
            }
            else {
                remainder = copy;
                std::cout << "Sorry there are only " << copies << " copies of " << title_val << " available" << std::endl << std::endl;
                buy_book(title_val);
            }
            break;
        }
    }

    ofstream out_file("temp.txt");
    if (!out_file) {
        cerr << "Error opening file" << endl;
    }

    in_file.seekg(0, ios::beg);

    while (std::getline(in_file, line)) {

        stringstream s_stream(line);
        string substr;
        vector<string> result;

        while (s_stream.good()) {
            std::getline(s_stream, substr, ',');
            result.push_back(substr);
        }
        title_val = result.at(0);
        author = result.at(1);
        review = result.at(2);
        publisher = result.at(3);
        price = result.at(4);
        copies = result.at(5);

        if (title_val != title) {
            out_file << line << endl;
        }
        //It means the book you bought---
        else {
            if (remainder > 0) {
                for (int i{ 0 }; i < 5; i++) {
                    out_file << result.at(i) << ",";
                }
                out_file << remainder << std::endl;
            }
            else {
            }

        }
    }
    in_file.close();
    out_file.close();
    remove("record.txt");
    rename("temp.txt", "record.txt");
}



void Books::edit_review(std::string title) {
    string line{};
    string title_val{}, author{}, review{}, publisher{}, price{}, copies{};
}

void Books::read_review(std::string title) {
    string line{};
    string title_val{}, author{}, review{}, publisher{}, price{}, copies{};
}

void Books::search_books() {
  
    string search_title{};
    char choice{}, choice_q{};
    system("CLS");
    std::cout << "Enter the title of the book" << std::endl;
    std::cout << "Title: ";
    std::cin.ignore(1, '\n');
    std::getline(std::cin, search_title);

    search_title = capitalise(search_title);

    bool status = check_book(search_title);
    if (status == true) {
        std::cout << "1 - Buy Book\n2 - Edit Review\n3 - Add a review\nOption: " << std::endl;
        std::cin >> choice;
        switch (choice) {
        case '1':
        {
            buy_book(search_title);
            display_menu();
        }
        break;

        case '2':
        {
            edit_review(search_title);
            display_menu();
        }
        break;
        case '3':
        {
            read_review(search_title);
            display_menu();
        }
        break;
        default:
        {
            std::cout << "Kindly enter a valid option" << std::endl;
        }
        }
    }
    else {
        std::cout << search_title << " is not available" << std::endl;
        std::cout << "1 - Home menu\n2 - Search another book\n3 - Quit application\n";
        std::cout << "Option: ";
        std::cin >> choice_q;
        switch (choice_q) {
        case '1':
            {
            display_menu();
            }
            break;

        case '2':
            {
            search_books();
            }
            break;
        case '3':
            {
            quit();
            }
            break;
        default:
            {
            std::cout << "Kindly enter a valid option";
            }
        }
    }
}


void Books::quit() {
    std::cout << "GoodBye" << endl;
    option = 'q';
}

string Books::capitalise(string input) {
    string reform{};
    for (auto letters : input) {
        letters = toupper(letters);
        reform += letters;
    }
    input = reform;
    return input;
}


/*
*                                 THINGS TO IMPLEMENT IN THE NEXT VERSION OF "THE BOOKSHOP APP"
* 1.  I can search for books using :the author-----the title--------the book number: and the same options of buying or reviewing
* the book come up
* 2.  After i see that the number of books are not enough, the user can be given options to either
*             Buy a different number of units
*             Search for another book
*             return to home
* 3. Implement exception handling for all the places where the user has to enter input... And test all
* 4. Extract a method called SEARCH  and have another method called BUY that have tailored and specific functions
* 5. Expound on the buying  method keeping in mind that a customer can want to buy more than one book at once and this
*     is often the case...
* 6. Create a feature that alerts the librarian on the Books that are currently getting out of stock or that are out of stock completely...
* 7. Have an option at all aspects of the program that enable the user to go back
* 8. Have an option where you can output the total sales of the day
* 9. When searching for the book, try to make it easier for the user to put in some part of the title...
* */
