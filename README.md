Enumeration program for all induced subtree in a given graph. 
See paper: http://arxiv.org/abs/1407.6140
Author:    Kunihiro Wasa
E-mail:    wasa[at]ist.hokudai.ac.jp

Options:
  -h [ --help ]                 display help (this page) 
  -f [ --file ] arg             file path
  -p [ --output_parenthesis ]   output the parenthesis of the search tree
  -e [ --output_entire ]        output all induced subtrees (entire)
  -d [ --output_differential ]  output all induced subtrees (differential)

Input file sample (petersen graph) 
1: 2 5 6 
2: 1 3 7 
3: 2 4 8 
4: 3 5 9 
5: 1 4 10 
6: 1 8 9 
7: 2 9 10 
8: 3 6 10 
9: 4 6 7 
10: 5 7 8 

Each line represents the adjacent list for a vertex v. 
	v: u[1] u[2] ... 
, where u[i] is the i-th adjacent of v. 
