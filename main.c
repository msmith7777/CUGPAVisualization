#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Create struct to represent wanted data
typedef struct Course{

    char* crs;
    char* crsNumber;
    char* crsTitle;
    double avgGpa;
    double avgWRate;
    char* instructor;
    char* honors;
    int count;

    struct Course* next;
}Course;

//Free a Course struct
void freeCrs(Course* course) {
    free(course -> crs);
    free(course -> crsNumber);
    free(course -> crsTitle);
    free(course -> instructor);
    free(course -> honors);
    free(course);
}

//Free the linked list
void freeList(Course* head) {
    Course* save;
    while(head != NULL) {
        save = head -> next;
        freeCrs(head);
        free(head);
        head = save;
    }
}
/*
Create a struct. Arguements, in order, are course, course number, course title, total gpa, 
total withdraw rate, instructor name, and honors ("H\n" for honors, "\n" if not"
*/
Course* createCourse(char* str, char* str2, char* str3, double gpa, double wRate, char* str4, char* str5) {
    Course* newNode = calloc(1, sizeof(Course));
    if (newNode == NULL) {
        printf("Malloc failed.\n");
        return NULL;
    }

    newNode -> crs = strdup(str);
    newNode -> crsNumber = strdup(str2);
    newNode -> crsTitle = strdup(str3);
    newNode -> instructor = strdup(str4);
    newNode -> honors = strdup(str5);
    
    newNode -> avgGpa = gpa;
    newNode -> avgWRate = wRate;
    newNode -> count = 1;

    newNode -> next = NULL;

    return newNode;
}

/*
Compare data in 2 Course structs. If course AND instructor name AND honors is the same, return 0. 
If just course name is the same, return 1. If neither, return 2.
*/
int crscmp(Course* crs1, Course* crs2) {
    int numCmp = strcmp(crs1 -> crs, crs2 -> crs);
    int crsNumberCmp = strcmp(crs1 -> crsNumber, crs2 -> crsNumber);
    int instrCmp = strcmp(crs1 -> instructor, crs2 -> instructor);
    int hnrsCmp = strcmp(crs1 -> honors, crs2 -> honors);

    if (numCmp == 0 && crsNumberCmp == 0 && instrCmp == 0 && hnrsCmp == 0){
        return 0;
    } else if (numCmp == 0 && crsNumberCmp == 0) {
        return 1;
    }
    return 2;
}

/*Merge 2 entries of a Course struct by averaging values. The 2nd course is the new one. !!DO NOT
CALL THIS ON A COURSE IN THE LIST!!
*/
void mergeCourses(Course* crs1, Course* crs2) {
    crs1 -> count += 1;
    crs1 -> avgGpa = (crs1 -> avgGpa*  (crs1 -> count - 1) + crs2 -> avgGpa) / (crs1 -> count);
    crs1 -> avgWRate = (crs1 -> avgWRate*  (crs1 -> count - 1) + crs2 -> avgWRate) / (crs1 -> count);

    //crs2 is not part of the linked list, so this is fine
    freeCrs(crs2);
}

//Insert one node between a given node and it's neighbor
void insertNode(Course* crs1, Course* crs2) {
    Course* nextnext = crs1 -> next;
    crs1 -> next = crs2;
    crs2 -> next = nextnext;
}

// Merge node with a matching course or group it with others of the same course and number
void addNode(Course** head, Course* node) {
    if (*head == NULL) {
     *head = node;
        return;
    }

    Course* current = *head;
    Course* save = *head;
    //Iterate through the linked list
    while (current != NULL) {
        int crsCmp = crscmp(current, node);
    //If the course has the same name AND instructor AND is honors as one in the list, merge the entries
        if (crsCmp == 0) {
            mergeCourses(current, node);
            return;
        }

    /*
    If the course has the same course name, keep checking through the group.
    If the instructor is there, and both are honors, merge the entries. Otherwise, just put it at the end.
    */
        if (crsCmp == 1) {
            while (current != NULL && crscmp(current, node) <= 1) {
                if (crscmp(current, node) == 0) {
                    mergeCourses(current, node);
                    return;
                }
                save = current;
                current = current -> next;
            }
            insertNode(save, node);
            return;
        }
        save = current;
        current = current -> next;
    }
    save -> next = node;
    node -> next = NULL;
}

/*Arguements are a line of CSV and token. Checks if token starts with quotes, and, if it does, 
returns ptr to the full field, ignoring commas between. TODO: fix.
*/
char* checkQuotes(char* line, char* token) {
    if (token[0] == '"') {
            char* buff;
            buff = strdup(token);

            while (buff[strlen([buff] - 1)]) {
                strcat(buff, ",");
                token = strtok(NULL, ",");
                strcat(buff, token);
            }
            return buff;
    }
    return token;
}

/*
Argument is a line from the CSV and expected number of fields. Extract useful information from the line, 
assign a Course to that data, and return the pointer to the course. If class is pass-fail, return NULL.
*/
Course* parseCSV(char* line, int numFields) {
    char* crs;
    char* crsNum;
    char* crsTitle;
    double avgGpa = 0;
    double avgWRate = 0;
    char* instructor;
    char* honors;

    int i = 0;
    char* token = strtok(line, ",");

    while (token && i < numFields) {
        switch (i) {
            case 0:
                crs = strdup(token);
                break;
            case 1:
                crsNum = strdup(token);
                strtok(NULL, ",");
                break;
            case 2:
                crsTitle = strdup(token);
                break;
            case 3:
                for (int j = i; j <= 7; j++) {
                    avgGpa += atoi(token) / (7 - j);
                    strtok(NULL, ",");
                }
                if (atoi(strtok(NULL, ",")) != 0 || atoi(strtok(NULL, ",")) != 0 ) {
                    return NULL;
                }
                break;
            case 4:
                avgWRate = atoi(token);
                break;
            case 5:
                instructor = checkQuotes(line, token);
                break;
            case 6:
                honors = strdup(token);
                break; 
        }
        char* token = strtok(NULL, ",");
        i++;
    }
    return createCourse(crs, crsNum, crsTitle, avgGpa, avgWRate, instructor, honors);
}
int main() {

    return 0;
}