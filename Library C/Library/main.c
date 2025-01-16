/*
    Student No: 123456789
    Name: Your Name

    Description:
    This program provides a basic simulation of a library management system.
    It includes operations such as:
    - Adding, deleting, searching, and listing books;
    - Adding, searching, and listing members;
    - Borrowing and returning books, and viewing the borrowing list;
    - Listing available and borrowed books.

    All data is stored in corresponding .txt files.
    Prepared in accordance with the project guidelines.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BOOKS 100
#define MAX_MEMBERS 100
#define MAX_BORROWS 100

#define BOOKS_FILE   "books.txt"
#define MEMBERS_FILE "members.txt"
#define BORROWS_FILE "borrows.txt"

/*
 * STRUCT DEFINITIONS
 * ------------------
 * Book:   Holds information about a single book (ID, title, author, status).
 * Member: Holds information about a single member (ID, name, phone).
 * Borrow: Holds information about borrowing a book (which book, which member, dates).
 */

typedef struct {
    int  ID;             // Book ID
    char title[100];     // Book title
    char author[100];    // Book author
    int  status;         // 1: available, 0: borrowed
} Book;

typedef struct {
    char ID[12];         // Member ID number (11 digits + 1 null terminator)
    char name[50];       // Member name
    char phone[20];      // Phone number
} Member;

typedef struct {
    int  bookID;         // ID of the borrowed book
    char memberID[12];   // ID number of the member who borrowed the book (11 digits)
    char borrowDate[11]; // Borrow date (format dd/mm/yyyy: 10 chars + '\0')
    char returnDate[11]; // Return date (format dd/mm/yyyy: 10 chars + '\0')
} Borrow;

/* -- FUNCTION PROTOTYPES -- */

// Book operations
int  loadBooks(Book books[]);
void saveBooks(const Book books[], int count);
void addBook();
void deleteBook();
void searchBook();
void listBooks();
void listAvailableBooks();
void listBorrowedBooks();

// Member operations
int  loadMembers(Member members[]);
void saveMembers(const Member members[], int count);
void addMember();
void searchMember();
void listMembers();

// Borrow operations
int  loadBorrows(Borrow borrows[]);
void saveBorrows(const Borrow borrows[], int count);
void borrowBook();
void returnBook();
void listBorrows();

// Helper functions
int  isValidMemberID(const char *id);
void clearInputBuffer();

int main()
{
    int choice;

    // Main loop (runs until the user chooses to exit)
    while (1)
    {
        printf("\n---------- KÜTÜPHANE YÖNETİM SİSTEMİ ----------\n");
        printf("1. Kitap Yönetimi\n");
        printf("2. Üye Yönetimi\n");
        printf("3. Ödünç İşlemleri\n");
        printf("4. Raporlama\n");
        printf("5. Çıkış\n");
        printf("----------------------------------------------\n");
        printf("Seçiminiz: ");
        scanf("%d", &choice);
        clearInputBuffer(); // Clear input buffer after scanf

        switch (choice)
        {
            case 1:
            {
                int bookChoice;
                printf("\n-- Kitap Yönetimi --\n");
                printf("1. Kitap Ekle\n");
                printf("2. Kitap Sil\n");
                printf("3. Kitap Ara\n");
                printf("4. Kitap Listele\n");
                printf("Seçiminiz: ");
                scanf("%d", &bookChoice);
                clearInputBuffer();

                switch(bookChoice)
                {
                    case 1: addBook();        break;
                    case 2: deleteBook();     break;
                    case 3: searchBook();     break;
                    case 4: listBooks();      break;
                    default: printf("Geçersiz seçim!\n"); break;
                }
            }
            break;

            case 2:
            {
                int memberChoice;
                printf("\n-- Üye Yönetimi --\n");
                printf("1. Üye Ekle\n");
                printf("2. Üye Ara\n");
                printf("3. Üye Listele\n");
                printf("Seçiminiz: ");
                scanf("%d", &memberChoice);
                clearInputBuffer();

                switch(memberChoice)
                {
                    case 1: addMember();      break;
                    case 2: searchMember();   break;
                    case 3: listMembers();    break;
                    default: printf("Geçersiz seçim!\n"); break;
                }
            }
            break;

            case 3:
            {
                int borrowChoice;
                printf("\n-- Ödünç İşlemleri --\n");
                printf("1. Kitap Ödünç Ver\n");
                printf("2. Kitap İade Al\n");
                printf("3. Ödünç Listesi\n");
                printf("Seçiminiz: ");
                scanf("%d", &borrowChoice);
                clearInputBuffer();

                switch(borrowChoice)
                {
                    case 1: borrowBook();  break;
                    case 2: returnBook();  break;
                    case 3: listBorrows(); break;
                    default: printf("Geçersiz seçim!\n"); break;
                }
            }
            break;

            case 4:
            {
                int reportChoice;
                printf("\n-- Raporlama --\n");
                printf("1. Mevcut (Boşta) Kitapları Listele\n");
                printf("2. Ödünçteki Kitapları Listele\n");
                printf("Seçiminiz: ");
                scanf("%d", &reportChoice);
                clearInputBuffer();

                switch(reportChoice)
                {
                    case 1: listAvailableBooks(); break;
                    case 2: listBorrowedBooks();  break;
                    default: printf("Geçersiz seçim!\n"); break;
                }
            }
            break;

            case 5:
                printf("Programdan çıkılıyor...\n");
                return 0;

            default:
                printf("Geçersiz seçim!\n");
                break;
        }
    }

    return 0;
}

