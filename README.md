# Library Management System in C

## 📖 Overview
This project is a Command Line Interface (CLI) application developed as a final project for my C Programming course. It simulates a basic library system where administrators can manage books, customers, and rental transactions.

The system uses **File Handling** to ensure data persistence, meaning all records (books, users, rentals) are saved to text files and retrieved upon restarting the program.

## 🚀 Features

### 👤 Customer Management
* **Create Customer:** Register new users with ID, name, age, and initial wallet balance.
* **Update Customer:** Modify existing user details.
* **Deposit Money:** Add funds to a customer's wallet to facilitate rentals.
* **List Customers:** View a database of all registered users.

### 📚 Book Management
* **Add Books:** Input book details (Title, Author, Age Limit, Price per Week).
* **Update Books:** Edit details of existing books.
* **Burn Book:** Permanently remove a book from the system (only if not currently rented).
* **List Books:** View inventory and check rental status (Available/Rented).

### 🔄 Rental System
* **Rent a Book:** * Validates customer age against book age limit.
    * Checks customer wallet balance.
    * Calculates cost based on rental duration (weeks).
    * Updates inventory status automatically.
* **Deliver (Return) Book:**
    * Returns the book to inventory.
    * **Refund Policy:** Automatically refunds 1 week's worth of fees to the customer upon return.
* **Track Rentals:** List all active rentals.

## 🛠️ Technical Implementation
* **Language:** C
* **Data Structures:** `struct` used for Customers, Books, and Rentals.
* **Persistence:** `fprintf` and `fscanf` used to store data in CSV-style text files (`customers.txt`, `books.txt`, `rented.txt`).
* **Logic:** Input validation for existing IDs, sufficient funds, and age restrictions.

## 💻 How to Run

1.  **Clone the repository**
    ```bash
    git clone [https://github.com/ahsndrmz05/library_management_system_inC.git](https://github.com/ahsndrmz05/library_management_system_inC.git)
    ```

2.  **Compile the code**
    You need a C compiler (like GCC). Run the following command in your terminal:
    ```bash
    gcc main.c -o library_app
    ```
    *(Note: It is recommended to rename the source file to `main.c` for easier compilation)*

3.  **Run the application**
    ```bash
    ./library_app
    ```

## 📂 File Structure
* `main.c`: The main source code.
* `customers.txt`: Stores user ID, Name, Surname, Age, and Wallet Balance.
* `books.txt`: Stores Book ID, Title, Author, Age Limit, Price, and Rental Status.
* `rented.txt`: Links Customer IDs to Book IDs and tracks rental dates.

## 📝 Author
**Ahsen Durmaz** *Computer Engineering Student*
