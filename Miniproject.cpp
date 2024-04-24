#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define MAX_ITEMS 100
#define SECURITY_CODE 00000
#define CURRENCY_SYMBOL "₹" // Define constant for currency symbol

// Color codes for text formatting
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct Item {
    int id;
    char seller_name[50];
    char name[50];
    char category[50]; 
    float price;
};

struct Item items[MAX_ITEMS];
int itemCount = 0;

// Function prototypes
int getChoice();
void addItem();
void viewItems(int calledFromMainMenu);
void buyItem();
void updateProducts();
void enterSecurityCode();
void mainMenu();
void saveData();
void loadData();
void deleteItem();
void AboutUs();
int exitFunction(); // Change return type to int

int main() {
    printf(ANSI_COLOR_MAGENTA "Rent and Sell Management System\n" ANSI_COLOR_RESET);
    enterSecurityCode();
    AboutUs(); // Call the About Us function after successful security code entry
    loadData(); // Load data from file

    int choice;
    int shouldExit; // Variable to store the return value of exitFunction()

    do {
        mainMenu();
        choice = getChoice();

        switch(choice) {
            case 1:
                addItem();
                break;
            case 2:
                viewItems(1);
                break;
            case 3:
                buyItem();
                break;
            case 4:
                updateProducts();
                break;
            case 5:
                deleteItem();
                break;
            case 6:
                AboutUs(); // Call the About Us function
                break;
            case 7:
                shouldExit = exitFunction(); // Store the return value
                break;
            default:
                printf(ANSI_COLOR_RED "Invalid choice. Please try again.\n" ANSI_COLOR_RESET);
        }
    } while (choice != 7 && shouldExit != 1); // Check both conditions

    return 0;
}

void saveData() {
    FILE *file = fopen("items_data.txt", "w");
    if (file == NULL) {
        printf(ANSI_COLOR_RED "Error opening file for writing.\n" ANSI_COLOR_RESET);
        return;
    }

    // Write data to file
    for (int i = 0; i < itemCount; i++) {
        if (fprintf(file, "%d %s %s %.2f\n", items[i].id, items[i].seller_name, items[i].name, items[i].price) < 0) {
            printf(ANSI_COLOR_RED "Error writing to file.\n" ANSI_COLOR_RESET);
            fclose(file); // Close the file before returning
            return;
        }
    }

    // Close the file after writing
    fclose(file);
    printf(ANSI_COLOR_GREEN "Data saved successfully.\n" ANSI_COLOR_RESET);
}

void loadData() {
    FILE *file = fopen("items_data.txt", "r");
    if (file == NULL) {
        printf(ANSI_COLOR_YELLOW "No previous data found.\n" ANSI_COLOR_RESET);
        return;
    }

    // Read data from file
    while (fscanf(file, "%d %s %s %f", &items[itemCount].id, items[itemCount].seller_name, items[itemCount].name, &items[itemCount].price) != EOF) {
        itemCount++;
    }

    // Check for errors during reading
    if (ferror(file)) {
        printf(ANSI_COLOR_RED "Error reading from file.\n" ANSI_COLOR_RESET);
        fclose(file); // Close the file before returning
        return;
    }

    // Close the file after reading
    fclose(file);
    printf(ANSI_COLOR_GREEN "Data loaded successfully.\n" ANSI_COLOR_RESET);
}

