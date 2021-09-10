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

//This method checks if a book is available
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

//This method is called when the book the user wants to add already exists. (only copies is updated)
void Books::update_copies(string title) {
    string title_val;
    string author;
    string review;
    string publisher;
    string line;
    string price;
    string copies;
    //For Data validation
    bool done{ false };
    std::string entry{};
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
            do {
                std::cout << "How many copies would you like to add?: ";
                std::cin >> entry;
                std::stringstream validator{ entry };
                if (validator >> copies_to_add) {
                    done = true;
                }
                else {
                    std::cout << "Kindly enter an integer" << std::endl;
                    std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
                }
            } while (!done);
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

//This is the function that shows the books with similar titles upon search
std::vector <std::string> Books::is_substr(std::string search_title) {
        
    string title_val;
    string author;
    string review;
    string publisher;
    string line;
    string price;
    string copies;
    vector <string> clues;
    bool status = false;

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

        int m = search_title.length();
        int n = title_val.length();
        int num{0};

        for (int i = 0; i <= n - m; i++) {
            for (int j{ 0 }; j < m; j++) {
                if (title_val[j] != search_title[j]) {
                    break;
                } 
                num++;
            }
            if (num == m) {
                status = true;
                clues.push_back(title_val);
            }
            
        }
      
    }

    in_file.close();
    return clues;
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
    int book_count{0};
    string line, temp;
    vector <string> lines;
    string title_val, author, review, publisher, price, copies;
    ifstream in_file("record.txt");
    if (!in_file) {
        cerr << "You have an empty stock, kindly add books first" << endl;
    }
    else {
       

        while (std::getline(in_file, line)) {
            lines.push_back(line);
            book_count++;
        }

        if (book_count == 0) {
            std::cout << "You have no book in stock" << std::endl;
        }
        else {
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
                std::cout << std::setw(25) << left << result.at(0) << "by " << std::setw(25) << std::left << result.at(1) << setw(3) << left << result.at(5) << std::setw(23) << std::left << "  copies in stock" << setw(2) << left << "N " << setw(8) << result.at(4) << std::endl;
            }
        }
    }
    in_file.close();
    display_menu();
}

//This method when called opens up to part of the code that allows you to add new books or more copies of already existing books to the collection
void Books::add_books() {
    bool instock{}, done{ false }, done_price{ false };
    int num{ 0 }, copies_val{ 0 }, counts{ 0 }, copies_to_add{ 0 };
    double price_val{ 0.0 };
    string title_val{}, author_val{}, publisher_val{}, entry{};
    size_t q{ 0 };

    ofstream out_file("record.txt",ios::app);
    if (!out_file) {
        cerr << "error opening file" << endl;
    }
    do {
        std::cout << "How many books will you like to add:  ";
        std::cin >> entry;
        std::istringstream validator{ entry };
        if (validator >> num) {
            done = true;
        }
        else {
            std::cout << "Kindly enter a valid input" << std::endl;
            std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
        }

    } while (!done);
   
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
            do {
                std::cout << "Selling Price: N ";
                std::cin >> entry;
                istringstream price_validator{ entry };
                if (price_validator >> price_val) {
                    done_price = true;
                }
                else {
                    std::cout << "Kindly enter a valid input" << std::endl;
                    std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
                }

            } while (!done_price);

           
            price.push_back(price_val);
            std::cin.ignore(1, '\n');
            out_file << price_val <<",";//five

            bool done_copy{ false };
            do {
                std::cout << "How many copies are you adding?: ";
                std::cin >> entry;
                istringstream copy_validator{ entry };
                if (copy_validator >> copies_val) {
                    done_copy = true;
                } 
                else{
                    std::cout << "Kindly enter a valid input" << std::endl;
                    std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
                }

            } while (!done_copy);
           
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
            
            bool done_quantity{ false };
            std::string entry;
            do {
                std::cout << "How many copies would you like to buy?\nQuantity: ";
                std::cin >> entry;

                istringstream quan_validator{ entry };
                if (quan_validator >> quantity) {
                    done_quantity = true;
                }
                else {
                    std::cout << "Kindly enter a valid input" << std::endl;
                    std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
                }
            } while (!done_quantity);

            
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
                else {
                    std::cout << "You entered an invalid input" << std::endl;
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
    std::remove("record.txt");
    std::rename("temp.txt", "record.txt");
}

void Books::edit_review(std::string title) {
    string line{};
    string title_val{}, author{}, review{}, publisher{}, price{}, copies{}, new_review{};


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
            std::cout << "Current review: " << result.at(2) << std::endl;
            std::cout << "New review: ";
            std::cin.ignore(1, '\n');
            std::getline(std::cin, new_review);
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
        else {
            for (size_t i{ 0 }; i < 6; i++) {
                if (i == 2) {
                    out_file << new_review << ",";
                }
                else {
                    out_file << result.at(i) << ",";
                }
            }
            
        }
    }
    std::cout << "Review has been updated Successfully!" << std::endl; 

    in_file.close();
    out_file.close();
    std::remove("record.txt");
    std::rename("temp.txt", "record.txt");
}

void Books::read_review(std::string title) {
    string line{};
    string title_val{}, author{}, review{}, publisher{}, price{}, copies{};
    char choice{};

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
            std::cout << "Review: " << result.at(2) << std::endl<<std::endl;
           
            break;
        }
    }
    
    in_file.close();
    std::cout << "Would you like to edit the review?\n1 - Yes\n2 - No\nOption: ";
    std::cin >> choice;
    switch (choice) {
        case '1':
        {
            edit_review(title);
        }
        break;

        case'2':
        {

        }
        break;
        default:
        {
            std::cout << "Invalid Option!";
            read_review(title);
        }
    }
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
       retry: std::cout << "1 - Buy Book\n2 - Edit Review\n3 - Read review\nOption: " << std::endl;
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
                goto retry;
            }
        }
    }
    else {
        vector <string> clue;
        bool substr{ false };
        clue = is_substr(search_title);

        if (clue.size() > 0) {
            substr = true;
        }
        else {
            substr = false;
        }
        size_t choice_clue{};
        char answer{};
        //This is where the program pops out books with similar spelling and asks the user if he meant any of them
        if (substr == true) {
            std::cout << "Similar Matches: \n";
            for (size_t i{ 0 }; i < clue.size(); i++) {
                std::cout << i + 1 << " - " << clue.at(i) << std::endl;
           }
            std::cout << "Option: ";
            std::cin >> choice_clue;
            size_t pos = choice_clue - 1;
            search_title = clue.at(pos);
            system("CLS");
            std::cout << "What would you like to do?\n";
            retry_1:  std::cout << "1 - Buy Book\n2 - Edit Review\n3 - Read review" << std::endl;
            std::cout << "Option: ";
            std::cin >> answer;

            switch (answer) {
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
                goto retry_1;
            }
            }
        }

        else {
            std::cout << search_title << " is not available" << std::endl;
            retry_2: std::cout << "1 - Home menu\n2 - Search another book\n3 - Quit application\n";
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
                goto retry_2;
            }
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
* 5. Expound on the buying  method keeping in mind that a customer can want to buy more than one book at once and this
*     is often the case...
* 6. Create a feature that alerts the librarian on the Books that are currently getting out of stock or that are out of stock completely...
* 7. Have an option at all aspects of the program that enable the user to go back
* */
