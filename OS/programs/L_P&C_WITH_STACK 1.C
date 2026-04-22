#include <stdio.h>
#include <stdlib.h>
 
#define MAX 10
 
int stack[MAX];
int top = -1;
 
// Function to check if stack is full
int isFull() {
    return top == MAX - 1;
}
 
// Function to check if stack is empty
int isEmpty() {
    return top == -1;
}
 
// Function to push an item onto the stack (Producer)
void push(int item) {
    if (isFull()) {
        printf("Stack is full. Producer cannot produce.\n");
        return;
    }
    stack[++top] = item;
    printf("Producer produced item: %d\n", item);
}
 
// Function to pop an item from the stack (Consumer)
void pop() {
    if (isEmpty()) {
        printf("Stack is empty. Consumer cannot consume.\n");
        return;
    }
    int item = stack[top--];
    printf("Consumer consumed item: %d\n", item);
}
 
// Main function with menu
int main() {
    int choice, item;
 
    while (1) {
        printf("\n1. Producer\n2. Consumer\n3. Exit\nEnter your choice: ");
        scanf("%d", &choice);
 
        switch (choice) {
            case 1:
                printf("Enter item to produce: ");
                scanf("%d", &item);
                push(item);
                break;
 
            case 2:
                pop();
                break;
 
            case 3:
                exit(0);
 
            default:
                printf("Invalid choice!\n");
        }
    }
 
    return 0;
}
