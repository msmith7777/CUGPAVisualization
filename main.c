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
    int count;

    struct Course *next;


}Course;

/*
Create a struct. Arguements, in order, are course, course number, course title, total gpa, 
total withdraw rate, and instructor name 
*/
Course *createCourse(char *str, char *str2, char *str3, double gpa, double wRate, char *str4) {
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
    
    newNode -> avgGpa = gpa;
    newNode -> avgWRate = wRate;
    newNode -> count = 1;

    newNode -> next = NULL;

    return newNode;
}

/*
Compare data in 2 Course structs. If course and instructor name is the same, return 0. 
If just course name is the same, return 1. If neither, return 2.
*/
int crscmp(Course *crs1, Course *crs2) {
    int titleCmp = strcmp(crs1->crsTitle, crs2->crsTitle);
    int instrCmp = strcmp(crs1->instructor, crs2->instructor);

    if (titleCmp == 0 && instrCmp == 0){
        return 0;
    } else if (titleCmp == 0) {
        return 1;
    }
    return 2;
}

//Merge 2 entries of a Course struct by averaging values. The 2nd course is the new one.
void mergeCourses(Course *crs1, Course *crs2) {
    crs1 -> count += 1;
    crs1 -> avgGpa = (crs1 -> avgGpa * (crs1 -> count - 1) + crs2 -> avgGpa) / (crs1 -> count);
    crs1 -> avgWRate = (crs1 -> avgWRate * (crs1 -> count - 1) + crs2 -> avgWRate) / (crs1 -> count);
    free(crs2);
    return;
}

//Insert one node between a given node and it's neighbor
void insertNode(Course *crs1, Course *crs2) {
    Course *nextnext = crs1 -> next;
    crs1 -> next = crs2;
    crs2 -> next = nextnext;
    return;
}

//Add node to linked list
void addNode(Course **head, Course *node) {
    if (*head == NULL) {
        printf("List is empty. Created new head.\n");
        *head = node;
        return;
    }

    Course *current = *head;
    Course *save = *head;
    //Iterate through the linked list
    while (current != NULL) {
        int crsCmp = crscmp(current, node);
    //If the course has the same name and instructor as one in the list, merge the entries
        if (crsCmp == 0) {
            mergeCourses(current, node);
            return;
        }

    /*
    If the course has the same course name, keep checking through the group.
    If the instructor is there, merge the entries. Otherwise, just put it at the end.
    */
        if (crsCmp == 1) {
            while (current -> next != NULL || crscmp(current, current -> next) != 1) {
                if (crscmp(current, node) == 0) {
                    mergeCourses(current, node);
                }
            }
            insertNode(current, node);
            return;
        }
        save = current;
        current = current -> next;
    }
    save -> next = node;
    node -> next = NULL;
    return;
}

int main() {

    return 0;
}