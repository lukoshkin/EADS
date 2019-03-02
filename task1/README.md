# Shortest path on a graph with strangely defined weights: version I
### Task

We are given a directed graph G=(V,E) and a source node s in V. The graph has weighted edges, and the weights are defined in the following way: for each edge (p,q), its weight is 1 iff the minimum number of edges to traverse to get from s to p in graph G is an even number (including 0), otherwise its weight is 2. In so weighted graph, given a destination node t, compute the shortest path from s to t using Dijkstra's algorithm with "early exit" strategy.

#### Input:

Input file named `input.txt` is formatted as follows:  
1st line: number of vertices n=|V| (single positive integer)  
2nd line: number of edges m=|E| (single positive integer)  
3rd line: index of source node s  
4th line: indexes of destination node t  
following m lines: pairs of vertices representing edges, separated by a space, where vertices are numbered from 0 to n-1  

#### Output:

Output file named output.txt should contain a single number: cost of the shortest path from s to t  

**Example 1:**  
input.txt  

```
5
6
0
4
0 1
1 2
2 3
3 4
1 4
2 4
```
output.txt
```
3
```
#### Technical remarks

For implementing priority queue, you are **not** allowed to use a standard library, you have to implement it yourself.  
Standard input/output is not supported, respect names `input.txt` and `output.txt` for input and output files respectively.
