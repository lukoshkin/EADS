# Help an artist!
### Task
Inspired by Picasso's "Girl on the ball", sculptor Vassily wants to carve a large ball1. He needs a stone block for that, and goes to a stone warehouse. There he finds a vast array of stones, all of them are rectangular boxes (rectangular parallelepipeds) of various sizes. Since Vassily wants to carve the largest ball possible, one simple option is to choose a stone box with the maximum value of the smallest dimension (e.g. box with dimensions 45×40×60 allows carving a ball of diameter 40). However, there is also another option: Vassily can choose two boxes (but not more) and glue them together to produce a larger box. He can only do that if the two boxes have an identical rectangular face: for example, he can glue up boxes of dimensions 70×80×50 and 80×40×70 to obtain a box of dimensions 70×80×90. (Note that it is possible to turn boxes around. Note also that each box is available in one copy, that is it is not possible to glue a box "with itself".)

There are too many boxes to choose from and Vassily does not know how to make the best choice to make the largest possible ball. He is an artist who never studied algorithms and never heard about hashing. Write a program efficiently helping Vassily to choose one or two boxes. "Efficiency" here means that your solution should avoid sorting or searching of large arrays of numbers and should preferably process boxes online.

#### Input:
 
Input file named input.txt is formatted as follows:  
The first line holds the number of boxes n (1≤n≤105).  
It is followed by n lines, line i storing the dimensions of the i-th box: 3 integers separated by a space.

#### Output:
Output file named output.txt should have three lines:  
The 1st line holds the number of chosen boxes (1 or 2)  
The 2nd line holds the index(es) of chosen box(es): either one or two numbers between 1 and n separated by a space. Indexes of two boxes are written in increasing order  
The 3rd line holds the diameter of the ball

**Example 1:**
input.txt
```
6
5 5 5
3 2 4
1 4 1
2 1 3
3 2 4
3 3 4
```
output.txt
```
1
1
5
```
**Example 2:**  
input.txt
```
7
10 7 8
5 10 3
4 2 6
5 5 5
10 2 8
4 2 1
7 7 7
```
output.txt
```
2
1 5
8
```

#### Technical remarks

For implementing hash table, you are not allowed to use standard implementations of associative arrays, like dict in Python or unordered_map in C++, or any external implementations of hash functions. You have to implement a hash table "from scratch".

Make sure that in the output, numbers are separated by a single space and the last line does not end with linefeed. Standard input/output is not supported, respect names input.txt and output.txt for input and output files respectively.
