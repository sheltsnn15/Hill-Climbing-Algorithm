#include "gradient.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

/**
 * @author Shelton Ngwenya
 * SNo. R00203947
 * 
 */

/**
 * @brief Steepest-Ascent Hill Climbing Algorithm
 * Step 1:
 * Evaluate the initial state.
 *      if is equal to goal state
 *          return success
 *      else
 *          set initial state to current state
 *
 * Step 2:
 * Repeat step 1 until solution is found or,
 * initial state does not change
 *      a) select a state that has not been yet applied to current state
 *      b) initialise a new 'best state' equal to current state and apply it to produce a new state
 *      c) evaluate new state
 *          if the current state is a goal state
 *              stop
 *              return success
 *          if it's better than the best state
 *              make it best state
 *          else
 *              continue loop with another new state
 *      d) make best state as current state, goto step 2(b)
 *
 *      EXAMINE ALL NEIGHBORING NODES AND SELECT THE NODE CLOSEST TO THE SOLUTION STATE AS OF PATH_POINT
 * Step 3 : Exit
 *
 * Solutions For Plateau
 * Backtrack to one of the previous states and explore other directions
 */

/**
 * @brief Get the real point from landscape
 *
 * @param center
 * @param x
 * @param y
 * @return path_point
 */
path_point get_landcape_co_ord(path_point center, int x, int y)
{
    path_point co_ord = {
        .x = center.x + (x - VIEW_RADIUS),
        .y = center.y + (y - VIEW_RADIUS)};

    return co_ord;
}

/**
 * @brief Method to compare centered peek to its neighbour
 *
 * @param current
 * @param neighbor
 * @param pv
 * @param nv
 * @return int
 */
int compare_neighbor(path_point *current, path_point *neighbor, float pv, float nv)
{
    if (pv == nv)
    {
        printf("Max %0.2f at (x='%d', y='%d') equals Neighbor %0.2f at (x='%d', y='%d')\n", pv, current->x, current->y, nv, neighbor->x, neighbor->y);
        return 1;
    }
    else
    {
        printf("Max %0.2f at (x='%d', y='%d') not equals Neighbor %0.2f at (x='%d', y='%d')\n", pv, current->x, current->y, nv, neighbor->x, neighbor->y);
        return 0;
    }
}

/**
 * @brief method to check if all points are -1
 *
 * @param arr
 * @param size
 * @return int
 */
int out_of_bonds_check(float (*view)[VIEW_SIZE], const int size)
{
    int oob = -1;
    int counter = 0;
    for (int i = 0; i < VIEW_SIZE; i++)
    {
        for (int k = 0; k < VIEW_SIZE; k++)
        {
            if (view[i][k] == oob)
                counter++;
        }
    }

    if (counter == 121)
        return 1;
    else
        return 0;
}

/**
 * @brief method to swap float points
 *
 * @param a
 * @param b
 */
