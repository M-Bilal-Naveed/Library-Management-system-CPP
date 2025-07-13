#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <algorithm>

struct Book {
    std::string title;
    std::string author;
    std::string ISBN;
    bool isAvailable;
};

struct Patron {
    std::string name;
    int ID;
    int booksBorrowed;
};

class Library {
private:
    std::vector<Book> books;
    std::vector<Patron> patrons;
    std::stack<Book> borrowedBooks;
    std::queue<Book> bookRequests;

public:
    // Function declarations
    void addBook(const std::string& title, const std::string& author, const std::string& ISBN);
    void removeBook(const std::string& ISBN);
    void displayBooks();
    void issueBook(int patronID, const std::string& ISBN);
    void returnBook(int patronID, const std::string& ISBN);
    void requestBook(int patronID, const std::string& ISBN);
    void processBookRequests();

    // Example constructor
    Library() {
        // Initialize your library with some default books and patrons
        addBook("The Catcher in the Rye", "J.D. Salinger", "978-0-316-76948-0");
        addBook("To Kill a Mockingbird", "Harper Lee", "978-0-06-112008-4");
        addBook("1984", "George Orwell", "978-0-452-28423-4");

        addPatron("John Doe", 1);
        addPatron("Jane Smith", 2);
    }

private:
    void addPatron(const std::string& name, int ID) {
        patrons.push_back({name, ID, 0});
    }
};

// Function definitions
void Library::addBook(const std::string& title, const std::string& author, const std::string& ISBN) {
    books.push_back({title, author, ISBN, true});
}

void Library::removeBook(const std::string& ISBN) {
    auto it = std::find_if(books.begin(), books.end(), [&ISBN](const Book& book) {
        return book.ISBN == ISBN;
    });

    if (it != books.end()) {
        books.erase(it);
        std::cout << "Book with ISBN " << ISBN << " removed from the library.\n";
    } else {
        std::cout << "Book with ISBN " << ISBN << " not found in the library.\n";
    }
}

void Library::displayBooks() {
    std::cout << "\n*** Library Catalog ***\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "| ISBN            | Title                   |\n";
    std::cout << "-----------------------------------------\n";
    for (const auto& book : books) {
        std::cout << "| " << book.ISBN << " | " << book.title;
        // Add spaces to align book titles
        for (size_t i = 0; i < 24 - book.title.length(); ++i) {
            std::cout << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "-----------------------------------------\n";
}

void Library::issueBook(int patronID, const std::string& ISBN) {
    auto it = std::find_if(books.begin(), books.end(), [&ISBN](const Book& book) {
        return book.ISBN == ISBN && book.isAvailable;
    });

    if (it != books.end()) {
        borrowedBooks.push(*it);
        it->isAvailable = false;
        for (auto& patron : patrons) {
            if (patron.ID == patronID) {
                patron.booksBorrowed++;
                break;
            }
        }
        std::cout << "Book with ISBN " << ISBN << " issued to patron ID " << patronID << ".\n";
    } else {
        std::cout << "Book with ISBN " << ISBN << " not available for issue.\n";
    }
}

void Library::returnBook(int patronID, const std::string& ISBN) {
    if (!borrowedBooks.empty() && borrowedBooks.top().ISBN == ISBN) {
        borrowedBooks.top().isAvailable = true;
        borrowedBooks.pop();
        for (auto& patron : patrons) {
            if (patron.ID == patronID && patron.booksBorrowed > 0) {
                patron.booksBorrowed--;
                std::cout << "Book with ISBN " << ISBN << " returned by patron ID " << patronID << ".\n";
                return;
            }
        }
        std::cout << "Error: Patron ID " << patronID << " has not borrowed this book.\n";
    } else {
        std::cout << "Error: Book with ISBN " << ISBN << " is not at the top of the borrowed books stack.\n";
    }
}

void Library::requestBook(int patronID, const std::string& ISBN) {
    auto it = std::find_if(books.begin(), books.end(), [&ISBN](const Book& book) {
        return book.ISBN == ISBN && !book.isAvailable;
    });

    if (it != books.end()) {
        bookRequests.push(*it);
        std::cout << "Book with ISBN " << ISBN << " requested by patron ID " << patronID << ".\n";
    } else {
        std::cout << "Book with ISBN " << ISBN << " is either available or not found in the library.\n";
    }
}

void Library::processBookRequests() {
    while (!bookRequests.empty() && borrowedBooks.size() < books.size()) {
        Book requestedBook = bookRequests.front();
        bookRequests.pop();
        requestedBook.isAvailable = true;
        borrowedBooks.push(requestedBook);
        std::cout << "Book with ISBN " << requestedBook.ISBN << " is now available. Fulfilling request.\n";
    }
}

int main() {
    Library library;

    int choice;
    do {
        std::cout << "\nLibrary Management System\n";
        std::cout << "1. Display Books\n";
        std::cout << "2. Add Book\n";
        std::cout << "3. Remove Book\n";
        std::cout << "4. Issue Book\n";
        std::cout << "5. Return Book\n";
        std::cout << "6. Request Book\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                library.displayBooks();
                break;
            case 2: {
                std::string title, author, ISBN;
                std::cout << "Enter book details:\n";
                std::cout << "Title: ";
                std::cin.ignore();
                std::getline(std::cin, title);
                std::cout << "Author: ";
                std::getline(std::cin, author);
                std::cout << "ISBN: ";
                std::getline(std::cin, ISBN);
                library.addBook(title, author, ISBN);
                break;
            }
            case 3: {
                std::string ISBN;
                std::cout << "Enter ISBN of the book to remove: ";
                std::cin >> ISBN;
                library.removeBook(ISBN);
                break;
            }
            case 4: {
                int patronID;
                std::string ISBN;
                std::cout << "Enter patron ID: ";
                std::cin >> patronID;
                std::cout << "Enter ISBN of the book to issue: ";
                std::cin >> ISBN;
                library.issueBook(patronID, ISBN);
                break;
            }
            case 5: {
                int patronID;
                std::string ISBN;
                std::cout << "Enter patron ID: ";
                std::cin >> patronID;
                std::cout << "Enter ISBN of the book to return: ";
                std::cin >> ISBN;
                library.returnBook(patronID, ISBN);
                break;
            }
            case 6: {
                int patronID;
                std::string ISBN;
                std::cout << "Enter patron ID: ";
                std::cin >> patronID;
                std::cout << "Enter ISBN of the requested book: ";
                std::cin >> ISBN;
                library.requestBook(patronID, ISBN);
                break;
            }
            case 7:
                library.processBookRequests();
                break;
            case 0:
                std::cout << "Program tw War Gaya.\n";
                break;
            default:
                std::cout << "Invalid choice. Please enter a valid option.\n";
        }

    } while (choice != 0);

    return 0;
}

