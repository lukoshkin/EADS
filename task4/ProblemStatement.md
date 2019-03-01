# Pretty printing

#### Problem

We are given a text (words separated by a single space, without any punctuation signs) that we want to print nicely on a paper with line length L. (We assume that L is no smaller than the longest word in the text) We are not allowed to increase space between words in the same line, nor to introduce left margins (spaces between the beginning of a line and the first word). We can only change right margins (spaces between the end of the last word in a line and the line end) by changing the formatting of the text. We want to format the text in a nice way, to make the right margins as even as possible.

However, our measure of "evenness" is somewhat unusual: we want to minimize the sum of squares of the right margins of all lines (including the last one).

Write a program optimizing the formatting of the input text. Output the minimum sum of squares of right margins and the corresponding number of lines.

Note that the optimal formatting is not unique. For example, for L=10 the following two formatting are both optimal, with the sum of squares equal to $$38 = 2^2+ 3^2+ 5^2 = 2^2 + 5^2 + 3^2$$

```
Am I you
to be I
or to
```
```
Am I you
to be
I or to
```
**Comment:** Formally, nothing guarantees that all optimal solutions will have the same number of lines. However, in the tests, all optimal solutions will be guaranteed to have the same number of lines, therefore you can output the number of lines of any optimal solution.

#### Input:

Input file named input.txt will contain the line length L in the first line, and the input text (at most 10,000 words) in the second line.

#### Output:

Output file named output.txt should have two lines: 
1st line: the minimum sum of squares of the right margins
2nd line: the corresponding number of lines (cf comment above).
For the above example, the output should be
```
38
3
```
Make sure not to put \n at the end of the 2nd line.