void swap(float *a, float *b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief method to reverse rows in array
 *
 * @param view
 */
void reverse_rows(float (*matrix)[VIEW_SIZE])
{
    printf("ROWS FLIP------------------------\n");

    printf("BEFORE------------------------------------------------\n");
    print_view(matrix);
    printf("------------------------------------------------------\n");
    // Traverse each row of arr[][]
    for (int i = 0; i < VIEW_SIZE; i++)
    {

        // Initialise start and end index
        int start = 0;
        int end = VIEW_SIZE - 1;

        // Till start < end, swap the element
        // at start and end index
        while (start < end)
        {

            // Swap the element
            swap(&matrix[i][start], &matrix[i][end]);

            // Increment start and decrement
            // end for next pair of swapping
            start++;
            end--;
        }
    }
    printf("AFTER-------------------------------------------------\n");
    print_view(matrix);
    printf("------------------------------------------------------\n");
}

/**
 * @brief method to reverse columns in array
 *
 * @param view
 */
void reverse_cols(float (*matrix)[VIEW_SIZE])
{
    printf("COLUMN FLIP------------------------\n");

    printf("BEFORE------------------------------------------------\n");
    print_view(matrix);
    printf("------------------------------------------------------\n");
    // Traverse each column of arr[][]
    for (int i = 0; i < VIEW_SIZE; i++)
    {
        // Initialise start and end index
        int start = 0;
        int end = VIEW_SIZE - 1;

        // Till start < end, swap the
        // element at start and end index
        while (start < end)
        {
            // Swap the element
            swap(&matrix[start][i], &matrix[end][i]);

            // Increment start and decrement
            // end for next pair of swapping
            start++;
            end--;
        }
    }

    printf("AFTER-------------------------------------------------\n");
    print_view(matrix);
    printf("------------------------------------------------------\n");
}

/**
 * @brief method to add traversed path points to path point array
 *
 * @param path_point_array
 * @param path_point
 */
void add_traversed_path_point(path_point *ppa, path_point pp, int size)
{
    ppa[size].x = pp.x;
    ppa[size].y = pp.y;
}

/**
 * @brief method to print contents in path_point_arr
 *
 * @param ppa
 * @param size
 */
void print_traversed_path_points(path_point *ppa, int n)
{
    printf("\n--Added to traversed arr---\n");
    int i;
    for (i = 0; i < n; i++)
    {
        if ((ppa[i].x != 0) && (ppa[i].y != 0))
        {
            printf("Point(%d, %d)\n", ppa[i].x, ppa[i].y);
        }
    }
}

/**
 * @brief method to get the last traversed path point object
 *
 * @param path_point_array
 * @return path_point
 */
path_point get_last_traversed_path_point(path_point *ppa, int *counter)
{
    int x, y;
    int size = *counter;
    //printf("Last Point Arr Size (%d)\n", size);
    path_point last_path_point = {
        .x = ppa[size-1].x,
        .y = ppa[size-1].y
    };
    //printf("Last point(x='%d', y='%d')\n", last_path_point.x, last_path_point.y);
    return last_path_point;
}

path_point find_highest_point()
{
    // view
    float view[VIEW_SIZE][VIEW_SIZE];
    // the radius of the view
    path_point center = {.x = VIEW_RADIUS, .y = VIEW_RADIUS};
    // set the peak radius
    path_point peak = center;
    int counter = 0, row, col;

    path_point path_point_array[1000];
    path_point last_point, neighbor;
    int backtracking = 0;

    while (true)
    {
        if (!backtracking){
            counter++;
            generate_view(view, peak.y, peak.x);
        }else{
            backtracking = 0;
        }
        // printf("View at (x='%d', y='%d')\n", peak.x, peak.y);
        //print_view(view);
        center = peak;
        // setting the value at the index of peak to -1 by default
        float peak_value = -1, neighbor_val = 0;

        // looping through view to find largest number and setting it as peak
        for (row = 0; row < VIEW_SIZE; row++)
        {
            for (col = 0; col < VIEW_SIZE; col++)
            {
                if (view[row][col] > peak_value)
                {
                    peak_value = view[row][col];
                    neighbor_val = view[row][col + 1];  // warning
                    peak = get_landcape_co_ord(center, col, row);
                    neighbor = get_landcape_co_ord(center, col, row + 1);
                }
            }
        }

        add_traversed_path_point(path_point_array, peak, counter);

        //print_traversed_path_points(path_point_array, counter + 1);

        if (declare_peak(peak.x, peak.y) == 1)
            return peak;
        else
        {
            if ((out_of_bonds_check(view, VIEW_SIZE * VIEW_SIZE) == 1) || (compare_neighbor(&peak, &neighbor, peak_value, neighbor_val) == 1))
            {
                last_point = get_last_traversed_path_point(path_point_array, &counter);
                counter--;
                printf("Last point (%d, %d)\n", last_point.x, last_point.y);
                row = last_point.x;
                col = last_point.y;
                //peak = last_point;
                //center = peak;
                //peak = get_landcape_co_ord(last_point, last_point.x, last_point.x);
                //printf("New peek point (%d, %d)\n", peak.x, peak.y);
                //printf("New peek point (%d, %d)\n", peak.x, peak.y);
                //reverse_cols(view);
                generate_view(view, col, row);
                reverse_rows(view);
                backtracking = 1;
            }
        }
    }
    return peak;
}
