#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TRANSACTIONS 10000  // Number of transactions
#define NUM_MEMBERS 1000  // Number of unique members
#define MAX_ITEMS_PER_TRANSACTION 5  // Maximum number of items per transaction
#define NUM_ITEMS 20  // Number of different items available in the store

const char *items[] = {
    "Bread", "Milk", "Butter", "Eggs", "Cheese", "Diaper", 
    "Beer", "Cola", "Chips", "Cereal", "Apples", "Bananas", 
    "Chicken", "Rice", "Pasta", "Tomatoes", "Lettuce", 
    "Juice", "Water", "Yogurt"
};

void generate_transaction_data(FILE *file) {
    for (int transaction_id = 1; transaction_id <= NUM_TRANSACTIONS; transaction_id++) {
        int member_id = rand() % NUM_MEMBERS + 1;
        int num_items = rand() % MAX_ITEMS_PER_TRANSACTION + 1;
        
        int selected_items[MAX_ITEMS_PER_TRANSACTION];
        for (int i = 0; i < num_items; i++) {
            int item_index;
            int is_duplicate;
            do {
                is_duplicate = 0;
                item_index = rand() % NUM_ITEMS;
                for (int j = 0; j < i; j++) {
                    if (selected_items[j] == item_index) {
                        is_duplicate = 1;
                        break;
                    }
                }
            } while (is_duplicate);

            selected_items[i] = item_index;
            fprintf(file, "%d,%d,%s\n", transaction_id, member_id, items[item_index]);
        }
    }
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // File to save the dataset
    FILE *file = fopen("grocery_store_dataset.csv", "w");
    if (!file) {
        printf("Error opening file!\n");
        return 1;
    }

    // Write the CSV header
    fprintf(file, "TransactionID,MemberID,ItemDescription\n");

    // Generate the transaction data
    generate_transaction_data(file);

    // Close the file
    fclose(file);

    printf("Dataset generated and saved to grocery_store_dataset.csv\n");
    return 0;
}