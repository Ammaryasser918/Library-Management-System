#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    string isbn;
    int year;
    string category;

public:
    // Constructor
    Book(int id, string title, string author, string isbn, int year, string category)
        : id(id), title(title), author(author), isbn(isbn), year(year), category(category) {
    }

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return isbn; }
    int getYear() const { return year; }
    string getCategory() const { return category; }

    void setTitle(string t) { title = t; }
    void setAuthor(string a) { author = a; }
    void setISBN(string i) { isbn = i; }
    void setYear(int y) { year = y; }
    void setCategory(string c) { category = c; }

    void display() const {
        cout << "\n" << string(60, '=') << endl;
        cout << "Book ID      : " << id << endl;
        cout << "Title        : " << title << endl;
        cout << "Author       : " << author << endl;
        cout << "ISBN         : " << isbn << endl;
        cout << "Year         : " << year << endl;
        cout << "Category     : " << category << endl;
        cout << string(60, '=') << endl;
    }

    void saveToFile(ofstream& file) const {
        file << id << "|" << title << "|" << author << "|"
            << isbn << "|" << year << "|" << category << endl;
    }
};

class Library {
private:
    vector<Book> books;
    string filename;
    int nextId;

    void loadFromFile() {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "No previous data found. Starting fresh.\n";
            nextId = 1;
            return;
        }

        string line;
        int maxId = 0;
        while (getline(file, line)) {
            size_t pos = 0;
            vector<string> tokens;

            while ((pos = line.find('|')) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            tokens.push_back(line);

            if (tokens.size() == 6) {
                int id = stoi(tokens[0]);
                int year = stoi(tokens[4]);
                books.emplace_back(id, tokens[1], tokens[2], tokens[3], year, tokens[5]);
                if (id > maxId) maxId = id;
            }
        }
        file.close();
        nextId = maxId + 1;
        cout << "✓ Loaded " << books.size() << " books from database.\n";
    }

    void saveToFile() {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not save data!\n";
            return;
        }