/*
    -------------------------
    BOOK OPERATIONS
    -------------------------
*/

/**
 * Reads books from the file into the books array.
 * Returns the number of books read.
 */
int loadBooks(Book books[])
{
    FILE *fp = fopen(BOOKS_FILE, "r");
    if (fp == NULL)
        return 0; // If file doesn't exist, return 0

    int count = 0;
    while(!feof(fp) && count < MAX_BOOKS)
    {
        /*
            File line format:
            ID|title|author|status
        */
        Book temp;
        if(fscanf(fp, "%d|%[^|]|%[^|]|%d\n", &temp.ID, temp.title, temp.author, &temp.status) == 4)
        {
            books[count++] = temp;
        }
    }

    fclose(fp);
    return count;
}

/**
 * Writes the book data from the array to the file.
 */
void saveBooks(const Book books[], int count)
{
    FILE *fp = fopen(BOOKS_FILE, "w");
    if (fp == NULL)
    {
        printf("Failed to open file for saving books!\n");
        return;
    }

    for(int i = 0; i < count; i++)
    {
        fprintf(fp, "%d|%s|%s|%d\n",
                books[i].ID,
                books[i].title,
                books[i].author,
                books[i].status);
    }

    fclose(fp);
}

/**
 * Adds a new book to the system.
 */
void addBook()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books);

    if(count >= MAX_BOOKS)
    {
        printf("Maximum number of books reached!\n");
        return;
    }

    Book newBook;
    printf("New Book ID: ");
    scanf("%d", &newBook.ID);
    clearInputBuffer();

    // Check for duplicate ID
    for(int i = 0; i < count; i++)
    {
        if(books[i].ID == newBook.ID)
        {
            printf("A book with this ID already exists!\n");
            return;
        }
    }

    printf("Book Title: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    // Remove the trailing \n character
    newBook.title[strcspn(newBook.title, "\n")] = '\0';

    printf("Author Name: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0';

    newBook.status = 1; // By default, a newly added book is available

    // Add to the end of the array
    books[count++] = newBook;

    // Write to file
    saveBooks(books, count);
    printf("Book added successfully!\n");
}

/**
 * Deletes a book from the system based on its ID.
 */
void deleteBook()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books);

    int id;
    printf("Enter the ID of the book to delete: ");
    scanf("%d", &id);

    int found = 0;
    for(int i = 0; i < count; i++)
    {
        if(books[i].ID == id)
        {
            found = 1;
            // To delete the i-th element, move the last element to this position
            books[i] = books[count - 1];
            count--;
            break;
        }
    }

    if(found)
    {
        saveBooks(books, count);
        printf("Book deleted.\n");
    }
    else
    {
        printf("No book found with this ID!\n");
    }
}

