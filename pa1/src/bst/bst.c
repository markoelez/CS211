#include <stdio.h>
#include <stdlib.h>

struct node {
  int val;
  struct node *left;
  struct node *right;
};

struct node* createNode(int target) {
  // allocate memory
  struct node *node = malloc(sizeof(struct node));
  // create node with target value
  node -> val = target;
  node -> left = NULL;
  node -> right = NULL;
  return node;
}

void insert(struct node *root, int target) {
  // check if val in left subtree
  if (root -> val > target) {
    // if we have reached a leaf node
    if (!(root -> left)) {
      // create node
      root -> left = createNode(target);
      return;
    }
    // recurse down left subtree
    insert(root -> left, target);
  }
  // check if val in right subtree
  if (root -> val < target) {
    // if we have reached a leaf node
    if (!(root -> right)) {
      // create node
      root -> right = createNode(target);
      return;
    }
    // recurse down right subtree
    insert(root -> right, target);
  }
}

void delete(struct node *parent, struct node *current, int target) {
  // if we have reached node
  if (current -> val == target) {
    // if this is a leaf node, delete it
    if (!(current -> left) && !(current -> right)) {
      // if this is a left child
      if (current -> val < parent -> val) {
        free(parent -> left);
        parent -> left = NULL;
        return;
      }
      // otherwise, this is a right child
      free(parent -> right);
      parent -> right = NULL;
      return;
    }
    // if node has a left subtree:
    // 1. get inorder successor
    // 2. swap with node
    // 3. delete inorder successor
    if (current -> left) {
      struct node *curr;
      struct node *prev = NULL;
      for (curr = current -> left; curr -> right != NULL; curr = curr -> right) {
        prev = curr;
      }
      if (prev) {
        current -> val = prev -> right -> val;
        delete(prev, prev -> right, prev -> right -> val);
      }
      else {
        int val = current -> left -> val;
        delete(current, current -> left, current -> left -> val);
        current -> val = val;
      }
      return;
    }
    if (current -> right) {
      struct node *node = current -> right;
      current -> val = current -> right -> val;
      current -> left = current -> right -> left;
      current -> right = current -> right -> right;
      free(node);
      return;
    }
  }
  // check left subtree
  if (current -> val > target) {
    delete(current, current -> left, target);
    return;
  }
  // check right subtree
  delete(current, current -> right, target);
  return;
}

int search(struct node *root, int target) {
  // check if we have reached a leaf node
  if (root == NULL) return 0;
  // check root
  if (root -> val == target) return 1;
  // check left subtree
  if (root -> val > target) {
    return search(root -> left, target);
  }
  // check right subtree
  return search(root -> right, target);
}

void printTree(struct node *root) {
  // check root
  if (root == NULL) return;
  // print left subtree
  printf("(");
  printTree(root -> left);
  // print root
  printf("%d", root -> val);
  // print right subtree
  printTree(root -> right);
  printf(")");
}

int main() {
  struct node *root = NULL;

  char type = '\0';
  int n = 0;
  int sc = 1;

  // deal with input
  while (sc >= 0) {
    if (type == 's') {
      if(search(root, n)) {
        printf("present\n");
      }
      else {
        printf("absent\n");
      }
    }

    if (type == 'i') {
      if (!search(root, n)) {
        if (root == NULL) {
          root = createNode(n);
        }
        else {
          insert(root, n);
        }
        printf("inserted\n");
      }
      else {
        printf("not inserted\n");
      }
    }

    if (type == 'd') {
      if (search(root, n)) {
        if (root -> val == n && !(root -> left) && !(root -> right)) {
          root = NULL;
        }
        else {
          delete(NULL, root, n);
        }
        printf("deleted\n");
      }
      else {
        printf("absent\n");
      }
    }

    type = '\0';
    sc = scanf(" %c", &type);
    if (type == 'p') {
      printTree(root);
      printf("\n");
    }
    if (type == 'i' || type == 's' || type == 'd') {
      sc += scanf("%d", &n);
    }
  }

  free(root);

  return EXIT_SUCCESS;
}