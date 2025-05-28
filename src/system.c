#include "header.h"
#include <time.h>
#include <ctype.h>
#include <string.h>

const char *USERS = "./data/users.txt"; // Path to user databas file
const char *RECORDS = "./data/records.txt"; //records data storage

int iso() {
    for (int i = 0; ISO_COUNTRY_CODES[i] != NULL; i++) {
        printf("%s\n", ISO_COUNTRY_CODES[i]);
    }
    return 0;
}

// Checks if s represents a positive number
static int isValidPositiveNumber(const char *s) {
    if (!s || !*s) return 0;
    char *end;
    double v = strtod(s, &end);
    // Must consume input, reach end of string, and value > 0
    return end != s 
        && *end == '\0' 
        && v > 0.0;
}

// Returns 1 if the string contains only digits (0–9), otherwise returns 0.
static int isAllDigits(const char *s) {
    if (!s || !*s) return 0;
    for (size_t i = 0; s[i]; ++i) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

//Checks whether a given 2-letter code s is a valid ISO country code (must match exactly one in the list).
static int isValidCountryCode(const char *s) {
    if (!s || strlen(s) != 2) return 0;
    for (int i = 0; ISO_COUNTRY_CODES[i] != NULL; i++) {
        if (strcmp(s, ISO_COUNTRY_CODES[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int registerUser(struct User *u)
{
    FILE *file = fopen(USERS, "a+"); // Open the users file in read/write (append) mode
    if (!file)
    {
        perror("Error opening users file.");
        exit(1);
    }

    struct User temp;
    char tempName[50];
    char tempPassword[50];
    char confirmPassword[50];
    int exists;

    printf("\n\t\t======= Registration =======\n\n");

    // Username registration loop
    do {
        exists = 0;
        printf("Enter a username (or press 0 and ENTER to cancel): ");
        scanf("%49s", tempName);
        while (getchar() != '\n'); // Clear input buffer

        if (strcmp(tempName, "0") == 0) {
            printf("\nRegistration canceled. Press ENTER to return to main menu.\n");
            getchar();
            fclose(file);
            return 0;  // User canceled registration
        }

        // Enforce minimum username length
        if (strlen(tempName) < 2) {
            printf("Username is too small. Please try again with at least 2 characters.\n\n");
            exists = 1;
            continue;
        }

        // Check for duplicate usernames
        rewind(file);
        while (fscanf(file, "%d %49s %49s",&temp.id,temp.name,temp.password) == 3) {
            if (strcasecmp(tempName, temp.name) == 0)
            {
                printf("Username \"%s\" already exists. Please try again.\n\n", tempName);
                exists = 1;
                break;
            }
        }
    } while (exists);

    
    // Password entry loop
    do {
        printf("Enter a password (at least 8 characters) (or press 0 and ENTER to cancel): ");
        scanf("%49s", tempPassword);
        while (getchar() != '\n');

        if (strcmp(tempPassword, "0") == 0) {
            fclose(file);
            return 0;  // User canceled
        }

        if (strlen(tempPassword) < 8 ) {
            printf("Password is too small. Please try again with at least 8 characters.\n\n");
            }
    
        printf("Confirm password :");
        scanf("%49s", confirmPassword);
        while (getchar() != '\n');

        if (strcmp(confirmPassword, "0") == 0) {
            fclose(file);
            return 0;  // User canceled
        }
    
        if (strcmp(tempPassword, confirmPassword) != 0) {
            printf("Passwords do not match. Please try again.\n\n");
        }
        } while (strlen(tempPassword) < 8 || strcmp(tempPassword, confirmPassword) != 0);
    
    // Assign unique user ID
    rewind(file);
    int maxId = 0;
    while (fscanf(file, "%d %49s %49s",&temp.id,temp.name,temp.password) == 3) {
        if (temp.id > maxId) {
            maxId = temp.id;
        }
    }
    u->id = maxId + 1;

    // Save user info to struct
    strcpy(u->name, tempName);
    strcpy(u->password, tempPassword);

    // Append the new user to the file
    fprintf(file, "%d %s %s\n", u->id, u->name, u->password);
    fclose(file);

    printf("\nRegistration successful! Press ENTER to continue.\n\n");
    getchar();  // wait for user to press ENTER
    return 1;
}

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    int result = fscanf(ptr, "%d %d %s %d %d/%d/%d %s %lld %lf %s",
                        &r->id,
                        &r->userId,
                        name,
                        &r->accountNbr,
                        &r->deposit.month,
                        &r->deposit.day,
                        &r->deposit.year,
                        r->country,
                        &r->phone,
                        &r->amount,
                        r->accountType);

    return result != EOF;
}


void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    // We overwrite the old record with the new one
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %lld %.2lf %s\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);

    fprintf(ptr, "\n");
    fflush(ptr);
}

void success(struct User u)
{
    int option;
invalid:
    printf("\n"
        "  [1] Main Menu\n"
        "  [0] Exit\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAccount(struct User u) {
    struct Record r, cr;
    char userName[50];
    char input[50];
    FILE *pf = fopen(RECORDS, "a+");
    if (!pf) {
        perror("Error opening records file.");
        exit(1);
    }

    // Determine next record ID
    int newRecordId = 0;
    rewind(pf);
    while (getAccountFromFile(pf, userName, &cr)) {
        if (cr.id > newRecordId) {
            newRecordId = cr.id;
        }
    }
    newRecordId++;
    r.id = newRecordId;

    // Loop until we get all inputs valid
    while (1) {
        int m, d, y;

        system("clear");
        printf("\t\t\t===== New Account Creation =====\n");

        // ——— DATE INPUT ———
        while (1) {
            printf("\nEnter the deposit date (M/D/YYYY or MM/DD/YYYY) [or press 0 and ENTER to go back to Main Menu]: ");
            fgets(input, sizeof(input), stdin);

            input[strcspn(input, "\n")] = '\0';

            if (strcmp(input, "0")== 0) {
                fclose(pf);
                mainMenu(u);
                return;
            }
            // Read 3 intigers separated by '/'
            if (sscanf(input, "%d/%d/%d", &m, &d, &y) != 3) {
                printf("Invalid format. Please use M/D/YYYY or MM/DD/YYYY.\n");
                // while (getchar() != '\n'); 
                continue;
            }
            while (getchar() != '\n');
            

            // Validate the date
            if (m < 1 || m > 12 || d < 1 || d > 31) {
                printf("Month/day out of range.\n");
                continue;
            }
            if (y < 1950 || y > 2150) {
                printf("Year must be between 1950 and 2150.\n");
                continue;
            }

            // Save valid date
            r.deposit.month = m;
            r.deposit.day   = d;
            r.deposit.year  = y;
            break;
        }

    // ——— ACCOUNT NUMBER ———
    while (1) {
        int accountExists = 0;
        printf("\nEnter the account number: ");
        if (scanf("%d", &r.accountNbr) != 1 || r.accountNbr <= 0) {
            printf("Invalid account number. Must be a positive integer.\n");
            while (getchar()!='\n');
            continue;
        }
        // Clear rest of line
        while (getchar()!='\n');

        // Check duplicates
        rewind(pf);
        while (getAccountFromFile(pf, userName, &cr)) {
            if (cr.userId == u.id && cr.accountNbr == r.accountNbr) {
                accountExists = 1;
                break;
            }
        }
        if (accountExists) {
            printf("Account number %d already exists for you. Please choose a different number.\n",
                   r.accountNbr);
            continue;
        }
        break;
    }

// ——— COUNTRY CODE ———
{
    char line[16], code[3];
    while (1) {
        printf("\nEnter country code e.g. US,): ");
        if (!fgets(line, sizeof line, stdin)) {
            clearerr(stdin);
            continue;
        }
        // If input was longer than buffer, flush the rest
        if (!strchr(line, '\n')) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        line[strcspn(line, "\n")] = '\0';

        // Must be exactly 2 characters
        if (strlen(line) != 2) {
            printf("Please enter exactly two letters (e.g. US).\n");
            continue;
        }
        // Both must be uppercase A–Z
        if (!isupper((unsigned char)line[0]) ||
            !isupper((unsigned char)line[1])) {
            printf("Both characters must be uppercase A–Z.\n");
            continue;
        }
        // Check against ISO_COUNTRY_CODES list
        strcpy(code, line);
        if (!isValidCountryCode(code)) {
            printf("“%s” is not a valid ISO country code.\n", code);
            continue;
        }
        
        strcpy(r.country, code);
        break;
    }
}

// ——— PHONE NUMBER ———
{
    char phone_buf[32];
    while (1) {
        printf("\nEnter the phone number: ");
        if (!fgets(phone_buf, sizeof phone_buf, stdin)) {
            clearerr(stdin);
            continue;
        }
        phone_buf[strcspn(phone_buf, "\n")] = '\0';

        if (!isAllDigits(phone_buf)) {
            printf("Invalid phone number. Please enter only digits (e.g. 6912345678).\n");
            continue;
        }

        // If all characters are digits, convert the phone number to long long 
        r.phone = atoll(phone_buf);
        if (r.phone <= 0) {
            printf("Phone number must be positive digits.\n");
            continue;
        }
        break;
    }
}

// ——— INITIAL DEPOSIT ———
char amt_buf[64];
while (1) {
    printf("\nEnter amount to deposit (must be positive): $");
    if (!fgets(amt_buf, sizeof amt_buf, stdin)) {
        clearerr(stdin);
        continue;
    }
    amt_buf[strcspn(amt_buf, "\n")] = '\0';

    if (!isValidPositiveNumber(amt_buf)) {
        printf("Invalid deposit amount. Please type a positive number (e.g. 1000.50).\n");
        continue;
    }

    // If the number is valid , saved it to r.amount
    r.amount = strtod(amt_buf, NULL);
    break;
}

// ——— ACCOUNT TYPE ———
while (1) {
    int typeChoice;
    printf(
        "\nChoose the type of account:\n"
        "  [1] savings\n"
        "  [2] current\n"
        "  [3] fixed01 (1 year)\n"
        "  [4] fixed02 (2 years)\n"
        "  [5] fixed03 (3 years)\n"
        "Enter your choice (1-5): "
    );
    int ret = scanf("%d", &typeChoice);
    while (getchar() != '\n');

    if (ret != 1 || typeChoice < 1 || typeChoice > 5) {
        printf("Invalid choice. Please enter a number between 1 and 5.\n");
        continue;
    }

    // If the choise is correct , save it to r.account
    switch (typeChoice) {
        case 1: strcpy(r.accountType, "savings"); break;
        case 2: strcpy(r.accountType, "current"); break;
        case 3: strcpy(r.accountType, "fixed01"); break;
        case 4: strcpy(r.accountType, "fixed02"); break;
        case 5: strcpy(r.accountType, "fixed03"); break;
    }
    break;
}

        // ——— SAVE ———
        saveAccountToFile(pf, u, r);
        fclose(pf);
        printf("\nAccount created successfully!\n");
        printf("\nNew account number %d was created for %s! \n\n", r.accountNbr, u.name);
        success(u);
        break;
    }
}

// Read all the accounts of user and print them.
void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    
    int found = 0;
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%lld \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            found = 1;
        }
    }

    // If there is not an account print the message.
    if (!found)
    {
        printf("\nNo accounts found for user %s.\n", u.name);
    }
    fclose(pf);
    success(u);  // Returning to the main menu after checking the list.
}

void updateAccountInfo(struct User u) {
    const char *tmpPath = "./data/records.tmp";
    char name[50];
    struct Record r;

    while (1) {
        int accountId, found = 0;
        FILE *in, *out;

        // ─── YOUR PROMPT & VALIDATION ───
        system("clear");
        printf("\n\t\t=== Update Account Information ===\n");
        printf("Enter the account number to update: ");
        if (scanf("%d", &accountId) != 1) {
            printf("Invalid input. Please enter a number (or press 0 and ENTER to go bak to Main Menu).\n");
            while (getchar() != '\n');    
            printf("Press ENTER to try again.");
            getchar();
            continue;
        }
        while (getchar() != '\n');
        if (accountId == 0) {
            mainMenu(u);
            return;
        }

        // ─── OPEN ORIGINAL & TEMP FILES ───
        in  = fopen(RECORDS, "r");
        out = fopen(tmpPath, "w");
        if (!in || !out) {
            perror("Error opening records file.");
            exit(1);
        }

        // ─── SCAN & MODIFY THE MATCH ───
        while (getAccountFromFile(in, name, &r)) {
            if (!found && r.userId == u.id && r.accountNbr == accountId) {
                found = 1;

            // — Show current details —
            printf("\nCurrent Account Details:\n");
            printf("  Account number: %d\n", r.accountNbr);
            printf("  Deposit Date: %02d/%02d/%04d\n",
                   r.deposit.month, r.deposit.day, r.deposit.year);
            printf("  Country: %s\n", r.country);
            printf("  Phone number: %lld\n", r.phone);
            printf("  Amount deposited: $%.2f\n", r.amount);
           printf("  Account Type: %s\n\n", r.accountType);

            // — Choose field to update —
            int option;
            while (1) {
                printf("Select the field to update:\n");
                printf("  [1] Phone number\n");
                printf("  [2] Country code\n");
                printf("Enter your choice (1-2): ");
                if (scanf("%d", &option) != 1) {
                    printf("Invalid input. Enter 1 or 2 (or press 0 and ENTER to go back to Main Menu).\n");
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');

                while (getchar() != '\n');
                if (accountId == 0) {
                    mainMenu(u);
                  return;
                }

                if (option == 1) {
                    // — Update phone —
                    while (1) {
                        char phone_buf[32];
                        printf("Enter the new phone number: ");
                        if (!fgets(phone_buf, sizeof phone_buf, stdin)) {
                            clearerr(stdin);
                            continue;
                        }
                        phone_buf[strcspn(phone_buf, "\n")] = '\0';
                        if (!isAllDigits(phone_buf)) {
                            printf("Invalid phone. Digits only.\n");
                            continue;
                        }
                        r.phone = atoll(phone_buf);
                        break;
                    }
                    break;
                }
                else if (option == 2) {
                    // — Update country —
                    while (1) {
                        char line[16];
                        printf("Enter the new country code (e.g. US): ");
                        if (!fgets(line, sizeof line, stdin)) {
                            clearerr(stdin);
                            continue;
                        }
                        // flush excess
                        if (!strchr(line, '\n')) {
                            int c; while ((c = getchar())!='\n' && c!=EOF);
                        }
                        line[strcspn(line, "\n")] = '\0';
                        if (strlen(line) != 2
                         || !isupper((unsigned char)line[0])
                         || !isupper((unsigned char)line[1])
                         || !isValidCountryCode(line))
                        {
                            printf("Invalid country code.\n");
                            continue;
                        }
                        strcpy(r.country, line);
                        break;
                    }
                    break;
                }
                else {
                    printf("Invalid option. Enter 1 or 2.\n");
                }
            }
        } 

        // Write possibly updated record to temp file —
            fprintf(out,
                "%d %d %s %d %02d/%02d/%04d %s %lld %.2f %s\n",
                r.id, r.userId, name,
                r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
        }

        fclose(in);
        fclose(out);

        // ─── NOT FOUND───
        if (!found) {
            remove(tmpPath);
            printf("Account %d not found or you don't have access.\n", accountId);
            printf("Press ENTER to try again.");
            getchar();
            continue;
        }

        // ─── SUCCESS ───
        remove(RECORDS);
        rename(tmpPath, RECORDS);
        printf("\nAccount updated successfully!\n\n");
        success(u);
        return;
    }
}

void viewAccountDetails(struct User u) {
    const char *tmpPath = "./data/records.tmp"; 
    char name[50];
    struct Record r;
    int accountId, found;
    FILE *in;

    while (1) {
        // ─── Prompt & validate the account number ───
        system("clear");
        printf("\n\t\t=== View Account Details ===\n");
        printf("Enter the account number to view (or 0 to go back): ");
        if (scanf("%d", &accountId) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');    
            printf("Press ENTER to try again.");
            // Give the prompt back
            getchar();
            continue;                      
        }
        while (getchar() != '\n');
        if (accountId == 0) {
            mainMenu(u);
            return;
        }

        // ─── Open the records file & scan for a match ───
        in = fopen(RECORDS, "r");
        if (!in) {
            perror("Error opening records file.");
            exit(1);
        }

        found = 0;
        while (getAccountFromFile(in, name, &r)) {
            if (r.userId == u.id && r.accountNbr == accountId) {
                found = 1;
                break;
            }
        }
        fclose(in);

        // ─── Handle not-found or display-details ───
        if (!found) {
            printf("\nAccount %d not found or you don't have access.\n", accountId);
            printf("Press ENTER to try again.");
            getchar();
            continue;
        }

        // ─── Display the record ───
        system("clear");
        printf("\n\t\t=== Account #%d Details ===\n\n", accountId);
        printf("  Record ID:        %d\n",     r.id);
        printf("  Account Number:   %d\n",     r.accountNbr);
        printf("  Deposit Date:     %02d/%02d/%04d\n",
               r.deposit.month, r.deposit.day, r.deposit.year);
        printf("  Country Code:     %s\n",     r.country);
        printf("  Phone Number:     %lld\n",   r.phone);
        printf("  Current Balance:  $%.2f\n",  r.amount);
        printf("  Account Type:     %s\n\n",   r.accountType);

        // ─── SHOW INTEREST INFO ───
        if (strcmp(r.accountType, "current") == 0) {
            printf("You will not get interests because the account is of type current.\n");
        }
        else {
            double rate;
            int term = 0;
            if (strcmp(r.accountType, "savings") == 0) {
                rate = 0.07;
            }
            else if (strcmp(r.accountType, "fixed01") == 0) {
                rate = 0.04;  term = 1;
            }
            else if (strcmp(r.accountType, "fixed02") == 0) {
                rate = 0.05;  term = 2;
            }
            else {  // fixed03
                rate = 0.08;  term = 3;
            }

            if (strcmp(r.accountType, "savings") == 0) {
                // Monthly interest for savings
                double interest = r.amount * rate / 12.0;
                printf("You will get $%.2f as interest on day %d of every month!\n",
                       interest, r.deposit.day);
            }
            else {
                double interest = r.amount * rate * term;
                // Compute maturity date = deposit date + term years
                struct tm tm = {0};
                tm.tm_mday = r.deposit.day;
                tm.tm_mon  = r.deposit.month - 1;
                tm.tm_year = r.deposit.year  - 1900 + term;
                mktime(&tm);  // Normalize
                printf("You will get $%.2f as interest on maturity date %02d/%02d/%04d\n",
                       interest,
                       tm.tm_mon + 1,
                       tm.tm_mday,
                       tm.tm_year + 1900);
            }
        }

        // ─── Return to main menu ───
        success(u);
        return;
    }
}

void makeTransaction(struct User u) {
    const char *tmpPath = "./data/records.tmp";
    char name[50];
    struct Record r;
    int accountId, found;
    FILE *in, *out;

    while (1) {
        // ─── Prompt & validate the account number ───
        system("clear");
        printf("\n\t\t=== Make Transaction ===\n");
        printf("Enter the account number for transaction [or press 0 and ENTER to go back]: ");
        if (scanf("%d", &accountId) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');    
            printf("Press Enter to try again [or press 0 and ENTER to go back].");
            getchar();
            continue; 
        }
        while (getchar() != '\n'); 
        if (accountId == 0) {
            mainMenu(u);
            return;
        }

        // ─── Open original & temp files ───
        in  = fopen(RECORDS, "r");
        out = fopen(tmpPath, "w");
        if (!in || !out) {
            perror("Error opening records file");
            exit(1);
        }

        // ─── Scan & modify the matching record ───
        found = 0;
        while (getAccountFromFile(in, name, &r)) {
            if (!found && r.userId == u.id && r.accountNbr == accountId) {
                found = 1;

                // ─── REFUSE TRANSACTIONS ON FIXED ACCOUNTS ───
                if (strcmp(r.accountType, "fixed01") == 0
                 || strcmp(r.accountType, "fixed02") == 0
                 || strcmp(r.accountType, "fixed03") == 0) {
                    printf("\nTransactions are not allowed on %s accounts.\n\n", r.accountType);
                    // clean up temp file
                    fclose(in);
                    fclose(out);
                    remove(tmpPath);
                    printf("Press ENTER to continue.");
                    getchar();
                    success(u);
                    return;
                }

                // ─── SHOW CURRENT BALANCE & CONTINUE ───
                printf("\nCurrent balance for account %d: $%.2f\n",
                       r.accountNbr, r.amount);

                // — Choose deposit or withdrawal —
                int option;
                while (1) {
                    printf("\nSelect transaction type:\n");
                    printf("  [1] Deposit\n");
                    printf("  [2] Withdraw\n");
                    printf("Enter choice (1-2): ");
                    if (scanf("%d", &option) != 1) {
                        printf("Invalid input. Enter 1 or 2.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    while (getchar() != '\n');
                    if (option == 1 || option == 2) break;
                    printf("Invalid choice. Enter 1 or 2.\n");
                }

                // — Prompt for amount —
                double amt;
                char amt_buf[64];
                while (1) {
                    if (option == 1)
                        printf("Enter amount to deposit (positive)[or press 0 and ENTER to go back to Main Menu]: $");
                    else
                        printf("Enter amount to withdraw (positive) [or press 0 and ENTER to go back to Main Menu]: $");

                    if (!fgets(amt_buf, sizeof amt_buf, stdin)) {
                        clearerr(stdin);
                        continue;
                    }
                    amt_buf[strcspn(amt_buf, "\n")] = '\0';

                    while (getchar() != '\n'); 
                    if (accountId == 0) {
                        mainMenu(u);
                    return;
                    }

                    if (!isValidPositiveNumber(amt_buf)) {
                        printf("Invalid amount. Please enter a positive number [or press 0 and ENTER to go back to Main Menu].\n");
                        continue;
                    }
                    amt = strtod(amt_buf, NULL);

                    if (strcmp(amt_buf, "0") == 0) {
                        printf("Transaction canceled.\n");
                        fclose(in);
                        fclose(out);
                        remove(tmpPath);
                        mainMenu(u);
                        return;
                    }

                    if (option == 2 && amt > r.amount) {
                        printf("Insufficient funds. Try a smaller amount.\n");
                        continue;
                    }
                    break;
                }

                // — Apply the transaction —
                if (option == 1) r.amount += amt;
                else             r.amount -= amt;

                printf("\n✔ Transaction applied: new balance $%.2f\n\n", r.amount);
            }

            // — Write (possibly updated) record to temp file —
            fprintf(out,
                "%d %d %s %d %02d/%02d/%04d %s %lld %.2f %s\n",
                r.id, r.userId, name,
                r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
        }

        fclose(in);
        fclose(out);

        // ─── IF NOT FOUND RETRY ───
        if (!found) {
            remove(tmpPath);
            printf("Account %d not found or you don't have access.\n", accountId);
            printf("Press Enter to try again.");
            getchar();
            continue;  // back to step 1
        }

        // ─── SUCCESS! SWAP FILES & RETURN ───
        remove(RECORDS);
        rename(tmpPath, RECORDS);
        success(u);
        return;
    }
}

void removeAccount(struct User u) {
    const char *tmpPath = "./data/records.tmp";
    char name[50];
    struct Record r;
    int accountId, found;
    FILE *in, *out;

    while (1) {
        // ─── Prompt & validate the account number ───
        system("clear");
        printf("\n\t\t=== Remove Account ===\n");
        printf("Enter the account number to remove [or press 0 and ENTER to go back to Main Menu]: ");
        if (scanf("%d", &accountId) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');    
            printf("Press Enter to try again [or press 0 and ENTER to go back to Main Menu].");
            getchar();
            continue;                  
        }
        while (getchar() != '\n');      
        if (accountId == 0) {
            mainMenu(u);
            return;
        }

        // ─── Open original & temp files ───
        in  = fopen(RECORDS, "r");
        out = fopen(tmpPath, "w");
        if (!in || !out) {
            perror("Error opening records file");
            exit(1);
        }

        // ─── Scan & skip the matching record ───
        found = 0;
        while (getAccountFromFile(in, name, &r)) {
            if (!found && r.userId == u.id && r.accountNbr == accountId) {
                // mark as removed and skip writing it
                found = 1;
                continue;
            }
            // write all other records
            fprintf(out,
                "%d %d %s %d %02d/%02d/%04d %s %lld %.2f %s\n",
                r.id, r.userId, name,
                r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
        }

        fclose(in);
        fclose(out);

        // ─── Handle “not found” vs. removal ───
        if (!found) {
            remove(tmpPath);
            printf("\nAccount %d not found or you don't have access.\n", accountId);
            printf("Press Enter to try again.");
            getchar();
            continue; 
        }

        // ─── Success: swap files & return ───
        remove(RECORDS);
        rename(tmpPath, RECORDS);
        printf("\n✔ Account %d removed successfully!\n\n", accountId);
        success(u);
        return;
    }
}

void transferOwnership(struct User u) {
    const char *tmpPath = "./data/records.tmp";
    char name[50];
    struct Record r;
    int accountId, found;
    FILE *in, *out;

    while (1) {
        // ─── Prompt & validate account number ───
        system("clear");
        printf("\n\t\t=== Transfer Account Ownership ===\n");
        printf("Enter the account number to transfer (or 0 to go back): ");
        if (scanf("%d", &accountId) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            printf("Press Enter to try again.");
            getchar();
            continue;
        }
        while (getchar() != '\n');
        if (accountId == 0) {
            mainMenu(u);
            return;
        }

        // ─── Open records & temp files ───
        in  = fopen(RECORDS, "r");
        out = fopen(tmpPath, "w");
        if (!in || !out) {
            perror("Error opening records file");
            exit(1);
        }

        // ─── Scan & update the matching record ───
        found = 0;
        while (getAccountFromFile(in, name, &r)) {
            if (!found && r.userId == u.id && r.accountNbr == accountId) {
                found = 1;

                // Show current owner info
                printf("\nCurrent owner of account %d is user ID %d (%s)\n\n",
                       r.accountNbr, r.userId, name);

                // Prompt for new owner’s username
                char newOwnerName[50];
                int newOwnerId;
                while (1) {
                    printf("Enter the new owner’s username [or press 0 and ENTER to go back to Main Menu]: ");
                    if (!fgets(newOwnerName, sizeof newOwnerName, stdin)) {
                        clearerr(stdin);
                        continue;
                    }
                    newOwnerName[strcspn(newOwnerName, "\n")] = '\0';
                    if (newOwnerName[0] == '\0') {
                        printf("Please enter a username.\n");
                        continue;
                    }

                    if (strcmp(newOwnerName, "0") == 0) {
                        fclose(in);
                        fclose(out);
                        remove(tmpPath);
                        mainMenu(u);
                        return;
                    }

                    // Check that username is in USERS
                    FILE *uf = fopen(USERS, "r");
                    if (!uf) {
                        perror("Error opening users file");
                        exit(1);
                    }
                    struct User tu;
                    int foundUser = 0;
                    while (fscanf(uf, "%d %49s %49s",
                                  &tu.id, tu.name, tu.password) == 3) {
                        if (strcmp(tu.name, newOwnerName) == 0) {
                            newOwnerId = tu.id;
                            foundUser = 1;
                            break;
                        }
                    }
                    fclose(uf);

                    if (!foundUser) {
                        printf("Username \"%s\" not found. Please try again.\n", newOwnerName);
                        continue;
                    }
                    break;
                }

                // **Apply both changes**:
                r.userId = newOwnerId;
                strcpy(name, newOwnerName);

                // ─── 3a) Avoid account-number conflict ───
                {
                    // First pass: detect any existing accountNbr conflict
                    FILE *chk = fopen(RECORDS, "r");
                    char tmpName2[50];
                    struct Record r2;
                    int conflict = 0;
                    while (getAccountFromFile(chk, tmpName2, &r2)) {
                        if (r2.userId == r.userId && r2.accountNbr == r.accountNbr) {
                            conflict = 1;
                            break;
                        }
                    }
                    fclose(chk);

                    if (conflict) {
                        // Second pass: find the highest accountNbr for new owner
                        chk = fopen(RECORDS, "r");
                        int maxAcc = 0;
                        while (getAccountFromFile(chk, tmpName2, &r2)) {
                            if (r2.userId == r.userId && r2.accountNbr > maxAcc) {
                                maxAcc = r2.accountNbr;
                            }
                        }
                        fclose(chk);

                        // assign a new, non‐conflicting account number
                        r.accountNbr = maxAcc + 1;
                        printf("\nAssigned new account number %d to avoid conflict.\n\n",
                               r.accountNbr);
                    }
                }

                printf("\nOwnership will be transferred to user %s.\n\n", name);
            }

            // write this (possibly updated) record
            fprintf(out,
                "%d %d %s %d %02d/%02d/%04d %s %lld %.2f %s\n",
                r.id,
                r.userId,
                name,
                r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
        }

        fclose(in);
        fclose(out);

        // ─── Not found? retry ───
        if (!found) {
            remove(tmpPath);
            printf("\nAccount %d not found or you don't have access.\n", accountId);
            printf("Press Enter to try again.");
            getchar();
            continue;
        }

        // ─── Success: swap files & return ───
        remove(RECORDS);
        rename(tmpPath, RECORDS);
        printf("\nOwnership transferred successfully!\n\n");
        success(u);
        return;
    }
}