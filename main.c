#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Create struct to represent wanted data
typedef struct Course{

    char* crs;
    char* crsNumber;
    char* crsTitle;
    char* instructor;
    char* honors;
    double avgAs, avgBs, avgCs, avgDs, avgFs, avgGpa, avgWRate;
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
            Course* insertAfter = save;

            while (current != NULL && crscmp(current, node) <= 1) {
                if (crscmp(current, node) == 0) {
                    mergeCourses(current, node);
                    return;
                } else if (current -> avgGpa > node -> avgGpa) {
                    insertAfter = current;
                }
                current = current -> next;
            }
            insertNode(insertAfter, node);
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
    char buff[1024];
    if (firstTime == 1) {
        token = strtok(line, ",");
    } else token = strtok(NULL, ",");

    if (token == NULL) return NULL;

    if (token[0] == '"') {
    //Skip opening quote
    strcpy(buff, token + 1);
    
    while (buff[strlen(buff) - 1] != '"' && (token = strtok(NULL, ","))) {
        strcat(buff, ",");
        strcat(buff, token);
    }
    
    // Remove closing quote if present
    if (buff[strlen(buff) - 1] == '"') {
        buff[strlen(buff) - 1] = '\0';
    }
    
    char* result = malloc(strlen(buff) + 1);
    strcpy(result, buff);
    return result;
}

    return strdup(token);
}

/*
Argument is a line from the CSV. Extract useful information from the line, assign a Course to that data, 
and return the pointer to the course. If class is pass-fail, return NULL.
*/
Course* parseLine(char* line) {
    char* tokens[14];

    tokens[0] = tokenize(line, 1);

    if (tokens[0] == NULL) return NULL;

    // Skip header line
    if (strcmp(tokens[0], "Course") == 0) {
        free(tokens[0]);
        return NULL;
    }

    for (int i = 1; i < 14; i++) {
        tokens[i] = tokenize(line, 0);
        
        if (tokens[i] == NULL) {
            // If it's the honors field (last field), use empty string
            if (i == 13) {
                tokens[i] = strdup("");
                break;
            }
            // Otherwise it's a malformed line
            for (int j = 0; j < i; j++) {
                free(tokens[j]);
            }        
            return NULL;
        }
    }
    //Remove newlines from tokens.
    for (int i = 0; i < 14; i++) {
    size_t len = strlen(tokens[i]);
    if (len > 0 && tokens[i][len - 1] == '\n') {
        tokens[i][len - 1] = '\0';
    }
    }
    //Classes are not pass/fail if they are entered as 0% in the CSV. If they are, they should be skipped
    if (strcmp(tokens[9], "0%") != 0 || strcmp(tokens[10], "0%") != 0) {
        for (int i = 0; i < 14; i++) {
                free(tokens[i]);
            }     
        return NULL;
    }

    double as = atof(tokens[4]);
    double bs = atof(tokens[5]);
    double cs = atof(tokens[6]);
    double ds = atof(tokens[7]);
    double fs = atof(tokens[8]);
    double wrate = atof(tokens[11]);
    
    double gpa = (as/100.0)*4 + (bs/100.0)*3 + (cs/100.0)*2 + (ds/100.0)*1;

    Course* result = createCourse(tokens[0], tokens[1], tokens[3], as, bs, cs, ds, fs, gpa, wrate, tokens[12], tokens[13]);

    for (int i = 0; i < 14; i++) {
                free(tokens[i]);
            }
            
    return result;
}

//Creates a linked list from the CSV already opened in r mode. Returns a pointer to the head
Course* createLL(FILE* file) {
    char buff[1024];
    Course* head = NULL;

    //Skip header line
    fgets(buff, 1024, file);

    Course* current;

    while(fgets(buff, 1024, file) != NULL) {
        current = parseLine(buff);
        if (current == NULL) continue;
        addNode(&head, current);
    }

    return head;
}

//Print the linked list to an already opened file in w mode and properly free the data.
void printLL(Course* head, FILE* file) {
fprintf(file, "[\n");

    Course* current = head;
    while (current != NULL) {
        fprintf(file, "  {\n");
        fprintf(file, "    \"course\": \"%s\",\n", current -> crs);
        fprintf(file, "    \"courseNumber\": \"%s\",\n", current -> crsNumber);
        fprintf(file, "    \"title\": \"%s\",\n", current -> crsTitle);
        fprintf(file, "    \"instructor\": \"%s\",\n", current -> instructor);
        fprintf(file, "    \"honors\": %s,\n", (strcmp(current->honors, "H") == 0) ? "true" : "false");
        fprintf(file, "    \"avgAs\": %.2f,\n", current -> avgAs);
        fprintf(file, "    \"avgBs\": %.2f,\n", current -> avgBs);
        fprintf(file, "    \"avgCs\": %.2f,\n", current -> avgCs);
        fprintf(file, "    \"avgDs\": %.2f,\n", current -> avgDs);
        fprintf(file, "    \"avgFs\": %.2f,\n", current -> avgFs);
        fprintf(file, "    \"avgGpa\": %.2f,\n", current -> avgGpa);
        fprintf(file, "    \"avgWRate\": %.2f,\n", current -> avgWRate);
        fprintf(file, "    \"numSections\": %d\n", current -> count);
        fprintf(file, "  }");

        if (current->next != NULL)
            fprintf(file, ",");
        fprintf(file, "\n");

        current = current->next;
    }

    fprintf(file, "]\n");
}

int main() {
    FILE* inputFile = fopen("csv_files/combined.csv", "r");
    if (inputFile == NULL) {
        printf("Error: Could not open combined.csv\n");
        return 1;
    }

    Course* head = createLL(inputFile);
    fclose(inputFile);

    FILE* outputFile = fopen("output.json", "w");
    if (outputFile == NULL) {
        printf("Error: Could not open output.json\n");
        freeList(head);
        return 1;
    }

    printLL(head, outputFile);
    fclose(outputFile);

    freeList(head);

    printf("Done! Check output.json\n");
    return 0;
}