void addItem() {
    if (itemCount >= MAX_ITEMS) {
        printf(ANSI_COLOR_RED "Maximum items reached. Cannot add more items.\n" ANSI_COLOR_RESET);
        return;
    }

    struct Item newItem;

    printf(ANSI_COLOR_CYAN "Enter item details:\n" ANSI_COLOR_RESET);
    
    // Prompt user for ID until a unique one is entered
    int id;
    int idUsed = 0;
    do {
        idUsed = 0; // Reset idUsed to 0 for each iteration
        printf("ID: ");
        if (scanf("%d", &id) != 1) {
            printf(ANSI_COLOR_RED "Invalid input. Please enter a valid integer ID.\n" ANSI_COLOR_RESET);
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        // Check if the ID is already used
        for (int i = 0; i < itemCount; i++) {
            if (items[i].id == id) {
                idUsed = 1;
                break;
            }
        }

        if (idUsed) {
            printf(ANSI_COLOR_RED "ID %d is already used. Please enter a different ID.\n" ANSI_COLOR_RESET, id);
        }
    } while (idUsed);

    // Assign the unique ID to the new item
    newItem.id = id;

    // Clear input buffer before reading seller name
    while (getchar() != '\n'); // Clear input buffer

    // Input seller name
    printf("Seller Name: ");
    fflush(stdout); // Flush stdout before input
    fgets(newItem.seller_name, sizeof(newItem.seller_name), stdin);
    newItem.seller_name[strcspn(newItem.seller_name, "\n")] = '\0'; // Remove trailing newline if present

    // Input product name
    printf("Product Name: ");
    fflush(stdout); // Flush stdout before input
    fgets(newItem.name, sizeof(newItem.name), stdin);
    newItem.name[strcspn(newItem.name, "\n")] = '\0'; // Remove trailing newline if present

    // Input category
    printf("Category: ");
    fflush(stdout); // Flush stdout before input
    fgets(newItem.category, sizeof(newItem.category), stdin);
    newItem.category[strcspn(newItem.category, "\n")] = '\0'; // Remove trailing newline if present

    // Input price
    printf("Price: %s", CURRENCY_SYMBOL); // Use constant for currency symbol
    if (scanf("%f", &newItem.price) != 1) {
        printf(ANSI_COLOR_RED "Invalid input. Please enter a valid floating-point price.\n" ANSI_COLOR_RESET);
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    items[itemCount] = newItem;
    itemCount++;

    printf(ANSI_COLOR_GREEN "Item added successfully!\n" ANSI_COLOR_RESET);
    saveData(); // Save data after adding new item
}

void viewItems(int calledFromMainMenu) {
    if (itemCount == 0) {
        printf(ANSI_COLOR_YELLOW "No items available.\n" ANSI_COLOR_RESET);
        return;
    }

    printf("\n" ANSI_COLOR_CYAN "========================= Items List =========================\n" ANSI_COLOR_RESET);
    printf("| %-5s | %-20s | %-20s | %-10s | %-10s |\n", "ID", "Seller Name", "Product Name", "Price", "Category");
    printf("|-------|----------------------|----------------------|------------|----------------|\n");

    // Clear input buffer before prompting for category input
    while (getchar() != '\n'); // Clear input buffer

    // Prompt user for category input
    char selectedCategory[50];
    printf("Enter the category you want to view: ");
    fflush(stdout); // Flush stdout before input
    fgets(selectedCategory, sizeof(selectedCategory), stdin);
    selectedCategory[strcspn(selectedCategory, "\n")] = '\0'; 

    for (int i = 0; i < itemCount; i++) {
        if (strcmp(items[i].category, selectedCategory) == 0) {
            printf("| %-5d | %-20s | %-20s | %-7.2f | %-15s |\n", items[i].id, items[i].seller_name, items[i].name, items[i].price, items[i].category);
        }
    }

    printf(ANSI_COLOR_CYAN "===============================================================\n" ANSI_COLOR_RESET);
}

void buyItem() {
    // Prompt the user to enter the category to filter the items
    printf("\n" ANSI_COLOR_CYAN "Enter the category you want to view: " ANSI_COLOR_RESET);
    fflush(stdout); // Flush stdout before input
    char selectedCategory[50];
    fgets(selectedCategory, sizeof(selectedCategory), stdin);
    selectedCategory[strcspn(selectedCategory, "\n")] = '\0'; 

    // Clear input buffer
    while (getchar() != '\n'); // Clear input buffer

    // Display items based on the selected category
    printf("\n" ANSI_COLOR_CYAN "========================= Items List =========================\n" ANSI_COLOR_RESET);
    printf("| %-5s | %-20s | %-20s | %-10s | %-10s |\n", "ID", "Seller Name", "Product Name", "Price", "Category");
    printf("|-------|----------------------|----------------------|------------|----------------|\n");
    for (int i = 0; i < itemCount; i++) {
        if (strcmp(items[i].category, selectedCategory) == 0) {
            printf("| %-5d | %-20s | %-20s | %-7.2f | %-15s |\n", items[i].id, items[i].seller_name, items[i].name, items[i].price, items[i].category);
        }
    }
    printf(ANSI_COLOR_CYAN "===============================================================\n" ANSI_COLOR_RESET);

    // Prompt the user to enter the ID of the item they want to buy
    int id;
    printf("\nEnter the ID of the item you want to buy: ");
    scanf("%d", &id);

    // Search for the item with the given ID
    int index = -1;
    for (int i = 0; i < itemCount; i++) {
        if (items[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Item with ID %d not found.\n", id);
    } else {
        printf("Item purchased successfully:\n");
        printf("ID: %d, Name: %s, Price: %.2f\n", items[index].id, items[index].name, items[index].price);

        // Prompt for confirmation
        char confirmation;
        printf("Do you want to confirm your purchase? (y/n): ");
        scanf(" %c", &confirmation);

        if (confirmation == 'y' || confirmation == 'Y') {
            // Remove the purchased item from the items list
            for (int i = index; i < itemCount - 1; i++) {
                items[i] = items[i + 1];
            }
            itemCount--;

            printf("Thank you for your purchase!\n");
            saveData(); // Save data after purchasing item
        } else {
            printf("Purchase canceled.\n");
        }
    }
}

void updateProducts() {
    int id;
    printf("Enter the ID of the product you want to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input. Please enter a valid integer ID.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    // Search for the product with the given ID
    int index = -1;
    for (int i = 0; i < itemCount; i++) {
        if (items[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Product with ID %d not found.\n", id);
    } else {
        printf("Enter the new details for the product:\n");
        printf("Name: ");
        scanf("%s", items[index].name);
        printf("Price: %s", CURRENCY_SYMBOL); // Use constant for currency symbol
        scanf("%f", &items[index].price);

        printf("Product updated successfully:\n");
        printf("ID: %d, Name: %s, Price: %.2f\n", items[index].id, items[index].name, items[index].price);
        saveData(); // Save data after updating product
    }
}

void enterSecurityCode() {
    int code;
    printf(ANSI_COLOR_CYAN "Enter the 5-digit security code: " ANSI_COLOR_RESET);

    // Keep prompting the user until a valid numeric code is entered
    while (scanf("%d", &code) != 1 || code < 0 || code > 99999) {
        printf(ANSI_COLOR_RED "Invalid input. Please enter a 5-digit numeric code.\n" ANSI_COLOR_RESET);

        // Clear the input buffer
        while (getchar() != '\n');
        
        printf(ANSI_COLOR_CYAN "Enter the 5-digit security code: " ANSI_COLOR_RESET);
    }

    // Clear the newline character from the input buffer
    getchar();

    if (code != SECURITY_CODE) {
        printf(ANSI_COLOR_RED "Incorrect security code. Access denied.\n" ANSI_COLOR_RESET);
        exit(1);
    }
}

void deleteItem() {
    printf("Enter the ID(s) of the item(s) you want to delete (separated by spaces or commas), or type 'all' to delete all items: ");
    char input[100]; // Assuming maximum length of input is 100 characters
    scanf("%s", input);

    if (strcmp(input, "all") == 0) {
        // Confirmation prompt before deleting all items
        char confirm;
        printf("Are you sure you want to delete all items? (y/n): ");
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y') {
            printf("All items deleted successfully.\n");
            itemCount = 0; // Reset item count to 0
        } else {
            printf("Deletion of all items canceled.\n");
        }
    } else {
        char *token = strtok(input, ", "); // Split input by comma or space
        while (token != NULL) {
            int id = atoi(token); // Convert token to integer
            int index = -1;

            // Search for the item with the given ID
            for (int i = 0; i < itemCount; i++) {
                if (items[i].id == id) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                printf("Item with ID %d not found.\n", id);
            } else {
                // Confirmation prompt before deleting
                char confirm;
                printf("Are you sure you want to delete item ID %d? (y/n): ", id);
                scanf(" %c", &confirm);

                if (confirm == 'y' || confirm == 'Y') {
                    printf("Item deleted successfully:\n");
                    printf("ID: %d, Name: %s, Price: %.2f\n", items[index].id, items[index].name, items[index].price);

                    // Remove the item from the items list
                    for (int i = index; i < itemCount - 1; i++) {
                        items[i] = items[i + 1];
                    }
                    itemCount--;
                } else {
                    printf("Deletion canceled for item ID %d.\n", id);
                }
            }

            token = strtok(NULL, ", "); // Move to next token
        }
    }

    saveData(); // Save data after deleting items
}

void AboutUs() {
    printf("\n" ANSI_COLOR_YELLOW "===== About Us =====\n" ANSI_COLOR_RESET);
    printf("Welcome to our online marketplace!\n");
    printf("This program allows you to manage your inventory, buy and sell products.\n");
    printf("Here's a brief overview of how to use this program:\n\n");
    
    printf(ANSI_COLOR_GREEN "1. Adding Items:\n" ANSI_COLOR_RESET);
    printf("   - Choose option 1 from the main menu to add items to your inventory.\n");
    printf("   - Enter details such as ID, seller name, product name, category, and price.\n\n");
    
    printf(ANSI_COLOR_GREEN "2. Viewing Items:\n" ANSI_COLOR_RESET);
    printf("   - Choose option 2 from the main menu to view items.\n");
    printf("   - You can filter items by category to see specific types of products.\n\n");

    printf(ANSI_COLOR_GREEN "3. Buying Items:\n" ANSI_COLOR_RESET);
    printf("   - Choose option 3 from the main menu to buy items.\n");
    printf("   - Enter the ID of the item you want to buy and follow the prompts.\n\n");

    printf(ANSI_COLOR_GREEN "4. Updating Products:\n" ANSI_COLOR_RESET);
    printf("   - Choose option 4 from the main menu to update product details.\n");
    printf("   - Enter the ID of the product you want to update and provide new details.\n\n");

    printf(ANSI_COLOR_GREEN "5. Deleting Items:\n" ANSI_COLOR_RESET);
    printf("   - Choose option 5 from the main menu to delete items.\n");
    printf("   - Enter the ID(s) of the item(s) you want to delete and confirm.\n\n");

    printf(ANSI_COLOR_GREEN "6. Exiting the Program:\n" ANSI_COLOR_RESET);
    printf("   - Choose option 6 from the main menu to exit the program.\n");
    printf("   - You will be prompted to confirm before exiting.\n");

    printf(ANSI_COLOR_YELLOW "=====================\n" ANSI_COLOR_RESET);
}

int exitFunction() {
    char confirm;
    do {
        printf(ANSI_COLOR_CYAN "Are you sure you want to exit? (y/n): " ANSI_COLOR_RESET);
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y') {
            printf(ANSI_COLOR_YELLOW "Exiting...\n" ANSI_COLOR_RESET);
            for (int i = 0; i < 10; i++) {
                printf(ANSI_COLOR_RED "exit " ANSI_COLOR_GREEN "exit " ANSI_COLOR_YELLOW "exit " ANSI_COLOR_BLUE "exit " ANSI_COLOR_MAGENTA "exit " ANSI_COLOR_CYAN "exit " ANSI_COLOR_RESET);
                Sleep(500); // Sleep for 0.5 seconds (500 milliseconds)
            }
            exit(0);
        } else if (confirm == 'n' || confirm == 'N') {
            printf(ANSI_COLOR_CYAN "Exiting canceled.\n" ANSI_COLOR_RESET);
            return 0; // Return 0 indicating not to exit
        } else {
            printf(ANSI_COLOR_RED "Invalid input. Please enter 'y' or 'n'.\n" ANSI_COLOR_RESET);
        }
    } while (1); // Loop indefinitely until a valid input is provided
}

void mainMenu() {
    printf(ANSI_COLOR_YELLOW "======================\n" ANSI_COLOR_RESET);
    printf("\n" ANSI_COLOR_YELLOW "===== Main Menu =====\n" ANSI_COLOR_RESET);
    printf("     1. Add items\n");
    printf("     2. View items     \n");
    printf("     3. Buy items\n");
    printf("     4. Update your products\n");
    printf("     5. Delete items\n");
    printf("     6. About Us\n"); // Add an option for About Us
    printf("     7. Exit\n"); // Update the exit option number
    printf(ANSI_COLOR_YELLOW "======================\n" ANSI_COLOR_RESET);
}

int getChoice() {
    int choice;
    printf(ANSI_COLOR_CYAN "Enter your choice: " ANSI_COLOR_RESET);    
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 7) {
        printf(ANSI_COLOR_RED "Invalid input. Please enter a valid choice (1-7): " ANSI_COLOR_RESET);
        while (getchar() != '\n'); // Clear input buffer
    }
    return choice;
}
