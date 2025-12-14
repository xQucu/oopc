#include "Book.h"
#include "Employee.h" // Defines class Employee
#include "map.h" // Defines template Map<Key, Value>
#include <iostream>
#include <string>

typedef unsigned int ID; // Identification number of employee
typedef Map<ID, Employee> Database; // Database of employees

using namespace std;
typedef Map<std::string, Book> Library;

void addEmployees(Database& database);
void modifyEmployees(Database& database);
void demonstrateLibrary();
void demonstrateEmployees();

int main()
{
    demonstrateEmployees();

    cout << "-----------------------" << endl;

    demonstrateLibrary();

    return 0;
}

void demonstrateEmployees()
{
    Database database;
    addEmployees(database);

    Database newDatabase = database; // Make a copy of database
    newDatabase.add(830505432, Employee("Ewa Nowak", "charwoman", 43)); // Add fourth employee
    modifyEmployees(newDatabase);

    cout << "Original database:" << endl
         << database << endl;
    cout << "Modified database:" << endl
         << newDatabase << endl;

    database = newDatabase; // Update original database

    cout << "Database after the assignment:" << endl
         << database << endl;

    cout << "Removing employee with ID 730505129 (Anna Zaradna)..." << endl;
    database.remove(730505129);

    cout << "Database after removal:" << endl
         << database << endl;
};

void addEmployees(Database& database)
{
    database.add(761028073, Employee("Jan Kowalski", "salesman", 28)); // Add first employee: name: Jan Kowalski, position: salseman, age: 28,
    database.add(510212881, Employee("Adam Nowak", "storekeeper", 54)); // Add second employee
    database.add(730505129, Employee("Anna Zaradna", "secretary", 32)); // Add third employee
}

void modifyEmployees(Database& database)
{
    Employee* employeePtr;

    employeePtr = database.find(510212881); // Find employee using its ID
    employeePtr->position = "salesman"; // Modify the position of employee

    employeePtr = database.find(761028073); // Find employee using its ID
    employeePtr->age = 29; // Modify the age of employee
}

void demonstrateLibrary()
{
    Library library;

    library.add("The Great Gatsby", Book("F. Scott Fitzgerald", "Fiction", 180, "on the bookshelf"));
    library.add("1984", Book("George Orwell", "Dystopian Fiction", 328, "on the bookshelf"));
    library.add("To Kill a Mockingbird", Book("Harper Lee", "Fiction", 281, "borrowed"));
    library.add("Pride and Prejudice", Book("Jane Austen", "Romance", 432, "on the bookshelf"));
    library.add("The Catcher in the Rye", Book("J.D. Salinger", "Fiction", 277, "on the bookshelf"));
    library.add("Animal Farm", Book("George Orwell", "Political Satire", 112, "borrowed"));
    library.add("Brave New World", Book("Aldous Huxley", "Science Fiction", 268, "on the bookshelf"));

    cout << endl
         << "Library catalog:" << endl;
    cout << library << endl;

    Library newLibrary = library;

    cout << "Finding '1984' and changing its status to borrowed..." << endl;
    Book* bookPtr = library.find("1984");
    if (bookPtr != nullptr) {
        cout << "Before: " << *bookPtr << endl;
        bookPtr->status = "borrowed";
        cout << "After: " << *bookPtr << endl;
    } else {
        cout << "Book not found!" << endl;
    }

    cout << endl
         << "Current library catalog:" << endl;
    cout << library << endl;

    cout << "Backup library catalog (before modifications):" << endl;
    cout << newLibrary << endl;

    cout << "Searching for 'Harry Potter'..." << endl;
    bookPtr = library.find("Harry Potter");
    if (bookPtr != nullptr) {
        cout << "Found: " << *bookPtr << endl;
    } else {
        cout << "Book 'Harry Potter' not found in the library!" << endl;
    }
}
