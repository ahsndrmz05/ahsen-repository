#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_AUTHOR 50
#define MAX_DATE 11

// Structure Definitions
typedef struct {
    int id;
    char name[MAX_NAME];
    char surname[MAX_NAME];
    int age;
    int wallet;
} Customer;

typedef struct {
    int id;
    char name[MAX_NAME];
    char author[MAX_AUTHOR];
    int age_limit;
    int price_per_week;
    int rented;
} Book;

typedef struct {
    int id;
    int customer_id;
    int book_id;
    char rented_date[MAX_DATE];
    int weeks;
} Rental;

// File Functions
void save_customer(Customer c) {
    FILE *f = fopen("customers.txt", "a");
    fprintf(f, "%d,%s,%s,%d,%d\n", c.id, c.name, c.surname, c.age, c.wallet);
    fclose(f);
}

void save_book(Book b) {
    FILE *f = fopen("books.txt", "a");
    fprintf(f, "%d,%s,%s,%d,%d,%d\n", b.id, b.name, b.author, b.age_limit, b.price_per_week, b.rented);
    fclose(f);
}

void save_rental(Rental r) {
    FILE *f = fopen("rented.txt", "a");
    fprintf(f, "%d,%d,%d,%s,%d\n", r.id, r.customer_id, r.book_id, r.rented_date, r.weeks);
    fclose(f);
}

// Utility
int get_last_id(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    int id = 0, tmp;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f)) {
        sscanf(buffer, "%d", &tmp);
        if (tmp > id) id = tmp;
    }
    fclose(f);
    return id;
}

