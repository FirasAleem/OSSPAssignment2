#include <stdio.h>
#include <string.h>
#include <pthread.h>

pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
//Initialize our lock

void *downtime() {

    Node *balancedTreePointer; //pointer to our new balanced subtree

    for (int i = 0; i < 4; ++i) { //we can use a for loop to do this 3 times as required
        sleep(2); //sleep for 2 seconds
        pthread_rwlock_wrlock(&lock); //wrlock since we are editing the tree
        balancedTreePointer = balanceTree(root); //pointer to our balanced tree
        freeSubtree(root); //free the old unbalanced tree to stop memory leaks
        root = balancedTreePointer; //assign the new pointer to the balanced tree to root
        pthread_rwlock_unlock(&lock); //unlock since we're done
    }

    return NULL;
}


void *ServeClient(char *client) {

    FILE *file = fopen(client, "r"); //open file for reading
    if (file == NULL) {
        return NULL;
    }

    //these are used for getline and strtok_r inside the while loop
    char *line = NULL;
    size_t size = 0;
    ssize_t readline = 0;
    
    char clientString[100]; //array for getting the client name
    strcpy(clientString, client); //make a copy of the client name into our array
    char *clientSaveptr = NULL;
    char *clientName = strtok_r(clientString, "_", &clientSaveptr); //We split the client file's name on _ to get just the start

    while ((readline = getline(&line, &size, file)) != -1) { //while loop to go through the file

        if (readline) { //easier way to check for not NULL (NULL evaluates to 0 which is False)

            char *saveptr = NULL; //used for strtok_r
            char *delimiter = " \t\n"; //store what we are using as the delimiter to break up our client command
            char *token = strtok_r(line, delimiter, &saveptr);//on first use of strtok_r, we use line.
            //line in this case is the actual command from the client (i.e. addNode, removeNode, etc.)

            if (token && strcmp(token, "addNode") == 0) { ///check if clients command is addNode
                int data = atoi(strtok_r(NULL, delimiter, &saveptr)); //subsequent strtok_r so NULL
                //also, atoi just converts the string to an integer, in this case, the value we're adding
                pthread_rwlock_wrlock(&lock); //wrlock as adding nodes is a read-write operation
                root = addNode(root, data); //we actually add the node in now
                printf("%sinsertNode %d\n", clientName, data); //printf for the log
                pthread_rwlock_unlock(&lock); //unlock the read write lock
            } else if (token && strcmp(token, "removeNode") == 0) { //check if clients command is removeNode
                int data = atoi(strtok_r(NULL, delimiter, &saveptr));  //subsequent strtok_r so NULL
                pthread_rwlock_wrlock(&lock); //wrlock as removing nodes is a read-write operation
                root = removeNode(root, data); //remove the node
                printf("%sdeleteNode %d\n", clientName, data); //printf for the log
                pthread_rwlock_unlock(&lock); //unlock the read write lock
            } else if (token && strcmp(token, "countNodes") == 0) { //check if clients command is countNodes
                int count = 0; //used for printing
                pthread_rwlock_rdlock(&lock); //rdlock is used since we just need to read to find number of nodes
                count = countNodes(root); //get a count of the nodes (read-only)
                printf("%scountNodes = %d\n", clientName, count); //printf for the log
                pthread_rwlock_unlock(&lock);//unlock the read lock
            } else if (token && strcmp(token, "avgSubtree") == 0) { //check if clients command is avgSubtree
                float average = 0; //used for printing
                pthread_rwlock_rdlock(&lock); //rdlock is used again, since we just  read to find avg of nodes
                average = avgSubtree(root); //find the average of the tree (also read-only)
                printf("%savgSubtree = %f\n", clientName, average); //printf for the log
                pthread_rwlock_unlock(&lock);//unlock the read lock
            }
        }
    }
    free(line); //free the line pointer to prevent memory leaks
    fclose(file); //close the file
    return NULL;
}


