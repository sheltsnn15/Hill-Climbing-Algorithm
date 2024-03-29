#include "gradient.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

/**
 * @author Shelton Ngwenya
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
 * Solution 1:
 *      Take small steps to peak value
 *              Look at previous paths taken
 *              Sum the differences between all x coordinates and y coordinates visited
 *              Average out those differences and return this as the average direction
 * Solution 2:
 *      Backtrack to one of the previous states and explore other directions (Still busy with this implementation)
 *      Visit all corners of the view
 *      Look for a value greater than the current peak
 *      if not found
 *          backtrack
 *          Explore another direction
 *      else
 *          go to step 1 
 *      
 * (Seeds/landscapes struggling with: 27 = 276 average calls,
 *                                   92 = 1002 average calls,
 *                                   97 = 1002 average calls)
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
int compare_neighbor(path_point *current, path_point *neighbor, float current_value, float neighbor_value)
{
    if (current_value == neighbor_value)
    {
        // printf("Max %0.2f at (x='%d', y='%d') equals Neighbor %0.2f at (x='%d', y='%d')\n", pv, current->x, current->y, nv, neighbor->x, neighbor->y);
        return 1;
    }
    else
    {
        // printf("Max %0.2f at (x='%d', y='%d') not equals Neighbor %0.2f at (x='%d', y='%d')\n", pv, current->x, current->y, nv, neighbor->x, neighbor->y);
        return 0;
    }
}

/**
 * @brief method to explore different directions in view
 *
 * @param path_point
 * @param float
 * @return int
 */
path_point explore_other_directions(path_point *current, float current_value)
{
    //  arr to store x offsets from view radius
    int offset_x[] = {0, 0, VIEW_SIZE - 1, VIEW_SIZE - 1};
    // arr to store y offsets from view radius
    int offset_y[] = {0, VIEW_SIZE - 1, 0, VIEW_SIZE - 1};
    // Loop through the offsets
    for (int i = 0; i < 4; i++)
    {
        // get the i'th x offset
        int nx = current->x + offset_x[i];
        // get the i'th y offset
        int ny = current->y + offset_y[i];

        // return path points
        path_point co_ord = {.x = nx, .y = ny};

        return co_ord;
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
}

/**
 * @brief method to reverse columns in array
 *
 * @param view
 */
void reverse_cols(float (*matrix)[VIEW_SIZE])
{
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
path_point get_last_traversed_path_point(path_point *ppa, int *counter, int index)
{
    int x, y;
    int size = *counter;
    path_point last_path_point = {
        .x = ppa[size - index].x,
        .y = ppa[size - index].y};
    return last_path_point;
}

path_point calc_average_direction(path_point *ppa, int n)
{
    int x_mean, y_mean;
    int x_sum = 0, y_sum = 0;
    // Take an integer set path points of n values

    // loop through path point values in arr
    for (int i = 0; i < n; i++)
    {
        // Subtract the last point to the next to last point
        // Add all the differences of path points together
        x_sum += (ppa[i + 1].x - ppa[i].x);
        y_sum += (ppa[i + 1].y - ppa[i].y);
    }

    // Divide result by n
    x_mean = (int)x_sum / (float)n;
    y_mean = (int)y_sum / (float)n;

    // Result is mean of path points's values

    // return the path points (these will be used as the average)
    path_point average_direction = {
        .x = x_mean,
        .y = y_mean};
    return average_direction;
}

path_point find_highest_point()
{
    // view
    float view[VIEW_SIZE][VIEW_SIZE];
    // the radius of the view
    path_point center = {.x = VIEW_RADIUS, .y = VIEW_RADIUS};
    // set the peak radius
    path_point peak = center;
    int counter = 0, row, col, x_mean, y_mean;

    path_point path_point_array[1000];
    path_point last_point, neighbor;
    int backtracking = 0, back_track_counter = 0;

    while (true)
    {
        // getchar();
        if (!backtracking)
        {
            counter++;
            generate_view(view, peak.y, peak.x);
        }
        else
        {
            backtracking = 0;
        }
        center = peak;

        // print_view(view);
        //    setting the value at the index of peak to -1 by default
        float peak_value = -1, neighbor_val = 0;

        // looping through view to find largest number and setting it as peak
        for (row = 0; row < VIEW_SIZE; row++)
        {
            for (col = 0; col < VIEW_SIZE; col++)
            {
                if (view[row][col] > peak_value)
                {
                    peak_value = view[row][col];
                    neighbor_val = view[row][col + 1];
                    peak = get_landcape_co_ord(center, col, row);
                }
            }
        }

        // printf("Peak at %d-%d, value (%.2f)\n", peak.y, peak.x, peak_value);

        add_traversed_path_point(path_point_array, peak, counter);

        if (declare_peak(peak.x, peak.y) == 1)
            return peak;
        else
        {

            if (out_of_bonds_check(view, VIEW_SIZE * VIEW_SIZE) == 1)
            {
                last_point = get_last_traversed_path_point(path_point_array, &counter, 1);
                counter--;
                row = last_point.x;
                col = last_point.y;
                int last_row = last_point.x;
                int last_col = last_point.y;
                generate_view(view, col, row);
                reverse_rows(view);
                backtracking = 1;
                back_track_counter++;
            }

            if ((compare_neighbor(&peak, &neighbor, peak_value, neighbor_val) == 1))
            {
                // print_traversed_path_points(path_point_array, counter + 1);
                row += calc_average_direction(path_point_array, counter).x;
                col += calc_average_direction(path_point_array, counter).y;

                //printf("\n-----Mean direction (%d,%d)-----\n\n", calc_average_direction(path_point_array, counter).x, calc_average_direction(path_point_array, counter).x);

                peak = get_landcape_co_ord(center, col, row);
            }
            if (back_track_counter == 2)
            {
                // printf("No greater value than %f\n", peak_value);
                last_point = get_last_traversed_path_point(path_point_array, &counter, 2);

                row = last_point.x;
                col = last_point.y;

                // printf("\n\nBack track counter inp\n\n");
                row += explore_other_directions(&peak, peak_value).x;
                col += explore_other_directions(&peak, peak_value).y;
                // getchar();
            }
        }
    }
    return peak;
}
