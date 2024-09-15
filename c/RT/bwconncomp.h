#include <stdio.h>
#include <stdlib.h>

#define N 3201  // Adjust these according to your image dimensions
#define M 72    // Adjust these according to your image dimensions

#define MAX_COMPONENTS 10000  // Define a maximum number of components (arbitrary large number)
#define MAX_PIXELS 10000      // Define maximum pixels in a component

// Struct to store connected components
typedef struct {
    int **pixelIdxList;  // List of pixel indices in each component
    int numObjects;      // Number of components
    int imageSize[2];    // Size of the image
    int connectivity;    // Connectivity type (in this case, it's 4)
} ConnectedComponents;

// 4-connectivity directions (up, down, left, right)
int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Flood fill (DFS) to label connected pixels
void floodFill(int *BW, int rows, int cols, int x, int y, int componentId, int *labels, int *componentPixels, int *pixelCount) {
    // Create a stack for DFS
    int stack[MAX_PIXELS][2];
    int top = -1;

    // Push the starting pixel to the stack
        top++;
    
    stack[top][0] = x;
    stack[top][1] = y;

    while (top >= 0) {
        int cx = stack[top][0];
        int cy = stack[top][1];
        top--;

        // Mark the current pixel with the componentId
        labels[cx * M + cy] = componentId;

        // Store the pixel in the component list (convert to 1D index)
        componentPixels[(*pixelCount)++] = cx * cols + cy;

        // Explore the 4 neighbors (4-connectivity)
        for (int i = 0; i < 4; i++) {
            int nx = cx + directions[i][0];
            int ny = cy + directions[i][1];

            // Check if the neighbor is inside the image bounds and is part of the object (BW == 1) and not labeled yet
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && BW[nx * M + ny] == 1 && labels[nx * M + ny] == 0) {
                top++;
                stack[top][0] = nx;
                stack[top][1] = ny;
            }
        }
    }
}

// bwconncomp function to find connected components with 4-connectivity
ConnectedComponents *bwconncomp(int *BW, int rows, int cols) {
    ConnectedComponents *CC = (ConnectedComponents *)malloc(sizeof(ConnectedComponents));
    CC->numObjects = 0;
    CC->imageSize[0] = rows;
    CC->imageSize[1] = cols;
    CC->connectivity = 4;

    int *labels = (int *)malloc(M * N * sizeof(int *));

    // Pixel index list for each component
    CC->pixelIdxList = (int **)malloc(MAX_COMPONENTS * sizeof(int *));

    // Initialize the componentId (starting from 1)
    int componentId = 1;

    // Loop through the image to find connected components
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // If the pixel is part of an object and not labeled yet
            if (BW[i * M + j] == 1 && labels[i * M + j] == 0) {
                // Array to store pixel indices for the current component
                int *componentPixels = (int *)malloc((M * N + 1) * sizeof(int));
                int pixelCount = 0;

                // Call floodFill to mark the connected component
                floodFill(BW, rows, cols, i, j, componentId, labels, componentPixels, &pixelCount);

                // Store the pixel list for this component
                componentPixels[pixelCount] = -1;  // Mark the end of the component
                CC->pixelIdxList[CC->numObjects] = componentPixels;
                CC->numObjects++;
                componentId++;
            }
        }
    }

    // Free the label matrix
    //for (int i = 0; i < rows; i++) {
   //     free(labels[i]);
   // }
    free(labels);

    return CC;
}
/*
// Main function
int main1() {
    // Example binary image (3x3)
    int *image[3];
    int img_data[3][3] = {{1, 1, 0}, {0, 0, 0}, {1, 0, 0}};
    for (int i = 0; i < 3; i++) {
        image[i] = img_data[i];
    }

    // Call bwconncomp
    ConnectedComponents *CC = bwconncomp(image, 3, 3);

    // Output the results
    printf("Number of components: %d\n", CC->numObjects);
    for (int i = 0; i < CC->numObjects; i++) {
        printf("Component %d: ", i + 1);
        for (int j = 0; CC->pixelIdxList[i][j] != -1; j++) {
            printf("(%d, %d) ", CC->pixelIdxList[i][j] / CC->imageSize[1] + 1, CC->pixelIdxList[i][j] % CC->imageSize[1] + 1);  // MATLAB-like 1-based index
        }
        printf("\n");
    }

    // Free memory
    for (int i = 0; i < CC->numObjects; i++) {
        free(CC->pixelIdxList[i]);
    }
    free(CC->pixelIdxList);
    free(CC);

    return 0;
}
*/