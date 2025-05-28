#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   
extern const char *USERS; // User database file
extern const char *RECORDS; // Records database file

static const char *ISO_COUNTRY_CODES[] = {
    "AD","AE","AF","AG","AI","AL","AM","AO","AQ","AR","AS","AT","AU","AW","AX","AZ",
    "BA","BB","BD","BE","BF","BG","BH","BI","BJ","BL","BM","BN","BO","BQ","BR","BS",
    "BT","BV","BW","BY","BZ","CA","CC","CD","CF","CG","CH","CI","CK","CL","CM","CN",
    "CO","CR","CU","CV","CW","CX","CY","CZ","DE","DJ","DK","DM","DO","DZ","EC","EE",
    "EG","EH","ER","ES","ET","FI","FJ","FM","FO","FR","GA","GB","GD","GE","GF","GG",
    "GH","GI","GL","GM","GN","GP","GQ","GR","GT","GU","GW","GY","HK","HM","HN","HR",
    "HT","HU","ID","IE","IL","IM","IN","IO","IQ","IR","IS","IT","JE","JM","JO","JP",
    "KE","KG","KH","KI","KM","KN","KP","KR","KW","KY","KZ","LA","LB","LC","LI","LK",
    "LR","LS","LT","LU","LV","LY","MA","MC","MD","ME","MF","MG","MH","MK","ML","MM",
    "MN","MO","MP","MQ","MR","MS","MT","MU","MV","MW","MX","MY","MZ","NA","NC","NE",
    "NF","NG","NI","NL","NO","NP","NR","NU","NZ","OM","PA","PE","PF","PG","PH","PK",
    "PL","PM","PN","PR","PT","PW","PY","QA","RE","RO","RS","RU","RW","SA","SB","SC",
    "SD","SE","SG","SH","SI","SJ","SK","SL","SM","SN","SO","SR","SS","ST","SV","SX",
    "SY","SZ","TC","TD","TF","TG","TH","TJ","TK","TL","TM","TN","TO","TR","TT","TV",
    "TZ","UA","UG","UK","UM","US","UY","UZ","VA","VC","VE","VG","VI","VN","VU","WF","WS",
    "YE","YT","ZA","ZM","ZW",
    NULL
};



struct Date
{
    int month, day, year;
};

// Structure to represent a bank account record
struct Record
{
    int id;                 // Unique record ID
    int userId;             // ID of the user who owns this account
    char country[100];      // Country of the user (must be in ISO_COUNTRY_CODES)
    long long int phone;    // Phone number
    char accountType[10];   // Type of account (e.g., "savings", "checking")
    int accountNbr;         // Account number (unique)
    double amount;          // Current balance
    struct Date deposit;    // Date of last deposit
    struct Date withdraw;   // Date of last withdrawal
};

// Structure to represent the user
struct User
{
    int id;
    char name[50];
    char password[50];
};

// Authentication functions
void loginMenu(char username[50], char password[50]);
// Retrieves password for a given user
const char *getPassword(struct User *user);

// File operations
int getAccountFromFile(FILE *file, char username[50], struct Record *record);

// System control functions
int registerUser(struct User *user);
void createNewAccount(struct User user);
void mainMenu(struct User user);
void checkAllAccounts(struct User user);
void saveAccountToFile(FILE *file, struct User user, struct Record record);


// Additional operations (formerly TODOs) 
void updateAccountInfo(struct User user);
void viewAccountDetails(struct User user);
void makeTransaction(struct User user);
void removeAccount(struct User user);
void transferOwnership(struct User user);

// Control helpers
void success(struct User user);

