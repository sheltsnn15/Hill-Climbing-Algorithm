# The Mountain

## Overview

You are tasked with finding the peak of a fog-shrouded mountain represented as a 2D matrix. The matrix is invisible, and you can only explore it through limited visibility using provided functions. Your goal is to reach the peak in the fewest number of function calls.

### Key Functions
- `void generate_view(float view[VIEW_SIZE][VIEW_SIZE], int center_y, int center_x)`  
  Populates a view matrix centered on the specified coordinates. Each call increments the usage count.

- `int declare_peak(int x, int y)`  
  Checks if the given coordinates are the peak. Returns 1 if true, 0 otherwise. Each call also increments the usage count.

### Objectives
- **Find the Peak:** Locate the highest point in the matrix using minimal `generate_view` and `declare_peak` calls.
- **Constraints:** You cannot directly access the matrix or know its dimensions. Use `generate_view` to explore the landscape.

### Tasks
1. **Implement `find_highest_point()`** in `gradient_sol.c`:  
   Start at an initial coordinate (e.g., x=0, y=0) and use `generate_view` to explore. Once you suspect you've found the peak, call `declare_peak` and return the peak coordinates as a `path_point` struct.

2. **Compile Code:**  
   Use the following command to compile:
   ```sh
   gcc gradient.c gradient_sol.c -lm -o gradient
   ```

3. **Evaluate Solution:**  
   - **Single Run:** Use `single_run(int seed)` for debugging. A fixed seed will provide a consistent landscape, while `-1` uses the current time for a new landscape each run.
   - **Performance Evaluation:** Use `performance_eval()` to test your solution against 100 random landscapes.
