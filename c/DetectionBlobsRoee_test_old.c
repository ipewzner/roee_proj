#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LABELS 10000
#define EPSILON 1e-6  // Tolerance for floating-point comparison

#define N 3201  // Adjust these according to your image dimensions
#define M 72    // Adjust these according to your image dimensions

// Function to label connected components in a binary image
void labelComponents(const int* binaryMap, int* labeledArray, int rows, int cols, int* numLabels) {
    // Initialize labeledArray with zeros
    memset(labeledArray, 0, rows * cols * sizeof(int));

    // Simple labeling algorithm (not optimized, could be replaced with a more sophisticated one)
    int label = 1;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (binaryMap[r * cols + c] == 1 && labeledArray[r * cols + c] == 0) {
                // Flood fill
                int stackSize = rows * cols;
                int* stack = (int*)malloc(stackSize * 2 * sizeof(int));
                int top = -1;

                stack[++top] = r;
                stack[++top] = c;
                labeledArray[r * cols + c] = label;

                while (top >= 0) {
                    int currR = stack[top--];
                    int currC = stack[top--];

                    // 4-connectivity
                    for (int dr = -1; dr <= 1; dr++) {
                        for (int dc = -1; dc <= 1; dc++) {
                            if ((dr == 0 || dc == 0) && (dr != 0 || dc != 0)) {
                                int newR = currR + dr;
                                int newC = currC + dc;

                                if (newR >= 0 && newR < rows && newC >= 0 && newC < cols) {
                                    if (binaryMap[newR * cols + newC] == 1 && labeledArray[newR * cols + newC] == 0) {
                                        labeledArray[newR * cols + newC] = label;
                                        stack[++top] = newR;
                                        stack[++top] = newC;
                                    }
                                }
                            }
                        }
                    }
                }

                free(stack);
                label++;
            }
        }
    }
    *numLabels = label - 1;  // Total number of labels assigned
}

// Function to detect blobs
void DetectionBlobsRoee(const int* binaryMap, int rows, int cols, int min_pixel, int max_pixel, int* ReportCL, int* numReport) {
    int* labeledArray = (int*)malloc(rows * cols * sizeof(int));
    int numLabels;
    labelComponents(binaryMap, labeledArray, rows, cols, &numLabels);

    printf("*******---*******");
    for (size_t i = 0; i < numLabels; i++) {
        printf("\n%d) %d", i, labeledArray[i]);
    }
    for (size_t i = 0; i < N * M; i++) {
        if (binaryMap[i] == 1) {
            printf("\n%d) %d", i, binaryMap[i]);
        }
    }

    int* CL = (int*)malloc(numLabels * sizeof(int));

    for (int i = 0; i < numLabels; i++) {
        CL[i] = 999;
    }

    int NumObjects = 0;

    for (int label = 1; label <= numLabels; label++) {
        int numOfPix = 0;
        int* row = (int*)malloc(rows * cols * sizeof(int));
        int* col = (int*)malloc(rows * cols * sizeof(int));

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (labeledArray[r * cols + c] == label) {
                    row[numOfPix] = r + 1;
                    col[numOfPix] = c + 1;
                    numOfPix++;
                }
            }
        }

        if (numOfPix >= min_pixel && numOfPix <= max_pixel) {
            int sumRow = 0;
            for (int i = 0; i < numOfPix; i++) {
                sumRow += row[i];
            }
            CL[label - 1] = sumRow / numOfPix;
            NumObjects++;
        }

        free(row);
        free(col);
    }

    int j = 0;
    for (int i = 0; i < numLabels; i++) {
        if (CL[i] != 999) {
            ReportCL[j++] = CL[i];
        }
    }
    printf("Number of Objects: %d\n", NumObjects);
    *numReport = NumObjects;

    free(labeledArray);
    free(CL);
}

