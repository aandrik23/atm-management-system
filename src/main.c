#include "header.h"
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Helper function to prompt only for the password (without echoing input)
static void promptPassword(char password[50]) {
    struct termios originalFlags, newFlags;
    // Save current terminal settings
    tcgetattr(fileno(stdin), &originalFlags);
    newFlags = originalFlags;
    // Disable echoing input characters (password will be hiden)
    newFlags.c_lflag &= ~ECHO;
    // Ensure newline is still echoed when user presses ENTER    
    newFlags.c_lflag |= ECHONL; 
    // Apply the modified terminal settings 
    tcsetattr(fileno(stdin), TCSANOW, &newFlags);

    // Promt for password input
    printf("\n\n\t\tEnter your password: ");
    scanf("%49s", password);
    // Restore original terminal settings
    tcsetattr(fileno(stdin), TCSANOW, &originalFlags);
    // Clear any extra input left in buffer
    while (getchar() != '\n'); 
}

// Login/register/exit menu
void initMenu(struct User *user)
{
    char buf[64];
    char namebuf[50];
    char *end;
    long choice;
    int loggedIn = 0;

    while (!loggedIn) {
        // Clear the screen and display menu
        system("clear");
        printf("\n\n\t\t======= ATM =======\n");
        printf("\n\t\t-->> Feel free to login / register :\n\n");
        printf("\t\t[1] - Login\n");
        printf("\t\t[2] - Register\n");
        printf("\t\t[3] - Exit\n\n");

        // Prompt for user menu selection (1-3)
        while (1) {
            printf("\t\tEnter your choice: ");
            if (!fgets(buf, sizeof(buf), stdin)) {
                clearerr(stdin);
                continue;
            }
            // Remove newline character from input
            buf[strcspn(buf, "\n")] = '\0';
            // Convert input to integer
            choice = strtol(buf, &end, 10);
            // Validate that input is a number within 1–3
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
            // Username input loop
            while (1) {
                printf("\n\t\t\tUsername (or press 0 and ENTER to go back): ");
                if (!fgets(namebuf, sizeof(namebuf), stdin)) {
                    clearerr(stdin);
                    continue;
                }
                namebuf[strcspn(namebuf, "\n")] = '\0';

                // Go back to main menu if user press 0
                if (strcmp(namebuf, "0") == 0) {
                    back = 1;
                    break;
                }

                // Copy entered username to user struct
                strcpy(user->name, namebuf);

                // Check if username exists
                const char *realpass = getPassword(user);
                if (strcmp(realpass, "no user found") == 0) {
                    printf("Username \"%s\" does not exist. Please try again.\n", user->name);
                    continue;  // Try again
                }
                break;  // Valid username
            }
            if (back) {
                continue;  // Return to menu
            }

            // Password input loop
            while (1) {
                promptPassword(user->password);

                // Allow user to cancel password input by entering 0
                if (strcmp(user->password, "0") == 0) {
                    back = 1;
                    break;
                }

                const char *realpass = getPassword(user);
                // Validate password
                if (strcmp(user->password, realpass) == 0) {
                    printf("\nPassword match!\n");
                    loggedIn = 1;
                    break;
                } else {
                    printf("\nWrong username or password. Please try again.\n");
                }
            }
            if (back) {
                continue; // Return to menu
            }
        }
        else if (choice == 2) {
            // ——— REGISTER FLOW ———
            if (registerUser(user)) {
                loggedIn = 1;
            }
        }
        else {
            // Exit from the program
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
        // Clear screen and show available actions
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
        
        // Loop until valid input between 1–8 is entered
        while (1) {
            if (!fgets(buf, sizeof(buf), stdin)) {
                clearerr(stdin);
                continue;
            }
            buf[strcspn(buf, "\n")] = '\0';
            
            // Ignore empty input
            if (buf[0] == '\0') {
                continue;
            }
            
            // Convert input to integer
            choice = strtol(buf, &end, 10);
            
            // Check for valid number
            if (end == buf || *end != '\0') {
                continue;
            }
            
            // Ensure number is in range 1–8
            if (choice < 1 || choice > 8) {
                printf("Invalid choice. Please enter a number between 1 and 8.\n");
                continue;
            }

            break;
        }

        // Perform the selected operation
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
    // Start login/register process
    initMenu(&user);

    // Once authenticated, show the main ATM menu
    mainMenu(user);

    return 0;
}


