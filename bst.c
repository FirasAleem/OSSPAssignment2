#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "bst.h"

struct _Node {
  int data;
  struct _Node* left;
  struct _Node* right;
};

Node* addNode(Node *root, int data){
  Node *newNode = malloc(sizeof(Node));
  newNode->data = data;
  newNode->left = NULL;
  newNode->right = NULL;

  Node *temp;
  Node *prev = NULL;

  temp = root;
  while(temp) {
    prev = temp;
    if (temp->data > newNode->data)
      temp = temp->right;
    else if (temp->data < newNode->data)
      temp = temp->left;
    else {
      free(newNode);
      return NULL;
    }
  }
  if (prev==NULL)
    root = newNode;
  else {
    if (prev->data > newNode->data)
      prev->right = newNode;
    else
      prev->left = newNode;
  }

  return root;
}

Node* freeSubtree(Node *N) {
  if (N == NULL){
      Node* returnNodeNull = NULL;
      return returnNodeNull;
  }
  freeSubtree(N->left);
  freeSubtree(N->right);
  free(N);
  Node* returnNodeNull = NULL;
    return returnNodeNull;
}

void removeSubtreeWorker(Node *R, int data) {
  if (R == NULL)
    return;

  if (R->left && R->left->data == data) {
    freeSubtree(R->left);
    R->left = NULL;
    return;
  }

  if (R->right && R->right->data == data) {
    freeSubtree(R->right);
    R->right = NULL;
    return;
  }

  if (R->data < data)
    removeSubtreeWorker(R->left, data);
  else
    removeSubtreeWorker(R->right, data);
}

Node* removeSubtree(Node *R, int data) {
  if (R == NULL)
    return NULL;

  if (R->data == data) {
    freeSubtree(R);
    return NULL;
  }
  removeSubtreeWorker(R, data);
  return R;
}

void displaySubtree(Node *N)
{
  if (N==NULL){
      return ;
    }
  displaySubtree(N->right);
  printf("%d\n", N->data);
  displaySubtree(N->left);
}

int countNodes(Node *N)
{
  int count=0;
  if(N==NULL)
    return 0;
  count = count + countNodes(N->right);
  count = count + countNodes(N->left);
  count = count + 1;
  return count;
}

// this is the most complicated task
Node* removeNode(Node* root, int data)
{
  // Ensure root isn't null.
  if (root == NULL) {
    return NULL;
  }
  	
  if (data > root->data) {  // Value is in the left subtree.
    root->left = removeNode(root->left, data);
  } else if (data < root->data) { // Value is in the right subtree.
    root->right = removeNode(root->right, data);
  } else { // Found the correct node with data
    // Check the three cases - no child, 1 child, 2 child...
    // No Children
    if (root->left == NULL && root->right == NULL) {
      free(root);
      root = NULL;
    }
    // 1 child (on the right)
    else if (root->left == NULL) {
      Node *temp = root; // save current node
      root = root->right;
      free(temp);
    }
    // 1 child (on the left)
    else if (root->right == NULL) {
      Node *temp = root; // save current node
      root = root->left;
      free(temp);
    }
    // Two children
    else {
      // find minimal data of right subtree
      Node *temp = root->left; 
      while(temp->right != NULL) {
        temp = temp->right;
      }
      root->data = temp->data; // duplicate the node
      root->left = removeNode(root->left, root->data); // delete the duplicate node
    }
  }	
  return root; // parent node can update reference
}

int numberLeaves(Node *N) {
  if (N == NULL)
    return 0;

  if (N->left == NULL && N->right == NULL)
    return 1;

  return numberLeaves(N->left) + numberLeaves(N->right);
}

int nodeDepth(Node *R, Node *N) {
  if (R == NULL || N == NULL)
    return -1;

  if (R == N)
    return 0;

  int subDepth = nodeDepth(R->data < N->data ? R->left : R->right, N);

  if (subDepth >= 0)
    return subDepth + 1;
  else
    return -1;
}

int sumOfTree(Node* root) {
    if (root == NULL) {
        return 0;
    }
    int total = (root -> data) + sumOfTree(root->left)+ sumOfTree(root->right);
    return total;
}

float avgSubtree(Node *N){
    if (N==NULL){
        return 0;
    }
    int total = sumOfTree(N);
    float average = (float) total / countNodes(N);

    return average;
}

void inorderTraversal(Node* root, int* bstArray, int* index) {
    if (root == NULL){
        return;
    }
    //store the left subtree
    inorderTraversal(root->left, bstArray, index);

    //store current root value
    bstArray[*index] = root->data;
    (*index)++;

    //store the right subtree
    inorderTraversal(root->right, bstArray, index);
}

int* sortedValuesIntoArray(Node * root){
    int index = 0;
    int size = countNodes(root);
    int* arrayBST = (int*)malloc(size*sizeof(int));
    inorderTraversal(root, arrayBST, &index);
    return arrayBST;
}

Node* arrayToBST(int* arrayPointer, int start, int end){
    if (start > end){
        return NULL;
    }
    int middle = (start + end)/2;
    Node* newRoot = (Node *) malloc(sizeof(Node));
    newRoot->data = arrayPointer[middle];
    newRoot->left = arrayToBST(arrayPointer, start, middle-1);
    newRoot->right = arrayToBST(arrayPointer, middle+1, end);
    return newRoot;
}

// This functions converts an unbalanced BST to a balanced BST but still inverted
Node* balanceTree(Node* root){

    int size = countNodes(root);
    int* arrayPointer = sortedValuesIntoArray(root);
    root = arrayToBST(arrayPointer, 0, size-1);

    //PRINTING, FOR TESTING!
//    int length = countNodes(root);
//    for(int i = 0; i < length; ++i ) {
//        printf("%d\n", *(arrayPointer + i));
//    }
    free(arrayPointer);
    return root;
}



