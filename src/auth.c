#include <termios.h>
#include "header.h"

extern const char *USERS; // External reference to the file path for user data

void loginMenu(char username[50], char password[50])
{
    // Structures to hold terminal settings
    struct termios originalFlags, newFlags;

    // Clear the screen and display login prompt
    system("clear");
    printf("\n\n\n\t\t\t\t\tBank Management System\n\t\t\t\t\t User Login:");

    // Prompt and read the username
    scanf("%s", username);

    // Get current terminal settings (to restore later)
    tcgetattr(fileno(stdin), &originalFlags); 
    newFlags = originalFlags;
    // Disable echo so password input is hidden
    newFlags.c_lflag &= ~ECHO; 
    // Still print a newline when press ENTER
    newFlags.c_lflag |= ECHONL; 

    // Apply new terminal settings
    if (tcsetattr(fileno(stdin), TCSANOW, &newFlags) != 0)
    {
        // Print error if terminal settings can't be changed
        perror("tcsetattr");
        exit(1);
    }

    // Prompt the password (will be hidden)
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", password);

    // Restore the original terminal settings
    if (tcsetattr(fileno(stdin), TCSANOW, &originalFlags) != 0)
    {
        perror("tcsetattr"); // Print error if terminal settings can't be restored
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
        // Show error if file couldn't be opened
        perror("Error opening file."); 
        exit(1);
    }

    // Read each user record from the file
    while (fscanf(file, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) == 3)
    {
        // Remove any newline characters from the password
        tempUser.password[strcspn(tempUser.password, "\n")] = '\0';

        // Check if the username matches the input
        if (strcmp(tempUser.name, user->name) == 0)
        {
            // Copy the matched password to the static buffer
            strcpy(passwordBuffer, tempUser.password);

            // Assign the user's ID to the passed-in user structure
            user->id = tempUser.id;  
            fclose(file);
            return passwordBuffer;  // Return the found password
        }
    }

    fclose(file); // Close file if user not found
    return "No user found.";  // Return message if username doesn't exist
}




