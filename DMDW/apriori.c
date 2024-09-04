#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_TRANSACTIONS 10000  // Number of transactions
#define NUM_MEMBERS 1000  // Number of unique members
#define MAX_ITEMS_PER_TRANSACTION 5  // Maximum number of items per transaction
#define NUM_ITEMS 20  // Number of different items available in the store
#define MIN_SUPPORT 50  // Minimum support count for an itemset to be considered frequent

const char *items[] = {
    "Bread", "Milk", "Butter", "Eggs", "Cheese", "Diaper", 
    "Beer", "Cola", "Chips", "Cereal", "Apples", "Bananas", 
    "Chicken", "Rice", "Pasta", "Tomatoes", "Lettuce", 
    "Juice", "Water", "Yogurt"
};

// Structure to hold a transaction
typedef struct {
    int transaction_id;
    int member_id;
    int items[MAX_ITEMS_PER_TRANSACTION];
    int num_items;
} Transaction;

// Function to generate transaction data
void generate_transaction_data(FILE *file, Transaction transactions[]) {
    for (int transaction_id = 1; transaction_id <= NUM_TRANSACTIONS; transaction_id++) {
        int member_id = rand() % NUM_MEMBERS + 1;
        int num_items = rand() % MAX_ITEMS_PER_TRANSACTION + 1;

        Transaction transaction = {transaction_id, member_id, {0}, num_items};

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
            transaction.items[i] = item_index;
            fprintf(file, "%d,%d,%s\n", transaction_id, member_id, items[item_index]);
        }
        transactions[transaction_id - 1] = transaction;
    }
}

// Function to find frequent itemsets using the Apriori algorithm
void apriori(Transaction transactions[], int transaction_count) {
    // Array to store the support count of each item
    int item_support[NUM_ITEMS] = {0};

    // Count the support for each item (1-itemsets)
    for (int i = 0; i < transaction_count; i++) {
        for (int j = 0; j < transactions[i].num_items; j++) {
            item_support[transactions[i].items[j]]++;
        }
    }

    // Print the frequent 1-itemsets
    printf("Frequent 1-itemsets:\n");
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (item_support[i] >= MIN_SUPPORT) {
            printf("%s: %d\n", items[i], item_support[i]);
        }
    }

    // Find frequent 2-itemsets
    printf("\nFrequent 2-itemsets:\n");
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (item_support[i] < MIN_SUPPORT) continue;

        for (int j = i + 1; j < NUM_ITEMS; j++) {
            if (item_support[j] < MIN_SUPPORT) continue;

            int pair_support = 0;
            for (int k = 0; k < transaction_count; k++) {
                int has_i = 0, has_j = 0;
                for (int l = 0; l < transactions[k].num_items; l++) {
                    if (transactions[k].items[l] == i) has_i = 1;
                    if (transactions[k].items[l] == j) has_j = 1;
                }
                if (has_i && has_j) pair_support++;
            }

            if (pair_support >= MIN_SUPPORT) {
                printf("%s, %s: %d\n", items[i], items[j], pair_support);
            }
        }
    }

    // Higher-order itemsets can be found similarly by extending the logic
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
    Transaction transactions[NUM_TRANSACTIONS];
    generate_transaction_data(file, transactions);

    // Close the file
    fclose(file);

    printf("Dataset generated and saved to grocery_store_dataset.csv\n");

    // Apply the Apriori algorithm to find frequent itemsets
    apriori(transactions, NUM_TRANSACTIONS);

    return 0;
}