/**
 * Searches for a book by its ID and prints the details if found.
 */
void searchBook()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books);

    int id;
    printf("Enter the ID of the book to search: ");
    scanf("%d", &id);

    for(int i = 0; i < count; i++)
    {
        if(books[i].ID == id)
        {
            printf("\nBook Found:\n");
            printf("ID     : %d\n", books[i].ID);
            printf("Title  : %s\n", books[i].title);
            printf("Author : %s\n", books[i].author);
            printf("Status : %s\n", (books[i].status == 1) ? "Available" : "Borrowed");
            return;
        }
    }

    printf("No book found with this ID!\n");
}

/**
 * Lists all books, whether available or borrowed.
 */
void listBooks()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books);

    if(count == 0)
    {
        printf("No books registered.\n");
        return;
    }

    printf("\n--- All Books ---\n");
    for(int i = 0; i < count; i++)
    {
        printf("[%d] %s - %s (%s)\n",
               books[i].ID,
               books[i].title,
               books[i].author,
               books[i].status == 1 ? "Mevcut" : "Ödünçte");
    }
}

/**
 * Lists only the books that are currently available (status == 1).
 */
void listAvailableBooks()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books);
    int found = 0;

    printf("\n--- Mevcut Kitaplar ---\n");
    for(int i = 0; i < count; i++)
    {
        if(books[i].status == 1)
        {
            printf("[%d] %s - %s\n", books[i].ID, books[i].title, books[i].author);
            found = 1;
        }
    }

    if(!found)
    {
        printf("No books are currently available.\n");
    }
}

/**
 * Lists only the books that are currently borrowed (status == 0).
 */
void listBorrowedBooks()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books);
    int found = 0;

    printf("\n--- Ödünçteki Kitaplar ---\n");
    for(int i = 0; i < count; i++)
    {
        if(books[i].status == 0)
        {
            printf("[%d] %s - %s\n", books[i].ID, books[i].title, books[i].author);
            found = 1;
        }
    }

    if(!found)
    {
        printf("No books are currently borrowed.\n");
    }
}

/*
    -------------------------
    MEMBER OPERATIONS
    -------------------------
*/

/**
 * Reads members from the file into the members array.
 * Returns the number of members read.
 */
int loadMembers(Member members[])
{
    FILE *fp = fopen(MEMBERS_FILE, "r");
    if (fp == NULL)
        return 0; // If file doesn't exist, return 0

    int count = 0;
    while(!feof(fp) && count < MAX_MEMBERS)
    {
        /*
            File line format:
            ID|name|phone
        */
        Member temp;
        if(fscanf(fp, "%[^|]|%[^|]|%[^\n]\n", temp.ID, temp.name, temp.phone) == 3)
        {
            members[count++] = temp;
        }
    }
    fclose(fp);
    return count;
}

/**
 * Writes the member data from the array to the file.
 */
void saveMembers(const Member members[], int count)
{
    FILE *fp = fopen(MEMBERS_FILE, "w");
    if (fp == NULL)
    {
        printf("Failed to open file for saving members!\n");
        return;
    }

    for(int i = 0; i < count; i++)
    {
        fprintf(fp, "%s|%s|%s\n",
                members[i].ID,
                members[i].name,
                members[i].phone);
    }

    fclose(fp);
}

/**
 * Adds a new member to the system.
 */
