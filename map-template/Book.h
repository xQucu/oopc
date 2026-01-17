#include <iostream>
#include <string>

class Book {
public:
    Book(std::string author = "", std::string category = "", int pages = 0, std::string status = "on the bookshelf")
        : author(author)
        , category(category)
        , pages(pages)
        , status(status)
    {
    }

    std::string author;
    std::string category;
    int pages;
    std::string status;
};

inline std::ostream& operator<<(std::ostream& os, const Book& book)
{
    os << "Author: " << book.author
       << ", Category: " << book.category
       << ", Pages: " << book.pages
       << ", Status: " << book.status;
    return os;
}
