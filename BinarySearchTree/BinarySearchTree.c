#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    int strength;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct Stack {
    int *data; // Stack elemanlar�
    int top; // Stack'in �st�n� g�steren indis
    int max_size; // Stack'in maksimum boyutu
} Stack;

Node* createNode(int value, int strength);
Node* insert(Node* node, int value, int strength);
Node* findMin(Node* node);
Node* search(Node* node, int value);
Node* deleteNode(Node* node, int value, Stack* budget);
void inOrder(Node* node);
void printTree(Node* root);
void freeBST(Node* node);

Stack* initialize(int max_size);
void push(Stack* stack, int value);
int pop(Stack* stack);
bool isFull(Stack* stack);
bool isEmpty(Stack* stack);
void reverseAndPrintStack(Stack* stack, int M);
void freeStack(Stack* stack);

int main() {
    int N, M, k, p;
    int i;
    int value, strength;
    Node* root = NULL;
    int* used_values = (int*) malloc(sizeof(int) * M); //Burada olmamas� laz�m biliyorum ama a�a��da oldu�unda ilgin� bi �ekilde �al��m�yor
    do {
        printf("Please enter valid values. (N > 1, M > 1, N > M)\n");
        printf("Enter the value of N: ");
        scanf("%d", &N);
        printf("Enter the value of M: ");
        scanf("%d", &M);
    } while( N < 1 || M < 1 || N <= M );    //N ve M say�lar�n�n kontrol�
    // rand() fonksiyonunun her �al��t�rmada rastgele say�lar �retmesi i�in srand() fonksiyonu kullan�m�
	//int* used_values = (int*) malloc(sizeof(int) * M); //Burada olmal�

	srand((unsigned int)time(NULL));
    for (i = 0; i < M; i++) {
        // �nceden kullan�lmam�� rastgele bir de�er se�ilene kadar d�ng� devam eder
        do {
            value = rand() % N + 1;
        } while (used_values[value] == 1);
        used_values[value] = 1; // kullan�ld���n� i�aretlemek i�in 1 olarak ayarlan�r
        strength = rand() % N + 1;
        root = insert(root, value, strength);
    }
    printTree(root);
    Stack* bucket = initialize (M);
	printf("\n");

	
    while (root != NULL) {
        printf("Enter the value of k: ");
        scanf("%d", &k);
        printf("Enter the value of p: ");
        scanf("%d", &p);
        
        Node* target = search(root, k);
        if (target != NULL) {
            value = target->value;
            target->strength -= p;
            if (target->strength <= 0) {
                root = deleteNode(root, value, bucket);
				push(bucket, value);
            }
        }
        else {
            printf("There is no apple with the given value.\n");
        }
        printTree(root);
        printf("\n");
    }
    
    printf("The order of fruits that fall into the bucket:");
	reverseAndPrintStack(bucket, M);
    
    free (used_values);
    freeBST(root);
    freeStack(bucket);
    return 0;
}

Node* createNode(int value, int strength) {
	Node* node = (Node*) malloc(sizeof(Node));
    node->value = value;
    node->strength = strength;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* insert(Node* root, int value, int strength) {
    if (root == NULL) {
        return createNode(value, strength);
    }
    if (value == root->value) { // e�er ayn� say� zaten varsa, g�c�n� g�ncelle
        root->strength = strength;
        return root;
    }
    if (value < root->value) {
        root->left = insert(root->left, value, strength);
    } else {
        root->right = insert(root->right, value, strength);
    }
    return root;
}

Node* findMin(Node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

Node* search(Node* node, int value) {
    if (node == NULL || node->value == value) {
        return node;
    }
    if (value < node->value) {
        return search(node->left, value);
    }
    return search(node->right, value);
}

Node* deleteNode(Node* node, int value, Stack* bucket) {
    if (node == NULL) {
        return node;
    }
    if (value < node->value) {
        node->left = deleteNode(node->left, value, bucket);
    } else if (value > node->value) {
        node->right = deleteNode(node->right, value, bucket);
    } else {
        if (node->left == NULL) {
            Node* temp = node->right;
            free(node);
            node = NULL;
            return temp;
        } else if (node->right == NULL) {
            Node* temp = node->left;
            free(node);
            return temp;
        }
        Node* temp = findMin(node->right);
        node->value = temp->value;
        node->strength = temp->strength;
        node->right = deleteNode(node->right, temp->value, bucket);
    }
    return node;
}


void printTree(Node* root) {
    if (root != NULL) {
        printTree(root->left);
        printf("(%d, %d) ", root->value, root->strength);
        printTree(root->right);
    }
}

void freeBST(Node* node) {
    if (node == NULL) {
        return;
    }
    freeBST(node->left);    // sol alt a�ac� serbest b�rak
    freeBST(node->right);   // sa� alt a�ac� serbest b�rak
    free(node);             // d���m� serbest b�rak
}

Stack* initialize(int max_size) {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->data = (int*) malloc(sizeof(int) * max_size);
    stack->top = -1;
    stack->max_size = max_size;
    return stack;
}

// Push i�lemi, stack'e eleman ekler
void push(Stack* stack, int value) {
	if (stack->top == stack->max_size - 1) {
        printf("Stack overflow!\n"); // Stack dolu
        return;
    }
    stack->data[++stack->top] = value; // Stack'e eleman ekle
}

// Pop i�lemi, stack'ten eleman ��kar�r
int pop(Stack* stack) {
    if (stack->top == -1) {
        printf("Stack underflow!\n"); // Stack bo�
        return -1;
    }
    return stack->data[stack->top--]; // Stack'ten eleman ��kar
}

bool isEmpty(Stack* stack) {
    if (stack->top == -1) {
        return true;
    } else {
        return false;
    }
}

void reverseAndPrintStack(Stack* stack, int M) {
    Stack* tempStack = initialize(M); // ge�ici stack yap�s� olu�tur

    // orijinal stack'teki t�m elemanlar� ge�ici stack'e ekleyin
    while (!isEmpty(stack)) {
        push(tempStack, pop(stack));
    }
    // ge�ici stack'teki t�m elemanlar� orijinal stack'ten ters s�rada yazd�r�n
    while (!isEmpty(tempStack)) {
        printf("%d ", pop(tempStack));
    }
    // ge�ici stack yap�s�n� serbest b�rak�n
    freeStack(tempStack);
}

void freeStack(Stack* stack) {
    free(stack->data);
    free(stack);
}
