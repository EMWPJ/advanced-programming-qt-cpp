#include <iostream>
#include <vector>
#include <string>
// 面向对象编程示例：模拟现实世界中的“图书馆-书籍-读者”关系
class Book {
public:
    Book(const std::string& title, const std::string& author)
        : title(title), author(author), borrowed(false) {}
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    bool isBorrowed() const { return borrowed; }
    void borrow() { borrowed = true; }
    void giveBack() { borrowed = false; }
private:
    std::string title;
    std::string author;
    bool borrowed;
};

class Reader {
public:
    Reader(const std::string& name) : name(name) {}
    void borrowBook(Book& book) {
        if (!book.isBorrowed()) {
            book.borrow();
            borrowedBooks.push_back(&book);
            std::cout << name << " 借阅了《" << book.getTitle() << "》" << std::endl;
        } else {
            std::cout << "《" << book.getTitle() << "》已被借出，无法借阅。" << std::endl;
        }
    }
    void returnBook(Book& book) {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
            if (*it == &book) {
                book.giveBack();
                borrowedBooks.erase(it);
                std::cout << name << " 归还了《" << book.getTitle() << "》" << std::endl;
                return;
            }
        }
        std::cout << name << " 没有借阅《" << book.getTitle() << "》，无法归还。" << std::endl;
    }
    void listBorrowedBooks() const {
        std::cout << name << " 当前借阅的书籍：" << std::endl;
        if (borrowedBooks.empty()) {
            std::cout << "  无" << std::endl;
        } else {
            for (const auto* book : borrowedBooks) {
                std::cout << "  《" << book->getTitle() << "》 by " << book->getAuthor() << std::endl;
            }
        }
    }
private:
    std::string name;
    std::vector<Book*> borrowedBooks;
};

class Library {
public:
    void addBook(const Book& book) {
        books.push_back(book);
    }
    Book* findBook(const std::string& title) {
        for (auto& book : books) {
            if (book.getTitle() == title) {
                return &book;
            }
        }
        return nullptr;
    }
    void listBooks() const {
        std::cout << "图书馆藏书列表：" << std::endl;
        for (const auto& book : books) {
            std::cout << "  《" << book.getTitle() << "》 by " << book.getAuthor();
            if (book.isBorrowed()) {
                std::cout << "（已借出）";
            }
            std::cout << std::endl;
        }
    }
private:
    std::vector<Book> books;
};
int main() {
    Library library;
    library.addBook(Book("C++ Primer", "Stanley B. Lippman"));
    library.addBook(Book("深入理解计算机系统", "Randal E. Bryant"));
    library.addBook(Book("算法导论", "Thomas H. Cormen"));
    Reader alice("Alice");
    Reader bob("Bob");
    library.listBooks();
    std::cout << std::endl;
    Book* book1 = library.findBook("C++ Primer");
    Book* book2 = library.findBook("算法导论");
    if (book1) alice.borrowBook(*book1);
    if (book2) bob.borrowBook(*book2);
    if (book1) bob.borrowBook(*book1); // 尝试借已借出的书
    std::cout << std::endl;
    alice.listBorrowedBooks();
    bob.listBorrowedBooks();
    std::cout << std::endl;
    library.listBooks();
    std::cout << std::endl;
    if (book1) alice.returnBook(*book1);
    if (book1) bob.borrowBook(*book1); // 现在Bob可以借了
    std::cout << std::endl;
    library.listBooks();
    std::cin.get();
    return 0;
}