int check_customer_exists(const char *name, const char *surname) {
    FILE *f = fopen("customers.txt", "r");
    if (!f) return 0;
    Customer c;
    while (fscanf(f, "%d,%49[^,],%49[^,],%d,%d\n", &c.id, c.name, c.surname, &c.age, &c.wallet) != EOF) {
        if (strcmp(c.name, name) == 0 && strcmp(c.surname, surname) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int check_book_exists(const char *name, const char *author) {
    FILE *f = fopen("books.txt", "r");
    if (!f) return 0;
    Book b;
    while (fscanf(f, "%d,%49[^,],%49[^,],%d,%d,%d\n", &b.id, b.name, b.author, &b.age_limit, &b.price_per_week, &b.rented) != EOF) {
        if (strcmp(b.name, name) == 0 && strcmp(b.author, author) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

Customer find_customer(int id) {
    Customer c = {-1};
    FILE *f = fopen("customers.txt", "r");
    while (fscanf(f, "%d,%[^,],%[^,],%d,%d\n", &c.id, c.name, c.surname, &c.age, &c.wallet) != EOF) {
        if (c.id == id) break;
    }
    fclose(f);
    return c;
}

Book find_book(int id) {
    Book b = {-1};
    FILE *f = fopen("books.txt", "r");
    while (fscanf(f, "%d,%[^,],%[^,],%d,%d,%d\n", &b.id, b.name, b.author, &b.age_limit, &b.price_per_week, &b.rented) != EOF) {
        if (b.id == id) break;
    }
    fclose(f);
    return b;
}

// Operations
void create_customer() {
    Customer c;
    printf("Name: "); scanf("%s", c.name);
    printf("Surname: "); scanf("%s", c.surname);
    printf("Age: "); scanf("%d", &c.age);
    printf("Wallet: "); scanf("%d", &c.wallet);

    if (check_customer_exists(c.name, c.surname)) {
        printf("Customer already exists.\n");
        return;
    }

    c.id = get_last_id("customers.txt") + 1;
    save_customer(c);
    printf("Customer added with ID %d.\n", c.id);
}

void deposit_money() {
    int id, amount;
    printf("Customer ID: "); scanf("%d", &id);
    printf("Amount: "); scanf("%d", &amount);

    FILE *f = fopen("customers.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    Customer c;
    while (fscanf(f, "%d,%[^,],%[^,],%d,%d\n", &c.id, c.name, c.surname, &c.age, &c.wallet) != EOF) {
        if (c.id == id) c.wallet += amount;
        fprintf(temp, "%d,%s,%s,%d,%d\n", c.id, c.name, c.surname, c.age, c.wallet);
    }
    fclose(f); fclose(temp);
    remove("customers.txt"); rename("temp.txt", "customers.txt");
    printf("Deposit complete.\n");
}

void add_book() {
    Book b;
    printf("Book Name: "); scanf(" %[^'\n']", b.name);
    printf("Author: "); scanf(" %[^'\n']", b.author);
    printf("Age Limit: "); scanf("%d", &b.age_limit);
    printf("Price per Week: "); scanf("%d", &b.price_per_week);

    if (check_book_exists(b.name, b.author)) {
        printf("Book already exists.\n");
        return;
    }

    b.id = get_last_id("books.txt") + 1;
    b.rented = 0;
    save_book(b);
    printf("Book added with ID %d.\n", b.id);
}
void rent_book() {
    int customer_id, book_id, weeks;
    char date[MAX_DATE];
    printf("Enter Customer ID: "); scanf("%d", &customer_id);
    printf("Enter Book ID: "); scanf("%d", &book_id);
    printf("Enter Rented Date (YYYY-MM-DD): "); scanf("%s", date);
    printf("Enter Weeks: "); scanf("%d", &weeks);

    Customer c = find_customer(customer_id);
    Book b = find_book(book_id);

    if (c.id == -1 || b.id == -1) {
        printf("Invalid customer or book ID.\n"); return;
    }
    if (b.rented) {
        printf("Book is already rented.\n"); return;
    }
    if (c.age < b.age_limit) {
        printf("Customer does not meet age limit.\n"); return;
    }
    if (c.wallet < b.price_per_week * weeks) {
        printf("Insufficient wallet balance.\n"); return;
    }

    // Update customer
    FILE *fc = fopen("customers.txt", "r"), *fct = fopen("temp.txt", "w");
    Customer tmp;
    while (fscanf(fc, "%d,%[^,],%[^,],%d,%d\n", &tmp.id, tmp.name, tmp.surname, &tmp.age, &tmp.wallet) != EOF) {
        if (tmp.id == c.id) tmp.wallet -= b.price_per_week * weeks;
        fprintf(fct, "%d,%s,%s,%d,%d\n", tmp.id, tmp.name, tmp.surname, tmp.age, tmp.wallet);
    }
    fclose(fc); fclose(fct);
    remove("customers.txt"); rename("temp.txt", "customers.txt");

    // Update book
    FILE *fb = fopen("books.txt", "r"), *fbt = fopen("temp.txt", "w");
    while (fscanf(fb, "%d,%[^,],%[^,],%d,%d,%d\n", &tmp.id, tmp.name, tmp.surname, &tmp.age, &tmp.wallet, &b.rented) != EOF) {
        if (tmp.id == b.id) b.rented = 1;
        fprintf(fbt, "%d,%s,%s,%d,%d,%d\n", b.id, b.name, b.author, b.age_limit, b.price_per_week, b.rented);
    }
    fclose(fb); fclose(fbt);
    remove("books.txt"); rename("temp.txt", "books.txt");

    // Save rental
    Rental r;
    r.id = get_last_id("rented.txt") + 1;
    r.customer_id = c.id;
    r.book_id = b.id;
    strcpy(r.rented_date, date);
    r.weeks = weeks;
    save_rental(r);
    printf("Book rented successfully.\n");
}

void list_renting_customers() {
    FILE *fr = fopen("rented.txt", "r");
    Rental r;
    printf("\nCustomers Renting Books:\n");
    while (fscanf(fr, "%d,%d,%d,%[^,],%d\n", &r.id, &r.customer_id, &r.book_id, r.rented_date, &r.weeks) != EOF) {
        Customer c = find_customer(r.customer_id);
        printf("Customer ID %d - %s %s is renting Book ID %d for %d weeks\n",
               c.id, c.name, c.surname, r.book_id, r.weeks);
    }
    fclose(fr);
}
void deliver_book() {
    int rental_id;
    printf("Enter Rental ID: ");
    scanf("%d", &rental_id);

    FILE *fr = fopen("rented.txt", "r");
    FILE *ftemp = fopen("temp.txt", "w");
    Rental r;
    int found = 0;

    while (fscanf(fr, "%d,%d,%d,%[^,],%d\n", &r.id, &r.customer_id, &r.book_id, r.rented_date, &r.weeks) != EOF) {
        if (r.id == rental_id) {
            Customer c = find_customer(r.customer_id);
            Book b = find_book(r.book_id);
            // Refund 1 week
            FILE *fc = fopen("customers.txt", "r");
            FILE *fct = fopen("ctemp.txt", "w");
            Customer tmp;
            while (fscanf(fc, "%d,%[^,],%[^,],%d,%d\n", &tmp.id, tmp.name, tmp.surname, &tmp.age, &tmp.wallet) != EOF) {
                if (tmp.id == c.id) tmp.wallet += b.price_per_week;
                fprintf(fct, "%d,%s,%s,%d,%d\n", tmp.id, tmp.name, tmp.surname, tmp.age, tmp.wallet);
            }
            fclose(fc); fclose(fct);
            remove("customers.txt"); rename("ctemp.txt", "customers.txt");

            FILE *fb = fopen("books.txt", "r");
            FILE *fbt = fopen("btemp.txt", "w");
            Book tmpb;
            while (fscanf(fb, "%d,%[^,],%[^,],%d,%d,%d\n", &tmpb.id, tmpb.name, tmpb.author, &tmpb.age_limit, &tmpb.price_per_week, &tmpb.rented) != EOF) {
                if (tmpb.id == b.id) tmpb.rented = 0;
                fprintf(fbt, "%d,%s,%s,%d,%d,%d\n", tmpb.id, tmpb.name, tmpb.author, tmpb.age_limit, tmpb.price_per_week, tmpb.rented);
            }
            fclose(fb); fclose(fbt);
            remove("books.txt"); rename("btemp.txt", "books.txt");
            found = 1;
        } else {
            fprintf(ftemp, "%d,%d,%d,%s,%d\n", r.id, r.customer_id, r.book_id, r.rented_date, r.weeks);
        }
    }
    fclose(fr); fclose(ftemp);
    remove("rented.txt"); rename("temp.txt", "rented.txt");

    if (found)
        printf("Book delivered and refund applied.\n");
    else
        printf("Rental ID not found.\n");
}

void burn_book() {
    int id;
    printf("Enter Book ID: ");
    scanf("%d", &id);
    Book b;
    FILE *fb = fopen("books.txt", "r");
    FILE *fbt = fopen("temp.txt", "w");
    int found = 0;
    while (fscanf(fb, "%d,%[^,],%[^,],%d,%d,%d\n", &b.id, b.name, b.author, &b.age_limit, &b.price_per_week, &b.rented) != EOF) {
        if (b.id == id && b.rented == 0) {
            found = 1; continue;
        }
        fprintf(fbt, "%d,%s,%s,%d,%d,%d\n", b.id, b.name, b.author, b.age_limit, b.price_per_week, b.rented);
    }
    fclose(fb); fclose(fbt);
    remove("books.txt"); rename("temp.txt", "books.txt");
    if (found) printf("Book burned successfully.\n");
    else printf("Cannot burn rented book or book not found.\n");
}
void update_customer() {
    int id, age;
    char name[MAX_NAME], surname[MAX_NAME];
    printf("Customer ID: "); scanf("%d", &id);
    printf("New Name: "); scanf("%s", name);
    printf("New Surname: "); scanf("%s", surname);
    printf("New Age: "); scanf("%d", &age);
    FILE *fc = fopen("customers.txt", "r");
    FILE *fct = fopen("temp.txt", "w");
    Customer c;
    while (fscanf(fc, "%d,%[^,],%[^,],%d,%d\n", &c.id, c.name, c.surname, &c.age, &c.wallet) != EOF) {
        if (c.id == id) {
            strcpy(c.name, name);
            strcpy(c.surname, surname);
            c.age = age;
        }
        fprintf(fct, "%d,%s,%s,%d,%d\n", c.id, c.name, c.surname, c.age, c.wallet);
    }
    fclose(fc); fclose(fct);
    remove("customers.txt"); rename("temp.txt", "customers.txt");
    printf("Customer updated.\n");
}
void update_book() {
    int id, price, age_limit;
    char name[MAX_NAME], author[MAX_AUTHOR];
    printf("Book ID: "); scanf("%d", &id);
    printf("New Name: "); scanf(" %[^'\n']", name);
    printf("New Author: "); scanf(" %[^'\n']", author);
    printf("New Age Limit: "); scanf("%d", &age_limit);
    printf("New Price/Week: "); scanf("%d", &price);
    FILE *fb = fopen("books.txt", "r");
    FILE *fbt = fopen("temp.txt", "w");
    Book b;
    while (fscanf(fb, "%d,%[^,],%[^,],%d,%d,%d\n", &b.id, b.name, b.author, &b.age_limit, &b.price_per_week, &b.rented) != EOF) {
        if (b.id == id) {
            strcpy(b.name, name);
            strcpy(b.author, author);
            b.age_limit = age_limit;
            b.price_per_week = price;
        }
        fprintf(fbt, "%d,%s,%s,%d,%d,%d\n", b.id, b.name, b.author, b.age_limit, b.price_per_week, b.rented);
    }
    fclose(fb); fclose(fbt);
    remove("books.txt"); rename("temp.txt", "books.txt");
    printf("Book updated.\n");
}
void list_customers() {
    FILE *f = fopen("customers.txt", "r");
    Customer c;
    printf("\nCustomers:\n");
    while (fscanf(f, "%d,%[^,],%[^,],%d,%d\n", &c.id, c.name, c.surname, &c.age, &c.wallet) != EOF) {
        printf("%d: %s %s, Age: %d, Wallet: %d\n", c.id, c.name, c.surname, c.age, c.wallet);
    }
    fclose(f);
}

void list_books() {
    FILE *f = fopen("books.txt", "r");
    Book b;
    printf("\nBooks:\n");
    while (fscanf(f, "%d,%[^,],%[^,],%d,%d,%d\n", &b.id, b.name, b.author, &b.age_limit, &b.price_per_week, &b.rented) != EOF) {
        printf("%d: %s by %s, Age Limit: %d, Price/Week: %d, Rented: %s\n", b.id, b.name, b.author, b.age_limit, b.price_per_week, b.rented ? "Yes" : "No");
    }
    fclose(f);
}

//Main menu
void menu() {
    int choice;
    do {
        printf("\nLibrary Management System\n");
        printf("1. Create New Customer\n2. Deposit Money to Customer\n3. Add New Book\n4. Rent a Book\n5. Deliver a Book\n6. Burn a Book\n7. Update Customer\n8. Update Book\n9. List Renting Customers\n10. List Customers\n11. List Books\n0. Exit\nSelect: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: create_customer(); break;
            case 2: deposit_money(); break;
            case 3: add_book(); break;
            case 4: rent_book(); break;
            case 5: deliver_book(); break;
            case 6: burn_book(); break;
            case 7: update_customer(); break;
            case 8: update_book(); break;
            case 9: list_renting_customers(); break;
            case 10: list_customers(); break;
            case 11: list_books(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

int main() {
    menu();
    return 0;
}