#include "gradient.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

int randint(int n)
{
    if ((n - 1) == RAND_MAX)
    {
        return rand();
    }
    else
    {
        assert(n <= RAND_MAX);

        int end = RAND_MAX / n; // truncate skew
        assert(end > 0);
        end *= n;

        int r;
        while ((r = rand()) >= end)
            ;

        return r % n;
    }
}

path_point randomPoints(path_point center, int x, int y)
{
    x = randint(20);
    y = randint(20);
    // printf("Random (X= %d, Y= %d)\n", x, y);
    path_point points = {
        .x = center.x + (x - VIEW_RADIUS),
        .y = center.y + (y - VIEW_RADIUS)};
    // printf("Random Returned (X= %d, Y= %d)\n", points.x, points.y);
    return points;
}

void pickMaxPoints(float (*view)[VIEW_SIZE], float *peakVal, float *prevPeakVal, path_point *peakCoord, path_point *center)
{
    srand(time(NULL));
    // get view
    // traverse view abd find max point
    // return max point || set max in the method
    // looping through view to find largest number and setting it as peak
    int row, col;
    for (row = 0; row < VIEW_SIZE; row++)
    {
        for (col = 0; col < VIEW_SIZE; col++)
        {
            if (view[row][col] > *peakVal)
            {
                *peakVal = view[row][col];
                *peakCoord = randomPoints(*center, col, row);
            }
        }
    }
    prevPeakVal = peakVal;
    printf("\nMax number at position (X= %d, Y=%d) Value= %.2f \n", peakCoord->x, peakCoord->y, *peakVal);
    printf("\nPrevious Peak Value= %.2f \n", *prevPeakVal);

    // return peakPoint;
}

path_point find_highest_point()
{

    float view[VIEW_SIZE][VIEW_SIZE];

    float previousMax = -1;
    float currentMax = 0;
    int i = 0, j = 0;

    path_point centerViewPoints = {.x = VIEW_RADIUS, .y = VIEW_RADIUS};
    // path_point rPoint = randomPoints(centerViewPoints, centerViewPoints.x, centerViewPoints.y);
    path_point rPoint = randomPoints(centerViewPoints, centerViewPoints.x, centerViewPoints.y);
    path_point peakPoint;

    // printf("Peak point co-ordinates %d %d \n", peakPoint.x, peakPoint.y);
    // rPoint = randomPoints(centerViewPoints, centerViewPoints.x, centerViewPoints.y);
    while (1)
    {

        srand(time(NULL));
        getchar();
        // printf("Peak point co-ordinates %d %d \n", peakPoint.x, peakPoint.y);
        generate_view(view, rPoint.y, rPoint.x);
        print_view(view);

        pickMaxPoints(view, &currentMax, &previousMax, &peakPoint, &centerViewPoints);
        if (currentMax == previousMax)
        {

            printf("\nHEYYYYYYYYYYYYYYYY\n");
            if (declare_peak(peakPoint.x, peakPoint.y) == 1)
            {
                printf("\nfound the peak\n");
                return peakPoint;
                break;
            }
            else
            {
                printf("\npeak not found\n");
                rPoint = randomPoints(centerViewPoints, centerViewPoints.x, centerViewPoints.y);
                //  peakPoint.x + 11;
                //  peakPoint.y + 11;
            }
        }
    }
    return peakPoint;
}