        for (const auto& book : books) {
            book.saveToFile(file);
        }
        file.close();
        cout << "✓ Data saved successfully!\n";
    }

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void pause() {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

public:
    // Constructor
    Library(string file = "library_data.txt") : filename(file), nextId(1) {
        loadFromFile();
    }

    ~Library() {
        saveToFile();
    }

    void addBook() {
        clearScreen();
        cout << "\n=== Add New Book ===\n\n";

        string title, author, isbn, category;
        int year;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        cout << "Enter ISBN: ";
        getline(cin, isbn);

        cout << "Enter Publication Year: ";
        cin >> year;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Category: ";
        getline(cin, category);

        books.emplace_back(nextId++, title, author, isbn, year, category);

        cout << "\n✓ Book added successfully with ID: " << (nextId - 1) << endl;
        saveToFile();
        pause();
    }

    void viewAllBooks() {
        clearScreen();
        cout << "\n=== All Books in Library ===\n";

        if (books.empty()) {
            cout << "\nNo books in the library yet!\n";
            pause();
            return;
        }

        for (const auto& book : books) {
            book.display();
        }

        cout << "\nTotal Books: " << books.size() << endl;
        pause();
    }

    void searchBook() {
        clearScreen();
        cout << "\n=== Search Books ===\n";
        cout << "1. Search by ID\n";
        cout << "2. Search by Title\n";
        cout << "3. Search by Author\n";
        cout << "4. Search by Category\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<Book> results;

        switch (choice) {
        case 1: {
            int id;
            cout << "Enter Book ID: ";
            cin >> id;
            for (const auto& book : books) {
                if (book.getId() == id) {
                    results.push_back(book);
                    break;
                }
            }
            break;
        }
        case 2: {
            string title;
            cout << "Enter Title: ";
            getline(cin, title);
            for (const auto& book : books) {
                if (book.getTitle().find(title) != string::npos) {
                    results.push_back(book);
                }
            }
            break;
        }
        case 3: {
            string author;
            cout << "Enter Author: ";
            getline(cin, author);
            for (const auto& book : books) {
                if (book.getAuthor().find(author) != string::npos) {
                    results.push_back(book);
                }
            }
            break;
        }
        case 4: {
            string category;
            cout << "Enter Category: ";
            getline(cin, category);
            for (const auto& book : books) {
                if (book.getCategory().find(category) != string::npos) {
                    results.push_back(book);
                }
            }
            break;
        }
        default:
            cout << "Invalid choice!\n";
            pause();
            return;
        }

        if (results.empty()) {
            cout << "\nNo books found!\n";
        }
        else {
            cout << "\nFound " << results.size() << " book(s):\n";
            for (const auto& book : results) {
                book.display();
            }
        }
        pause();
    }

    void editBook() {
        clearScreen();
        cout << "\n=== Edit Book ===\n";
        cout << "Enter Book ID to edit: ";

        int id;
        cin >> id;

        auto it = find_if(books.begin(), books.end(),
            [id](const Book& b) { return b.getId() == id; });

        if (it == books.end()) {
            cout << "\nBook not found!\n";
            pause();
            return;
        }

        it->display();

        cout << "\nWhat do you want to edit?\n";
        cout << "1. Title\n";
        cout << "2. Author\n";
        cout << "3. ISBN\n";
        cout << "4. Year\n";
        cout << "5. Category\n";
        cout << "6. Edit All\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string input;
        int year;

        switch (choice) {
        case 1:
            cout << "Enter new Title: ";
            getline(cin, input);
            it->setTitle(input);
            break;
        case 2:
            cout << "Enter new Author: ";
            getline(cin, input);
            it->setAuthor(input);
            break;
        case 3:
            cout << "Enter new ISBN: ";
            getline(cin, input);
            it->setISBN(input);
            break;
        case 4:
            cout << "Enter new Year: ";
            cin >> year;
            it->setYear(year);
            break;
        case 5:
            cout << "Enter new Category: ";
            getline(cin, input);
            it->setCategory(input);
            break;
        case 6:
            cout << "Enter new Title: ";
            getline(cin, input);
            it->setTitle(input);

            cout << "Enter new Author: ";
            getline(cin, input);
            it->setAuthor(input);

            cout << "Enter new ISBN: ";
            getline(cin, input);
            it->setISBN(input);

            cout << "Enter new Year: ";
            cin >> year;
            it->setYear(year);

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter new Category: ";
            getline(cin, input);
            it->setCategory(input);
            break;
        default:
            cout << "Invalid choice!\n";
            pause();
            return;
        }

        cout << "\n✓ Book updated successfully!\n";
        saveToFile();
        pause();
    }

    void deleteBook() {
        clearScreen();
        cout << "\n=== Delete Book ===\n";
        cout << "Enter Book ID to delete: ";

        int id;
        cin >> id;

        auto it = find_if(books.begin(), books.end(),
            [id](const Book& b) { return b.getId() == id; });

        if (it == books.end()) {
            cout << "\nBook not found!\n";
            pause();
            return;
        }

        it->display();

        cout << "\nAre you sure you want to delete this book? (y/n): ";
        char confirm;
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            books.erase(it);
            cout << "\n✓ Book deleted successfully!\n";
            saveToFile();
        }
        else {
            cout << "\nDeletion cancelled.\n";
        }
        pause();
    }

    void displayMenu() {
        clearScreen();
        cout << "\n";
        cout << "===========================================\n";
        cout << "    LIBRARY MANAGEMENT SYSTEM              \n";
        cout << "===========================================\n";
        cout << "\n";
        cout << "1. Add New Book\n";
        cout << "2. View All Books\n";
        cout << "3. Search Books\n";
        cout << "4. Edit Book\n";
        cout << "5. Delete Book\n";
        cout << "6. Exit\n";
        cout << "\nEnter your choice: ";
    }

    void run() {
        int choice;
        do {
            displayMenu();
            cin >> choice;

            switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                viewAllBooks();
                break;
            case 3:
                searchBook();
                break;
            case 4:
                editBook();
                break;
            case 5:
                deleteBook();
                break;
            case 6:
                clearScreen();
                cout << "\nThank you for using Library Management System!\n";
                cout << "Goodbye!\n\n";
                break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
                pause();
            }
        } while (choice != 6);
    }
};

// Main Function
int main() {
    Library library;
    library.run();
    return 0;
}