To solve the puzzle for day7 a struct named _PorcessingTree_ is created.
It represents a tree structure to store all the results possible from the various operations to be done on the various `int` data that are given as the operands.

For example:
Consider the line `3267: 81 40 27`
The operands are `81` , `40` and `27`
We have two types of operators, `+` (add) and `x` (multiply).
If we have to build the tree with all possible values that the operations of these operators on these operands can generate,
then the tree (binary, balanced) will look like the image below:
<p>
                               
                             [81]      <----------------------- layer-0
                             __|___
                        +40/       \x40
                       [121]       [3240]   <------------------ layer-1
                       _|_           __|___
                   +27/   \x27   +27/      \x27
                  [148]  [3267]  [3267]  [87480]  <------------ layer-2
</p>

The last level in the above tree shows all possible solutions that can be produced using all tree operands being operated on by either ADD(+) and/or MUL(x) operators.
The desired result is `3267` and it is indeed one of the elements in the last layer. 
We can, therefore, conclude that it is indeed possible to obtain, `3267`, the desirted-result, with the given operands.

This shows that we can create such a tree and compare the desired reult with all the elements in the last layer;
if a match is found then the possibility is YES else No.
(possiblity here means the possibility of obtaining the desired result using the operands)
