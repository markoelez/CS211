#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct node {
  int val;
  struct node *next;
};

struct node* insertNode(int *len, struct node *head, struct node *list, int target) {
  // if target is at head, return
  if (list -> val == target) {
    return head;
  }
  // if target is less than head
  if (list -> val > target) {
    // allocate memory for new node
    struct node *node = malloc(sizeof(struct node));
    node -> val = target;
    // set node as new head
    node -> next = head;
    head = node;
    // update length
    (*len)++;
    // return new list
    return head;
  }
  // if target is greater than head
  if (list -> val < target) {
    // check if node fits after head
    if ((list -> next && list -> next -> val > target) || !(list -> next)) {
      // allocate memory
      struct node *n = malloc(sizeof(struct node));
      // insert node
      n -> val = target;
      n -> next = list -> next;
      list -> next = n;
      (*len)++;
      // return new list
      return head;
    }
  }
  // recursively search the rest of the list
  return insertNode(len, head, list -> next, target);
}

struct node* deleteNode(struct node *head, int target) {
  // check if head is target
  if (head -> val == target) {
    // if so, return next
    return head -> next;
  }
  // init pointers
  struct node *curr;
  struct node *prev;
  // iterate through, moving pointer as we go
  for (curr = head; curr -> val != target; curr = curr -> next) {
    prev = curr;
  }
  // curr will be pointing at target
  prev -> next = curr -> next;
  return head;
}

int search(struct node *head, int target) {
  // if null head, return
  if (!head) return 0;
  // iterate through list
  struct node *curr;
  for (curr = head; curr != NULL; curr = curr -> next) {
    // return if found
    if (curr -> val == target) return 1;
  }
  return 0;
}

void printList(struct node *list) {
  // not null, print and recurse
  if (list != NULL) {
    printf(" %d", list -> val);
    printList(list -> next);
  } else {
    printf("\n");
  }
}

int main(int argc, char **argv) {
  // init list
  struct node *head = NULL;
  int len = 0;
  
  char c = '\0';
  int n = 0;
  int count = 2;
  // deal with input
  while (count >= 2) {
    if (c == 'i') {
      if (!head) {
        head = malloc(sizeof(struct node));
        head -> val = n;
        head -> next = NULL;
        len++;
      } else if (!search(head, n)) {
        head = insertNode(&len, head, head, n);
      }
    }
    if (c == 'd' && head && search(head, n)) {
      head = deleteNode(head, n);
      len--;
    }
    if (c == 'i' || c == 'd') {
      printf("%d :", len);
      printList(head);
    }
    c = '\0';
    count = scanf(" %c", &c);
    if (c == 'i' || c == 'd') {
      count += scanf("%d", &n);
    }
  }
  // free up memory
  free(head);
  return EXIT_SUCCESS;
}