void addMember()
{
    Member members[MAX_MEMBERS];
    int count = loadMembers(members);

    if(count >= MAX_MEMBERS)
    {
        printf("Maximum number of members reached!\n");
        return;
    }

    Member newMember;
    printf("Enter Member TC ID Number (11 digits): ");
    fgets(newMember.ID, sizeof(newMember.ID), stdin);
    newMember.ID[strcspn(newMember.ID, "\n")] = '\0';

    // Validate ID
    if(!isValidMemberID(newMember.ID))
    {
        printf("Invalid ID number!\n");
        return;
    }

    // Check for duplicate ID
    for(int i = 0; i < count; i++)
    {
        if(strcmp(members[i].ID, newMember.ID) == 0)
        {
            printf("A member with this ID already exists!\n");
            return;
        }
    }

    printf("Member Name: ");
    fgets(newMember.name, sizeof(newMember.name), stdin);
    newMember.name[strcspn(newMember.name, "\n")] = '\0';

    printf("Member Phone Number: ");
    fgets(newMember.phone, sizeof(newMember.phone), stdin);
    newMember.phone[strcspn(newMember.phone, "\n")] = '\0';

    // Add to the end of the array
    members[count++] = newMember;

    saveMembers(members, count);
    printf("Member added successfully!\n");
}

/**
 * Searches for a member by their 11-digit ID and prints the details if found.
 */
