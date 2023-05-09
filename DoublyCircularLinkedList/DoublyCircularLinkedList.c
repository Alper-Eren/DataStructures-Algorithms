#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
};

struct Node* createNode(int data);
void createWheel(struct Node** head, int common_number, int N, int M);
void createOtherWheel(struct Node** head, int common_number, int N, int M);
void findCommonNumber(struct Node* head1, struct Node** head2, struct Node** head3, int common_number, int M);
int findCommon(struct Node* head, int common_number);
void rotateWheels (struct Node** head, int rotate, int isClockwise);
void printWheels (struct Node *head);
void freeList(struct Node* head);

int main() {
    int N, M;
    do {
        printf("Please enter valid values. (N > 1, M > 1, N > M)\n");
        printf("Enter the value of N: ");
        scanf("%d", &N);
        printf("Enter the value of M: ");
        scanf("%d", &M);
    } while( N < 1 || M < 1 || N <= M );	//N ve M sayýlarýnýn kontrolü
    // rand() fonksiyonunun her çalýþtýrmada rastgele sayýlar üretmesi için srand() fonksiyonu kullanýmý
	srand((unsigned int)time(NULL));
    int common_number = (rand() % N) + 1;
    printf ("Common_number: %d\n\n", common_number);
    
    struct Node* head1 = NULL;
    struct Node* head2 = NULL;
    struct Node* head3 = NULL;
	
	printf("First wheel:");
	createOtherWheel(&head1, common_number, N, M);
	printWheels (head1);
    printf("Common number found at position %d\n\n", findCommon (head1, common_number));

	printf("Second wheel: ");
	createOtherWheel(&head2, common_number, N, M);
    printWheels (head2);
    printf("Common number found at position %d\n\n", findCommon(head2, common_number));

    printf("Third wheel: ");
	createOtherWheel(&head3, common_number, N, M);
    printWheels (head3);
    printf("Common number found at position %d\n\n", findCommon(head3, common_number));

	findCommonNumber(head1, &head2, &head3, common_number, M);
	
	freeList(head1);
	freeList(head2);
	freeList(head3);

	return 0;
}

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void createWheel(struct Node** head, int common_number, int N, int M) {
    struct Node* tail = NULL;
    struct Node* current = NULL;
    int num, found;
    int i = 1, j;
    //ortak sayýnýn çarka eklenmesi
    struct Node* commonNode = createNode(common_number);
    if (current == NULL) {
        *head = commonNode;
        current = *head;
    } else {
        current->next = commonNode;
        commonNode->prev = current;
        current = current->next;
    }
	int nums[M]; // Eklenen sayýlarý saklayan dizi
	// çark elemanlarýný oluþtur
	while (i < M) {
	    num = rand() % N + 1;
	    if (num != common_number) {
	        found = 0;
	        for (j = 0; j < i; j++) {
	            if (nums[j] == num) {
	                found = 1;
	            }
	        }
	        //ortak sayý ile ayný deðilse diziye eklenmesi
	        if (!found) {
	            nums[i] = num;
	            struct Node* newNode = createNode(num);
                current->next = newNode;
                newNode->prev = current;
                current = current->next;
	            i++;
	        }
	    }
	}
    // son düðümü head ile baðla
    tail = current;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = *head;
    (*head)->prev = tail;
}

void createOtherWheel(struct Node** head, int common_number, int N, int M) {
	createWheel(head, common_number, N, M);
    struct Node* current = NULL;
	current = *head;
	int random = rand() % M;
	int i, tmp;
	//ortak sayýnýn yerini random gelen sayýyla deðiþtirme iþlemi
	for (i = 0; i < random; i++){
		current = current->next;	
	}
	tmp = current->data;
	current->data = (*head)->data;
	(*head)->data = tmp;
}

void printWheels (struct Node *head) {
	struct Node *current = head;
	do {
		printf ("%d ", current->data);
		current = current->next;
	} while(current != head);
	printf ("\n");
}

int findCommon(struct Node* head, int common_number) {
	int pos = 1;
	struct Node* current = head;
	while (current->data != common_number) {
    	current = current->next;
        pos++;
    }
    return pos;
}

void findCommonNumber(struct Node* head1, struct Node** head2, struct Node** head3, int common_number, int M) {
    int pos1 = 1, pos2 = 1, pos3 = 1;
	//positionlarý atama iþlemi
	pos1 = findCommon (head1, common_number);
	pos2 = findCommon (*head2, common_number);
	pos3 = findCommon (*head3, common_number);
	//her çark için 2 ayrý uzaklýk bulmamýz gerekiyor
    int steps2 = (pos1 - pos2 + M) % M;
	int neg_steps2 = (pos2 - pos1 + M) % M;  
    int steps3 = (pos1 - pos3 + M) % M;
    int neg_steps3 = (pos3 - pos1 + M) % M;
	
	printf ("First Wheel:\n");
	printWheels (head1);
	//hangisi daha yakýnsa onu kullanýyoruz
	printf("\nSecond Wheel: \n");
	if (pos1 == pos2) {
		printf("No need to rotate the second wheel.\n");
		printWheels (*head2);	
	}
	else if (steps2 > neg_steps2) {
		printf("The second wheel needs to be rotated %d steps counterclockwise.\n", neg_steps2);
		rotateWheels (head2, neg_steps2, 0);
	} 
	else {
		printf("The second wheel needs to be rotated %d steps clockwise.\n", steps2);
		rotateWheels (head2, steps2, 1);
	}

	printf("Third Wheel:\n");  	
	if (pos1 == pos3) {
		printf("No need to rotate the third wheel.\n");
		printWheels (*head3);
	}
	else if (steps3 > neg_steps3) {
		printf("The third wheel needs to be rotated %d steps counterclockwise.\n", neg_steps3);
		rotateWheels (head3, neg_steps3, 0);
	}  
	else {
		printf("The third wheel needs to be rotated %d steps clockwise.\n", steps3);
		rotateWheels (head3, steps3, 1);
	}   	
}

void rotateWheels (struct Node** head, int rotate, int isClockwise) {
	int i;
	//hangi tarafa diziyi öteleyeceðimiz clokcwise deðiþkeni ile fonksiyonumuza geliyor
	for (i = 0; i < rotate; i++) {
		if (isClockwise) {
			*head = (*head)->prev;
		}
		else {
			*head = (*head)->next;
		}
	}
	printWheels (*head);
	printf("\n");
}

void freeList(struct Node* head) {
	//Diziyi serbest býrakýyoruz
    struct Node* current = head;
    struct Node* next;
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != head);
}

