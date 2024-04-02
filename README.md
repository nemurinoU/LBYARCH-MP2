# LBYARCH-MP2
## x86-to-C interface programming project
The program runs two versions of the DAXPY function: one implemented in C and the other implemented in NASM (x86_64 assembly language). The DAXPY function computes the result of A * X + Y where A is a scalar and X and Y are vectors. The program also outputs the average runtime for each kernel, providing insights into the performance of the C and NASM implementations of the DAXPY function.

### Generating Test Cases
In order to generate test cases, a *main_test.py* function can be found in the *\_\_tests\_\_* folder. Running this file will automatically generate the values for n, A, X, and Y. Note that the size of the vectors should be changed according to the size wanted. 

### Running the Program
To run the program, open the *.sln* file. Then, build and run the *main.c* file. The program will prompt you for a file input, and you may input any .txt file found in the *\_\_tests\_\_* folder that were previously generated by *main_test.py*.

## Kernel Performance Analysis
Below are tables for the average runtimes of the two kernels and the corresponding vector lengths. 

### Average Execution Time in Debug Mode
| Vector Length  | C Kernel |  x86_64 Kernel
| ------------- | ------------- | ----------- | 
| **2<sup>20</sup>**  | 2.788 ms  |  1.060 ms |
| **2<sup>24</sup>**  | 63.631 ms |  23.169 ms |
| **2<sup>28</sup>**  |   |           |

### Average Execution Time in Release Mode
| Vector Length  | C Kernel |  x86_64 Kernel
| ------------- | ------------- | ----------- | 
| **2<sup>20</sup>**  | 1.185 ms  |  1.051 ms |
| **2<sup>24</sup>**  | 21.100 ms |  22.388 ms |
| **2<sup>28</sup>** | 2637.284 ms  | 1379.334 ms |

## Analysis

When running the program in debug mode, it is apparent that the NASM kernel is far faster than the C program, and the runtime exponentially increases the bigger the vector size becomes. However, when running it in release mode, the difference becomes smaller, and in certain cases C outperforms the NASM kernel as well. This suggests that the optimization that occurs when the program is released in release mode significantly improves the performance of the programs, particularly the C kernel's. For instance, the C kernel's performance improves by 202% for the vector length of 2<sup>24</sup> when it's executed in release mode. This underscores the importance of optimization techniques as well as compiler settings.

## Runtime per iteration
Below is a breakdown of the runtime for each of the 30 iterations that were performed for each category. The size of each vector is indicated for each test performed.

## DEBUG MODE
### 2<sup>20</sup> for C (left) and NASM (right)
<div>
<img src="./screenshots/2e20debug-c.png" width="150" height="400">
<img src="./screenshots/2e20debug-asm.png" width="150" height="400">
</div>

### 2<sup>24</sup> for C (left) and NASM (right)
<div>
<img src="./screenshots/2e24debug-c.png" width="150" height="400">
<img src="./screenshots/2e24debug-asm.png" width="150" height="400">
</div>


### 2<sup>28</sup> for C (left) and NASM (right)

<div>
<img src="./screenshots/debug2r28_part1.png" width="500" height="500">
<img src="./screenshots/debug2r28_part2.png" width="500" height="500">
</div>

## RELEASE MODE
### 2<sup>20</sup> for C (left) and NASM (right)
<div>
<img src="./screenshots/2e20release-c.png" width="150" height="400">
<img src="./screenshots/2e20release-asm.png" width="150" height="400">
</div>

### 2<sup>24</sup> for C (left) and NASM (right)
<div>
<img src="./screenshots/2e24release-c.png" width="150" height="400">
<img src="./screenshots/2e24release-asm.png" width="150" height="400">
</div>

### 2^28 for C (left) and NASM (right)

<div>
<img src="./screenshots/release2r28_part1.png" width="500" height="500">
<img src="./screenshots/release2r28_part2.png" width="500" height="500">
</div>

## Output with Correctness Check
In order to validate the result of our NASM kernel, we compared the output with its C counterpart. Below is a sample result when a category is checked. We ensured that every run had NASM results that matched with the C kernel.

<img src="./screenshots/2e20debug-correct.png" width="500" height="70">

## Members
This project was created by Francis Benedict Martinez and Krizchelle Wong


