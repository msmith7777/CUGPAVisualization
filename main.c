#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Create struct to represent wanted data
typedef struct Course{

    char* crs;
    char* crsNumber;
    char* crsTitle;
    double avgAs;
    double avgBs;
    double avgCs;
    double avgDs;
    double avgFs;
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
        head = save;
    }
}

/*
Create a struct. Arguements, in order, are course, course number, course title, percenteges of
each letter grad, total gpa, total withdraw rate, instructor name, and honors ("H\n" for honors, "\n" if not"
*/
Course* createCourse(char* str, char* str2, char* str3, double AS, double BS, double CS, double DS, double FS, double gpa, double wRate, char* str4, char* str5) {
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

    // Check if any strdup failed
    if (!newNode->crs || !newNode->crsNumber || !newNode->crsTitle || !newNode->instructor || !newNode->honors) {
        freeCrs(newNode);
        return NULL;
    }
    
    newNode -> avgAs = AS;
    newNode -> avgBs = BS;
    newNode -> avgCs = CS;
    newNode -> avgDs = DS;
    newNode -> avgFs = FS;
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

//Average a series by adding a new value to a previously calculated average.
double runningAverage(double avg, double newVal, int count) {
    return (avg * count + newVal) / (count + 1);
}

/*
Merge 2 entries of a Course struct by averaging values. The 2nd course is the new one. !!DO NOT
CALL THIS ON A COURSE IN THE LIST!!
*/
void mergeCourses(Course* crs1, Course* crs2) {
    crs1 -> avgAs = runningAverage(crs1 -> avgAs, crs2 -> avgAs, crs1 -> count);
    crs1 -> avgBs = runningAverage(crs1 -> avgBs, crs2 -> avgBs, crs1 -> count);
    crs1 -> avgCs = runningAverage(crs1 -> avgCs, crs2 -> avgCs, crs1 -> count);
    crs1 -> avgDs = runningAverage(crs1 -> avgDs, crs2 -> avgDs, crs1 -> count);
    crs1 -> avgFs = runningAverage(crs1 -> avgFs, crs2 -> avgFs, crs1 -> count);
    crs1 -> avgGpa = runningAverage(crs1 -> avgGpa, crs2 -> avgGpa, crs1 -> count);
    crs1 -> avgWRate = runningAverage(crs1 -> avgWRate, crs2 -> avgWRate, crs1 -> count);

    crs1 -> count += 1;

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
            Course* nodeOfInterest;
            while (current != NULL && crscmp(current, node) <= 1) {
                if (crscmp(current, node) == 0) {
                    mergeCourses(current, node);
                    return;
                } else if (current -> avgGpa > node -> avgGpa) {
                    nodeOfInterest = current;
                }
                current = current -> next;
            }
            insertNode(nodeOfInterest, node);
            return;
        }
        save = current;
        current = current -> next;
    }
    save -> next = node;
    node -> next = NULL;
}

/*
Create a token seperated by commas from a line. If quotes are included, ignore commas within quotes.
Return a pointer to the token. firstTime = 1 means it's the first time, anything else means it's not.
*/
char* tokenize(char* line, int firstTime) {
    char* token = line;
    if (firstTime = 1) {
        token = strtok(line, ",");
    } else token = strtok(NULL, ",");

    if (line[0] == '"') {
        char* buff;
        strcat(buff, token);

        while (buff[strlen(buff) - 1] != '"') {
            token = strtok(NULL, ",");
            strcat(buff, token);
        }

        return buff;
    }

    return token;
}

/*
Argument is a line from the CSV. Extract useful information from the line, assign a Course to that data, 
and return the pointer to the course. If class is pass-fail, return NULL.
*/
Course* parseCSVV(char* line) {
    char* tokens[14];
    int tokenCount = 0;

    tokens[0] = tokenize(line, 1);

    for (int i = 1; i < 14; i++) {
        tokens[i] = tokenize(line, 0);
    }
    //Check if class is pass/fail
    if (strcmp(tokens[9], "0%") != 0 || strcmp(tokens[10], "0%") != 0) {
        return NULL;
    }

    double as = atoi(tokens[4]);
    double bs = atoi(tokens[5]);
    double cs = atoi(tokens[6]);
    double ds = atoi(tokens[7]);
    double fs = atoi(tokens[8]);
    double wrate = atoi(tokens[11]);
    
    double gpa = as * 4 + bs * 3 + cs * 2 + ds;

    return createCourse(tokens[0], tokens[2], tokens[3], as, bs, cs, ds, fs, gpa, wrate, tokens[12], tokens[13])
}
int main() {

    return 0;
}