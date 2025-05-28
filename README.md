# ATM MANAGEMENT SYSTEM

A terminal-based C program that emulates essential banking functions. Created for learning purposes, it showcases user login, account handling, money transfers, and interest calculation, with strong input validation.

## Project Structure

```
.
├── data
│   ├── records.txt 
│   └── users.txt 
├── src
│   ├── auth.c 
│   ├── header.h 
│   ├── main.c 
│   ├── system.c
├── Makefile
└── README.md 
```

## Requirements

* A C99-compatible compiler such as `gcc` or `clang`.
* The `make` utility to build the project
* A Unix-based environment (Linux or macOS) that supports terminal I/O

## Build & Install

1. Open a terminal and navigate to the project root directory.
2. Run the following command:

   ```
   make
   ```
3. This will compile the source code and produce an executable named `atm`.

## Usage

Start the application:

```
./atm
```

Once launched, the application presents an interactive menu that allows users to:

* **Login** to an existing account.
* **Register** as a new user.
* **Account operations:** Create, view, modify, or remove accounts.
* **Transaction handling:** Make deposits and withdrawals.
* **Ownership management:p** Transfer accounts between users.

## Data Files

The application relies on two main data files for storing user and account information::
* `data/user.txt` : Contains user credentials in the format: `<userId>` `<username>` `<password>`

* `data/records.txt` : Stores account records using the format: `<recordId>` `<userId>` `<username>` `<accountNbr>` `<MM/DD/YYYY>` `<countryCode>` `<phone>` `<amount>` `<accountType>`

## Features
* **User Authentication**
    * Secure password entry (input is hidden).

    * Enforcement of unique usernames to prevent duplicates.

* **Account Management**
    * Create new accounts with:

        * Opening date

        * Country code (ISO format)

        * Phone number

        * Initial deposit

        * Account type

        * View all accounts or search by account number

        * Update account details (e.g., phone number or country code)

        * Permanently delete existing accounts

* **Financial Transactions**
    * Make deposits and withdrawals

    * Built-in overdraft protection to prevent negative balances

* **Ownership Transfer**
    * Transfer account ownership between users

    * Resolves conflicts in case of duplicate account numbers

## Validation Rules
To ensure data consistency and system integrity, the following validation rules apply:

* Usernames:
Minimum of 2 characters and must be unique.

* Passwords:
Minimum of 8 characters and stored in plain text within the file system (handled securely during input).

* Dates:
Acceptable formats: M/D/YYYY or MM/DD/YYYY
Valid year range: 1950–2150

* Country Codes:
Must follow the ISO 3166-1 alpha-2 standard (e.g., US, GR, FR), uppercase only.

* Phone Numbers:
Digits only, no special characters or spaces and must be positive.

* Amounts:
Only positive numeric values are allowed.

* Account Types:
Must be one of the following:

    * savings

    * current

    * fixed01

    * fixed02

    * fixed03

## License
© 2025 Andrikopoulos Andreas Rafail - Zone01 Athens