/*
// Function to load CSV data into an array
void loadCSV(const char* filename, int* array, int* rows, int* cols) {
    printf("Loading %s...\n", filename);

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
         printf("Error opening file: %s\n", filename);

        exit(EXIT_FAILURE);
    }

    int r = 0, c = 0;
    while (fscanf(file, "%lf,", &array[r * (*cols) + c]) == 1) {
        c++;
        if (c == *cols) {
            c = 0;
            r++;
        }
    }

    *rows = r;
    *cols = c ? c : *cols;  // Adjust columns if it's the last line
    fclose(file);
}
*/

void loadCSV(const char* filename, int* arr, int* size) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    printf("size: %d\n", *size);
    char line[1024];
    int i = 0, j = 0;
    while (fgets(line, 1024, fp) != NULL && i < *size) {
        char* token = strtok(line, ",");
        while (token != NULL && j < *size) {
            arr[j] = atoi(token);
            // printf("\n %d) %d ", j,arr[j]);
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }

    fclose(fp);
}

// Function to read binary map from CSV
void ReadCSVInt(const char* filename, int* array) {
    printf("Loading %s...\n", filename);

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (fscanf(file, "%d,", &array[i * M + j]) != 1) {
                perror("Failed to read data from file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            /*
            if (i > (N - 10) && (j < 2 || j > 69)) {
                printf("\n%d,%d) %d", i, j, array[i * M + j]);
            }
            if (i == 2733 && (j < 2 || j > 69)) {
                printf("\n%d,%d) %d", i, j, array[i * M + j]);
            }
            */
        }
    }

    fclose(file);
}

// Function to check if two arrays are approximately equal
int arraysAreEqual(int* arr1, int* arr2, int size) {
    printf("Checking if arrays are equal...\n");
    printf("size: %d\n", size);
    for (int i = 0; i < size; i++) {
        printf("\n%d,%d) %d", i, arr1[i], arr2[i]);

        if (fabs(arr1[i] - arr2[i]) > EPSILON) {
            return 0;  // Arrays are not equal
        }
    }
    return 1;  // Arrays are equal
}

// Function to test DetectionBlobsRoee
void test_DetectionBlobsRoee() {
    // Load expected data
    int size = 24;           // this test is 24
    int rows = N, cols = M;  // Initialize rows and cols to the correct values
    int* expected = (int*)malloc(MAX_LABELS * sizeof(int));
    int* binaryMap = (int*)malloc(N * M * sizeof(int));

    ReadCSVInt("C:/Users/ipewz/Desktop/roee_proj/forTest/bm.csv", binaryMap);
    loadCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/CL.csv", expected, &size);

    for (int i = 0; i < 24; i++) {
        printf("%d ", expected[i]);  // Correct format specifier for int
    }

    printf("rows: %d, cols: %d\n", rows, cols);

    // Allocate memory for result
    int* result = (int*)malloc(MAX_LABELS * sizeof(int));

    // Call DetectionBlobsRoee
    int numResult;
    DetectionBlobsRoee(binaryMap, rows, cols, 1, 10000, result, &numResult);

    for (int i = 0; i < numResult; i++) {
        printf("%d ", expected[i]);  // Correct format specifier for int
    }
    for (int i = 0; i < numResult; i++) {
        printf("%d ", result[i]);  // Correct format specifier for int
    }

    // Compare results
    if (arraysAreEqual(result, expected, numResult)) {
        printf("Test passed\n");
    } else {
        printf("Test failed\n");
        printf("Expected:\n");
        for (int i = 0; i < numResult; i++) {
            printf("%d ", expected[i]);  // Correct format specifier for int
        }
        printf("\n");
        printf("Result:\n");
        for (int i = 0; i < numResult; i++) {
            printf("%d ", result[i]);  // Correct format specifier for int
        }
        printf("\n");
    }

    // Free allocated memory
    free(expected);
    free(binaryMap);
    free(result);
}

int main() {
    test_DetectionBlobsRoee();
    return 0;
}

// gcc DetectionBlobsRoee_test.c -o DetectionBlobsRoee_test.exe -lm
//./DetectionBlobsRoee_test.exe