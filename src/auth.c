#include <termios.h>
#include "header.h"

extern const char *USERS;

void loginMenu(char username[50], char password[50])
{
    struct termios originalFlags, newFlags;

    // Clear terminal and prompt for user login
    system("clear");
    printf("\n\n\n\t\t\t\t\tBank Management System\n\t\t\t\t\t User Login:");

    // Get the username input
    scanf("%s", username);

    // Disable terminal echo for password entry
    tcgetattr(fileno(stdin), &originalFlags);  // Get current terminal settings
    newFlags = originalFlags;
    newFlags.c_lflag &= ~ECHO;  // Turn off echo
    newFlags.c_lflag |= ECHONL; // Turn on newline echo

    if (tcsetattr(fileno(stdin), TCSANOW, &newFlags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }

    // Prompt for password input without echo
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", password);

    // Restore the original terminal settings
    if (tcsetattr(fileno(stdin), TCSANOW, &originalFlags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
}

const char *getPassword(struct User *user)
{
    FILE *file;
    struct User tempUser;
    static char passwordBuffer[50];  // Static buffer to store the password temporarily

    // Open the users file
    file = fopen(USERS, "r");
    if (!file)
    {
        perror("Error opening file.");
        exit(1);
    }

    // Search for the user and retrieve password
    while (fscanf(file, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) == 3)
    {
        // Clean up any newline character from password input
        tempUser.password[strcspn(tempUser.password, "\n")] = '\0';

        if (strcmp(tempUser.name, user->name) == 0)
        {
            strcpy(passwordBuffer, tempUser.password);  // Copy the password to buffer
            user->id = tempUser.id;  // Assign the user ID to the passed User struct
            fclose(file);
            return passwordBuffer;  // Return the found password
        }
    }

    fclose(file);
    return "No user found.";  // Return if no matching user was found
}




//EXW FTIAXEI TA "0 TO GO BACK" STO LOGIN KAI SO REGISTER!!!