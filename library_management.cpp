#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    int totalCopies;
    int availableCopies;

    // Function to take book details from user
    void input() {
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Total Copies: ";
        cin >> totalCopies;
        availableCopies = totalCopies;
    }

    // Function to display a book's details
    void display() const {
        cout << left << setw(10) << id
             << setw(30) << title
             << setw(20) << author
             << setw(10) << totalCopies
             << setw(10) << availableCopies << "\n";
    }
};

class Library {
public:
    // Add a book to the file
    void addBook() {
        Book b;
        b.input();
        ofstream fout("books.txt", ios::app);
        fout << b.id << "|" << b.title << "|" << b.author << "|" 
             << b.totalCopies << "|" << b.availableCopies << "\n";
        fout.close();
        cout << "\nBook added successfully.\n";
    }

    // Display all books
    void displayBooks() {
        ifstream fin("books.txt");
        if (!fin) {
            cout << "\nNo books found.\n";
            return;
        }
        cout << "\n" << left << setw(10) << "ID"
             << setw(30) << "Title"
             << setw(20) << "Author"
             << setw(10) << "Total"
             << setw(10) << "Avail" << "\n";
        cout << string(80, '=') << "\n";

        string line;
        while (getline(fin, line)) {
            Book b;
            parseBook(line, b);
            b.display();
        }
        fin.close();
    }

    // Search for a book by ID
    void searchBook() {
        int searchId;
        cout << "Enter Book ID to search: ";
        cin >> searchId;

        ifstream fin("books.txt");
        string line;
        bool found = false;
        while (getline(fin, line)) {
            Book b;
            parseBook(line, b);
            if (b.id == searchId) {
                cout << "\nBook Found:\n";
                cout << string(50, '-') << "\n";
                b.display();
                found = true;
                break;
            }
        }
        fin.close();
        if (!found) cout << "\nBook not found.\n";
    }

    // Issue a book
    void issueBook() {
        int issueId;
        cout << "Enter Book ID to issue: ";
        cin >> issueId;

        fstream file("books.txt", ios::in | ios::out);
        string line;
        streampos pos;
        bool issued = false;

        while (file.good()) {
            pos = file.tellg();
            if (!getline(file, line)) break;
            Book b;
            parseBook(line, b);

            if (b.id == issueId) {
                if (b.availableCopies > 0) {
                    b.availableCopies--;
                    file.seekp(pos);
                    file << b.id << "|" << b.title << "|" << b.author << "|" 
                         << b.totalCopies << "|" << b.availableCopies 
                         << string(100, ' ') << "\n";
                    cout << "\nBook issued successfully.\n";
                    issued = true;
                } else {
                    cout << "\nNo copies available.\n";
                }
                break;
            }
        }
        file.close();
        if (!issued) cout << "\nBook not found.\n";
    }

    // Return a book
    void returnBook() {
        int returnId;
        cout << "Enter Book ID to return: ";
        cin >> returnId;

        fstream file("books.txt", ios::in | ios::out);
        string line;
        streampos pos;
        bool returned = false;

        while (file.good()) {
            pos = file.tellg();
            if (!getline(file, line)) break;
            Book b;
            parseBook(line, b);

            if (b.id == returnId) {
                if (b.availableCopies < b.totalCopies) {
                    b.availableCopies++;
                    file.seekp(pos);
                    file << b.id << "|" << b.title << "|" << b.author << "|" 
                         << b.totalCopies << "|" << b.availableCopies 
                         << string(100, ' ') << "\n";
                    cout << "\nBook returned successfully.\n";
                    returned = true;
                } else {
                    cout << "\nAll copies are already in the library.\n";
                }
                break;
            }
        }
        file.close();
        if (!returned) cout << "\nBook not found.\n";
    }

private:
    // Parse a book record from file line
    void parseBook(const string &line, Book &b) {
        size_t pos1 = 0, pos2;
        pos2 = line.find('|'); b.id = stoi(line.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1; pos2 = line.find('|', pos1); b.title = line.substr(pos1, pos2 - pos1);
        pos1 = pos2 + 1; pos2 = line.find('|', pos1); b.author = line.substr(pos1, pos2 - pos1);
        pos1 = pos2 + 1; pos2 = line.find('|', pos1); b.totalCopies = stoi(line.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1; b.availableCopies = stoi(line.substr(pos1));
    }
};

int main() {
    Library lib;
    int choice;

    do {
        cout << "\n==============================\n";
        cout << "   LIBRARY MANAGEMENT SYSTEM  \n";
        cout << "==============================\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Search Book\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.displayBooks(); break;
            case 3: lib.searchBook(); break;
            case 4: lib.issueBook(); break;
            case 5: lib.returnBook(); break;
            case 6: cout << "\nExiting program...\n"; break;
            default: cout << "\nInvalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
