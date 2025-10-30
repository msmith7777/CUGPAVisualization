#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Create struct to represent wanted data
typedef struct Course{

    char crs[10];
    char crsNumber[10];
    char crsTitle[64];
    double avgGpa;
    double avgWRate;
    char instructor[64];
    char honors[5];
    int count;

    struct Course *next;


}Course;

/*
Create a struct. Arguements, in order, are course, course number, course title, total gpa, 
total withdraw rate, instructor name, and whether it's honors
*/
Course *createCourse(char *str, char *str2, char *str3, double gpa, double wRate, char *str4, char *str5) {
    Course *newNode = malloc(sizeof(Course));
    if (newNode == NULL) {
        printf("Malloc failed.\n");
        exit(1);
    }

    strncpy(newNode -> crs, str, sizeof(newNode -> crs) - 1);
    newNode -> crs[sizeof(newNode -> crs) - 1] = '\0';

    strncpy(newNode -> crsNumber, str2, sizeof(newNode -> crsNumber) - 1);
    newNode -> crsNumber[sizeof(newNode -> crsNumber) - 1] = '\0';

    strncpy(newNode -> crsTitle, str3, sizeof(newNode -> crsTitle) - 1);
    newNode -> crsTitle[sizeof(newNode -> crsTitle) - 1] = '\0';

    strncpy(newNode -> instructor, str4, sizeof(newNode -> instructor) - 1);
    newNode -> instructor[sizeof(newNode -> instructor) - 1] = '\0';

    strncpy(newNode -> honors, str5, sizeof(newNode -> honors) - 1);
    newNode -> honors[sizeof(newNode -> honors) - 1] = '\0';
    
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
int crscmp(Course *crs1, Course *crs2) {
    int numCmp = strcmp(crs1 -> crs, crs2 -> crs);
    int crsNumber = strcmp(crs1 -> crsNumber, crs2 -> crsNumber);
    int instrCmp = strcmp(crs1 -> instructor, crs2 -> instructor);
    int hnrsCmp = strcmp(crs1 -> honors, crs2 -> honors);

    if (numCmp == 0 && crsNumber == 0 && instrCmp == 0 && hnrsCmp == 0){
        return 0;
    } else if (numCmp == 0 && crsNumber == 0) {
        return 1;
    }
    return 2;
}

/*Merge 2 entries of a Course struct by averaging values. The 2nd course is the new one. !!DO NOT
CALL THIS ON A COURSE IN THE LIST!!
*/
void mergeCourses(Course *crs1, Course *crs2) {
    crs1 -> count += 1;
    crs1 -> avgGpa = (crs1 -> avgGpa * (crs1 -> count - 1) + crs2 -> avgGpa) / (crs1 -> count);
    crs1 -> avgWRate = (crs1 -> avgWRate * (crs1 -> count - 1) + crs2 -> avgWRate) / (crs1 -> count);
    free(crs2);
}

//Insert one node between a given node and it's neighbor
void insertNode(Course *crs1, Course *crs2) {
    Course *nextnext = crs1 -> next;
    crs1 -> next = crs2;
    crs2 -> next = nextnext;
}

// Merge node with a matching course or group it with others of the same course and number
void addNode(Course **head, Course *node) {
    if (*head == NULL) {
        *head = node;
        return;
    }

    Course *current = *head;
    Course *save = *head;
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

/*Argument is a line from the CSV. Extract useful information from the line, assign a Course to
that data, and add the Course to the linked list
*/
void parseCSV(char *line) {
    
}
int main() {

    return 0;
}