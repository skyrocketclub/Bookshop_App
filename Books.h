#ifndef _BOOKS_H_
#define _BOOKS_H_
#include <vector>
#include <string>


class Books
{
private:
    //The attributes are being defined and initialised Here
    std::vector<std::string> title{ " " };
    std::vector<std::string> author{ " " };
    std::vector<std::string> reviews{ " " };
    std::vector<std::string> publisher{ " " };
    std::vector<double>price{ 0.00 };
    std::vector<int> copies{ 0 };
    std::vector<int> position{ 0 };
    
    int total_num{ 0 };
    int quited{ 0 };
    char option{};

public:
    //The methods are initialised here as well... Sort of their prototypes
    bool check_book(std::string title);
    void update_copies(std::string title);

    void buy_book(std::string title);
    void edit_review(std::string title);
    void read_review(std::string title);

    void display_menu();
    void list_books();
    void add_books();
    void search_books();
    void quit();
    std::string capitalise(std::string input);

};
#endif // _BOOKS_H_