void searchMember()
{
    Member members[MAX_MEMBERS];
    int count = loadMembers(members);

    char id[12];
    printf("Enter the TC ID Number (11 digits) of the member to search: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    for(int i = 0; i < count; i++)
    {
        if(strcmp(members[i].ID, id) == 0)
        {
            printf("\nMember Found:\n");
            printf("TC ID No : %s\n", members[i].ID);
            printf("Name     : %s\n", members[i].name);
            printf("Phone    : %s\n", members[i].phone);
            return;
        }
    }
    printf("No member found with this ID!\n");
}

/**
 * Lists all registered members.
 */
void listMembers()
{
    Member members[MAX_MEMBERS];
    int count = loadMembers(members);

    if(count == 0)
    {
        printf("No members registered.\n");
        return;
    }

    printf("\n--- All Members ---\n");
    for(int i = 0; i < count; i++)
    {
        printf("[%s] %s - %s\n",
               members[i].ID,
               members[i].name,
               members[i].phone);
    }
}

/*
    -------------------------
    BORROW OPERATIONS
    -------------------------
*/

/**
 * Reads borrow data from the file into the borrows array.
 * Returns the number of borrow records read.
 */
int loadBorrows(Borrow borrows[])
{
    FILE *fp = fopen(BORROWS_FILE, "r");
    if (fp == NULL)
        return 0; // If file doesn't exist, return 0

    int count = 0;
    while(!feof(fp) && count < MAX_BORROWS)
    {
        /*
            File line format:
            bookID|memberID|borrowDate|returnDate
        */
        Borrow temp;
        if(fscanf(fp, "%d|%[^|]|%[^|]|%[^\n]\n",
                  &temp.bookID,
                  temp.memberID,
                  temp.borrowDate,
                  temp.returnDate) == 4)
        {
            borrows[count++] = temp;
        }
    }

    fclose(fp);
    return count;
}

/**
 * Writes the borrow data from the array to the file.
 */
void saveBorrows(const Borrow borrows[], int count)
{
    FILE *fp = fopen(BORROWS_FILE, "w");
    if (fp == NULL)
    {
        printf("Failed to write borrow records to file!\n");
        return;
    }

    for(int i = 0; i < count; i++)
    {
        fprintf(fp, "%d|%s|%s|%s\n",
                borrows[i].bookID,
                borrows[i].memberID,
                borrows[i].borrowDate,
                borrows[i].returnDate);
    }

    fclose(fp);
}

/**
 * Borrows a book for a member, provided the book is available.
 */
void borrowBook()
{
    Book   books[MAX_BOOKS];
    Member members[MAX_MEMBERS];
    Borrow borrows[MAX_BORROWS];

    int bookCount   = loadBooks(books);
    int memberCount = loadMembers(members);
    int borrowCount = loadBorrows(borrows);

    int bookID;
    char memberID[12];
    printf("Enter the ID of the book to borrow: ");
    scanf("%d", &bookID);
    clearInputBuffer();

    // Check if the book exists
    int bookIndex = -1;
    for(int i = 0; i < bookCount; i++)
    {
        if(books[i].ID == bookID)
        {
            bookIndex = i;
            break;
        }
    }
    if(bookIndex == -1)
    {
        printf("No book found with this ID!\n");
        return;
    }
    // Check if the book is already borrowed
    if(books[bookIndex].status == 0)
    {
        printf("This book is already borrowed!\n");
        return;
    }

    printf("Enter the TC ID Number (11 digits) of the member borrowing the book: ");
    fgets(memberID, sizeof(memberID), stdin);
    memberID[strcspn(memberID, "\n")] = '\0';

    // Check if the member exists
    int memberIndex = -1;
    for(int i = 0; i < memberCount; i++)
    {
        if(strcmp(members[i].ID, memberID) == 0)
        {
            memberIndex = i;
            break;
        }
    }
    if(memberIndex == -1)
    {
        printf("No member found with this ID!\n");
        return;
    }

    // Prepare a new borrow record
    Borrow newBorrow;
    newBorrow.bookID = bookID;
    strcpy(newBorrow.memberID, memberID);

    printf("Enter borrow date (dd/mm/yyyy): ");
    fgets(newBorrow.borrowDate, sizeof(newBorrow.borrowDate), stdin);
    newBorrow.borrowDate[strcspn(newBorrow.borrowDate, "\n")] = '\0';

    // Return date can initially be blank or set to "-" as a placeholder
    strcpy(newBorrow.returnDate, "-");

    // Add to the borrows array
    borrows[borrowCount++] = newBorrow;

    // Update the book status
    books[bookIndex].status = 0;

    // Save to files
    saveBorrows(borrows, borrowCount);
    saveBooks(books, bookCount);

    printf("Book borrowing operation successful!\n");
}

/**
 * Returns a borrowed book and updates the records.
 */
void returnBook()
{
    Book   books[MAX_BOOKS];
    Borrow borrows[MAX_BORROWS];

    int bookCount   = loadBooks(books);
    int borrowCount = loadBorrows(borrows);

    int bookID;
    printf("Enter the ID of the book to return: ");
    scanf("%d", &bookID);
    clearInputBuffer();

    // Find the active borrow record
    int borrowIndex = -1;
    for(int i = 0; i < borrowCount; i++)
    {
        if(borrows[i].bookID == bookID && strcmp(borrows[i].returnDate, "-") == 0)
        {
            borrowIndex = i;
            break;
        }
    }
    if(borrowIndex == -1)
    {
        printf("No active borrow record found for this book!\n");
        return;
    }

    // Ask for return date
    printf("Enter return date (dd/mm/yyyy): ");
    fgets(borrows[borrowIndex].returnDate, sizeof(borrows[borrowIndex].returnDate), stdin);
    borrows[borrowIndex].returnDate[strcspn(borrows[borrowIndex].returnDate, "\n")] = '\0';

    // Update book status
    for(int i = 0; i < bookCount; i++)
    {
        if(books[i].ID == bookID)
        {
            books[i].status = 1;
            break;
        }
    }

    // Save changes
    saveBorrows(borrows, borrowCount);
    saveBooks(books, bookCount);

    printf("Book return operation successful!\n");
}

/**
 * Lists all borrow records.
 */
void listBorrows()
{
    Borrow borrows[MAX_BORROWS];
    int count = loadBorrows(borrows);

    if(count == 0)
    {
        printf("No borrow records found.\n");
        return;
    }

    printf("\n--- All Borrow Records ---\n");
    for(int i = 0; i < count; i++)
    {
        printf("BookID: %d | MemberID: %s | Borrowed: %s | Returned: %s\n",
               borrows[i].bookID,
               borrows[i].memberID,
               borrows[i].borrowDate,
               borrows[i].returnDate);
    }
}

/*
    -------------------------
    HELPER FUNCTIONS
    -------------------------
*/

/**
 * Validates the member's ID number (should be 11 digits long).
 */
int isValidMemberID(const char *id)
{
    if(strlen(id) != 11) return 0;
    for(int i = 0; i < 11; i++)
    {
        if(!isdigit((unsigned char)id[i]))
            return 0;
    }
    return 1;
}

/**
 * Clears the input buffer after using scanf, etc.
 */
void clearInputBuffer()
{
    int c;
    while((c = getchar()) != '\n' && c != EOF) { }
}
