#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

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

// Function to calculate Chi-Square for item pairs
void calculate_chi_square(Transaction transactions[], int transaction_count) {
    int pair_counts[NUM_ITEMS][NUM_ITEMS] = {0};
    int item_counts[NUM_ITEMS] = {0};
    int total_transactions = transaction_count;

    // Calculate support counts for individual items and pairs
    for (int i = 0; i < transaction_count; i++) {
        int unique_items[NUM_ITEMS] = {0}; // To track which items are present in a single transaction
        for (int j = 0; j < transactions[i].num_items; j++) {
            int item_index = transactions[i].items[j];
            item_counts[item_index]++;
            unique_items[item_index] = 1;
        }

        for (int x = 0; x < NUM_ITEMS; x++) {
            if (unique_items[x]) {
                for (int y = x + 1; y < NUM_ITEMS; y++) {
                    if (unique_items[y]) {
                        pair_counts[x][y]++;
                    }
                }
            }
        }
    }

    // Calculate Chi-Square values for each pair of items
    printf("\nChi-Square values for item pairs:\n");
    for (int i = 0; i < NUM_ITEMS; i++) {
        for (int j = i + 1; j < NUM_ITEMS; j++) {
            int A = pair_counts[i][j];  // Count of transactions containing both items i and j
            int B = item_counts[i] - A; // Count of transactions containing item i but not j
            int C = item_counts[j] - A; // Count of transactions containing item j but not i
            int D = total_transactions - (A + B + C); // Count of transactions containing neither i nor j

            // Calculate expected frequencies
            double expected_A = ((item_counts[i] * item_counts[j]) / (double)total_transactions);
            double expected_B = item_counts[i] - expected_A;
            double expected_C = item_counts[j] - expected_A;
            double expected_D = total_transactions - (expected_A + expected_B + expected_C);

            // Chi-Square formula: Σ((O-E)^2 / E)
            double chi_square = 0.0;
            if (expected_A > 0) chi_square += ((A - expected_A) * (A - expected_A)) / expected_A;
            if (expected_B > 0) chi_square += ((B - expected_B) * (B - expected_B)) / expected_B;
            if (expected_C > 0) chi_square += ((C - expected_C) * (C - expected_C)) / expected_C;
            if (expected_D > 0) chi_square += ((D - expected_D) * (D - expected_D)) / expected_D;

            printf("(%s, %s): Chi-Square = %.2f\n", items[i], items[j], chi_square);
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
    Transaction transactions[NUM_TRANSACTIONS];
    generate_transaction_data(file, transactions);

    // Close the file
    fclose(file);

    printf("Dataset generated and saved to grocery_store_dataset.csv\n");

    // Calculate Chi-Square for each pair of items
    calculate_chi_square(transactions, NUM_TRANSACTIONS);

    return 0;
}
