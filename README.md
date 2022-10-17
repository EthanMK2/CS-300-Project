# CS-300-Project

The problem I tried to solve was finding the best data structure and algorithm to achieve the memory and performance objectives from using data from 
a "university courses" file. The file contained course id's, names, and prerequisites, if any. I created a program in C++ that parsed the file, loaded the data into a 
hash table, and retrieved data from the data structure by searching. 

I approached the problem by writing out the strengths of each data structure in their best case scenario. From there, I analyzed what the most likely scenario would be 
for the data structure, and used that to find the best for the problem. For example, a balanced binary search tree was the most effective, but the probability of the 
tree becoming balanced was very unlikely. 

The hardest roadblock for me was making the function that parsed the file correctly given any kind of course set up. Any course can have some prerequisites or none, 
and so I had to consider those possibilities. Going character by character was difficult, and was frustrating. Good thing I used Visual Studio Code, because it has 
a very useful debugger. I used many breakpoints to go line by line, and was able to find the strange issues that occured. This project really expanded my approach in 
software design and development, and makes me realize how important testing is. Without the debugger, I don't think I would have completed the project as well as I did. 
The project helped me refine my way of finding solutions and refactoring, as it became more important as the project progressed. There are still some aspects of the 
code that can be improved, but overall the project went well and broadened my analytical skills.
