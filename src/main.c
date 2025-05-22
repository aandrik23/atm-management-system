#include "header.h"
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Helper function to prompt only for the password (no username), unchanged
static void promptPassword(char password[50]) {
    struct termios originalFlags, newFlags;
    tcgetattr(fileno(stdin), &originalFlags);
    newFlags = originalFlags;
    newFlags.c_lflag &= ~ECHO;    // Disable echo
    newFlags.c_lflag |= ECHONL;   // Echo newline after enter
    tcsetattr(fileno(stdin), TCSANOW, &newFlags);

    printf("\n\n\t\tEnter your password: ");
    scanf("%49s", password);
    tcsetattr(fileno(stdin), TCSANOW, &originalFlags);
    while (getchar() != '\n');  // Clear leftover newline
}

// Main menu initialization
void initMenu(struct User *user)
{
    char buf[64];
    char namebuf[50];
    char *end;
    long choice;
    int loggedIn = 0;

    while (!loggedIn) {
        // Display top-level menu
        system("clear");
        printf("\n\n\t\t======= ATM =======\n");
        printf("\n\t\t-->> Feel free to login / register :\n\n");
        printf("\t\t[1] - Login\n");
        printf("\t\t[2] - Register\n");
        printf("\t\t[3] - Exit\n\n");

        // Prompt for a choice (1–3)
        while (1) {
            printf("\t\tEnter your choice: ");
            if (!fgets(buf, sizeof(buf), stdin)) {
                clearerr(stdin);
                continue;
            }
            buf[strcspn(buf, "\n")] = '\0';
            choice = strtol(buf, &end, 10);
            if (end == buf || *end != '\0' || choice < 1 || choice > 3) {
                printf("Invalid choice. Please press 1, 2, or 3.\n");
                continue;
            }
            break;
        }

        if (choice == 1) {
            // ——— LOGIN FLOW ———
            system("clear");
            printf("\n\n\n\t\t\tBank Management System\n");

            int back = 0;
            // Username loop
            while (1) {
                printf("\n\t\t\tUsername (or press 0 and ENTER to go back): ");
                if (!fgets(namebuf, sizeof(namebuf), stdin)) {
                    clearerr(stdin);
                    continue;
                }
                namebuf[strcspn(namebuf, "\n")] = '\0';

                if (strcmp(namebuf, "0") == 0) {
                    back = 1;
                    break;
                }

                // Set the username
                strcpy(user->name, namebuf);

                const char *realpass = getPassword(user);
                if (strcmp(realpass, "no user found") == 0) {
                    printf("Username \"%s\" does not exist. Please try again.\n", user->name);
                    continue;  // Retry username
                }
                break;  // Valid username
            }
            if (back) {
                continue;  // Go back to menu
            }

            // Password loop
            while (1) {
                promptPassword(user->password);

                if (strcmp(user->password, "0") == 0) {
                    // Go back to username prompt
                    back = 1;
                    break;
                }

                const char *realpass = getPassword(user);
                if (strcmp(user->password, realpass) == 0) {
                    printf("\nPassword match!\n");
                    loggedIn = 1;
                    break;
                } else {
                    printf("\nWrong username or password. Please try again.\n");
                }
            }
            if (back) {
                continue; // Go back to top menu
            }
        }
        else if (choice == 2) {
            // ——— REGISTER FLOW ———
            if (registerUser(user)) {
                loggedIn = 1;
            }
        }
        else {
            // Exit
            exit(0);
        }
    }
}

// Main menu after successful login
void mainMenu(struct User user)
{
    char buf[64];
    long choice;
    char *end;

    while (1) {
        system("clear");
        printf("\n\n\t\t======= ATM =======\n\n");
        printf("\t\t-->> Feel free to choose one of the options below <<--\n\n");
        printf("\t\t[1] - Create a new account\n");
        printf("\t\t[2] - Update account information\n");
        printf("\t\t[3] - Check account details\n");
        printf("\t\t[4] - List owned accounts\n");
        printf("\t\t[5] - Make Transaction\n");
        printf("\t\t[6] - Remove existing account\n");
        printf("\t\t[7] - Transfer ownership\n");
        printf("\t\t[8] - Exit\n\n");

        printf("\t\tEnter your choice: ");
        // Keep asking until we get a valid choice 1–8
        while (1) {
            if (!fgets(buf, sizeof(buf), stdin)) {
                clearerr(stdin);
                continue;
            }
            buf[strcspn(buf, "\n")] = '\0';
            
            // 1) Blank line? Silently retry
            if (buf[0] == '\0') {
                continue;
            }
            
            choice = strtol(buf, &end, 10);
            
            // 2) Non-numeric? Silently retry
            if (end == buf || *end != '\0') {
                continue;
            }
            
            // 3) Numeric but out of range? Show error
            if (choice < 1 || choice > 8) {
                printf("Invalid choice. Please enter a number between 1 and 8.\n");
                continue;
            }

            // 4) Good choice: break out
            break;
        }

        switch (choice) {
            case 1: createNewAccount(user);        break;
            case 2: updateAccountInfo(user);   break;
            case 3: viewAccountDetails(user);  break;
            case 4: checkAllAccounts(user);    break;
            case 5: makeTransaction(user);     break;
            case 6: removeAccount(user);       break;
            case 7: transferOwnership(user);   break;
            case 8: exit(0);                   return;
        }
    }
}

int main()
{
    struct User user;
    initMenu(&user);  // Initialize menu and login/register flow
    mainMenu(user);   // Proceed to the main menu after login
    return 0;
}


