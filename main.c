#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- DATA STRUCTURES ---
typedef struct Product {
    int id;
    char name[50];
    int stock;
    float price;
    struct Product* next;
} Product;

typedef struct Transaction {
    char receiptLine[150];
    struct Transaction* next;
} Transaction;

Product* head = NULL;
Transaction *front = NULL, *rear = NULL;

// --- UTILITY: Panlinis ng "Enter" key sa memory ---
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- ALGORITHMS ---
Product* findProduct(int id) {
    Product* curr = head;
    while(curr) {
        if(curr->id == id) return curr;
        curr = curr->next;
    }
    return NULL;
}

void addTransactionLog(char* msg) {
    Transaction* newNode = (Transaction*)malloc(sizeof(Transaction));
    strcpy(newNode->receiptLine, msg);
    newNode->next = NULL;
    if(!rear) { front = rear = newNode; }
    else { rear->next = newNode; rear = newNode; }
}

// --- DISPLAY LOGIC ---
void showInventory() {
    Product* curr = head;
    printf("\n====================================================");
    printf("\n           AVAILABLE PRODUCT IN WAREHOUSE");
    printf("\n====================================================");
    printf("\nID\tProduct Name\t\tStock\tPrice");
    printf("\n----------------------------------------------------");
    if(!curr) printf("\n          *** STORE IS EMPTY ***");
    while(curr) {
        printf("\n%-8d %-20s %-7d P%.2f", curr->id, curr->name, curr->stock, curr->price);
        curr = curr->next;
    }
    printf("\n====================================================\n");
}

// --- CUSTOMER BUYING FEATURE ---
void processPurchase() {
    int id, qty;
    float payment, total, change;
    char customerName[50];

    if (head == NULL) {
        printf("\n[Error] Store is empty! Admin must add products first.\n");
        return;
    }

    printf("\n[CUSTOMER] Enter Your Name: ");
    clearBuffer(); // Linisin ang buffer bago kumuha ng pangalan
    fgets(customerName, 50, stdin);
    customerName[strcspn(customerName, "\n")] = 0; 

    printf("[%s] Enter Product ID to Buy: ", customerName); 
    if (scanf("%d", &id) != 1) {
        printf("\n[Error] Invalid ID format!\n");
        clearBuffer();
        return;
    }
    
    Product* p = findProduct(id);

    if(!p) {
        printf("\n[Error] Product ID not found!\n");
        return;
    }

    if(p->stock <= 0) {
        printf("\n[Error] Sorry %s, %s is Out of Stock!\n", customerName, p->name);
        return;
    }

    printf("      Buying: %s | Price: P%.2f | Available: %d", p->name, p->price, p->stock);
    printf("\n      Enter Quantity:");
    scanf("%d", &qty);

    if(qty > p->stock || qty <= 0) {
        printf("\n[Error] Invalid quantity!\n");
        return;
    }

    total = qty * p->price;
    printf("\nTOTAL AMOUNT TO PAY: P%.2f", total);
    printf("\nPlease Enter Cash Amount: P");
    scanf("%f", &payment);

    if(payment < total) {
        printf("\n[Error] Insufficient Cash! Transaction Cancelled.\n");
    } else {
        change = payment - total;
        p->stock -= qty; 

        printf("\n\n************************************");
        printf("\n         OFFICIAL RECEIPT");
        printf("\n**************************************");
        printf("\n Customer:            %s", customerName);
        printf("\n Item:                     %s", p->name);
        printf("\n Quantity:                     %d", qty);
        printf("\n Total:                   P%.2f", total);
        printf("\n Cash:                  P%.2f", payment);
        printf("\n--------------------------------------");
        printf("\n CHANGE:                 P%.2f", change);
        printf("\n****************88********************");
        printf("\n      Thank you for your order!");
        printf("\n*************************************\n");

        char log[150];
        sprintf(log, "Customer: %s | Bought: %d %s | Total: P%.2f", customerName, qty, p->name, total);
        addTransactionLog(log);
    }
    clearBuffer();
}

// --- ADMIN ADD PRODUCT ---
void addNewProduct() {
    int newId;
    printf("\n[ADMIN] Enter New Product ID: "); 
    if (scanf("%d", &newId) != 1) { 
        printf("[Error] Invalid ID format! Use numbers only.\n");
        clearBuffer();
        return;
    }

    if(findProduct(newId)) { 
        printf("\n[Error] ID %d nauna na! Transaction Cancelled.\n", newId);
        clearBuffer();
        return; 
    }

    Product* newNode = (Product*)malloc(sizeof(Product));
    if (newNode == NULL) {
        printf("[Error] Out of memory!\n");
        return;
    }

    newNode->id = newId;
    printf("[ADMIN] Enter Name: "); 
    scanf("%s", newNode->name);
    printf("[ADMIN] Enter Stock: "); 
    scanf("%d", &newNode->stock);
    printf("[ADMIN] Enter Price: "); 
    scanf("%f", &newNode->price);
    
    newNode->next = head;
    head = newNode;

    printf("\nSUCCESS: %s added to inventory!\n", newNode->name);
    clearBuffer();
}

void seedData() {
    int ids[] = {101, 102, 103};
    char names[][20] = {"Laptop", "Keyboard", "Mouse"};
    int stocks[] = {5, 15, 25};
    float prices[] = {35000, 1200, 500};
    for(int i=0; i<3; i++) {
        Product* n = (Product*)malloc(sizeof(Product));
        n->id = ids[i]; strcpy(n->name, names[i]);
        n->stock = stocks[i]; n->price = prices[i];
        n->next = head; head = n;
    }
}

int main() {
    seedData();
    int choice;
    while(1) {
        showInventory(); 
        printf("\n--------------- SMART RETAIL WAREHOUSE -------------");
        printf("\n1. BUY PRODUCT (Customer)");
        printf("\n2. ADD NEW PRODUCT (Admin)");
        printf("\n3. VIEW SALES HISTORY (Admin)");
        printf("\n4. EXIT");
        printf("\nChoice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Use numbers 1-4.\n");
            clearBuffer();
            continue;
        }

        switch(choice) {
            case 1: processPurchase(); break;
            case 2: addNewProduct(); break;
            case 3: {
                Transaction* curr = front;
                printf("\n--- ADMIN: DAILY SALES LOG ---");
                if(!curr) printf("\nNo transactions yet.");
                while(curr) { printf("\n[LOG] %s", curr->receiptLine); curr = curr->next; }
                printf("\n-------------------------------\n");
                break;
            }
            case 4: printf("Exiting system...\n"); exit(0);
            default: printf("\nInvalid Choice!\n");
        }
    }
    return 0;
}
