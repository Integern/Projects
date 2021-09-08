# University Projects
This is a collation of all the coding projects that I have completed while studying for my bachelors in Mathematical Sciences from the University of Bath (which included some modules from the Computer Science department)




## What I learnt from these projects
I completed a variety of projects that focused on different aspects of computer science in a range of languages: C++, Python, Java, MATLAB, Haskell.

The most important skill, in my opinion, is a solid grasp of Object Orientated Programming. This was especially profound in my project where I created a 3D renderer - having different classes for different objects, having a ray class interact with all objects, etc... I really enjoy coding in this fashion as it keeps code neat, clean and easy to document+maintain.

Throughout I applied a lot of my mathematical knowledge to solving problems. Vectors were prevelant in many of my projects, which has led to a mastery in my understanding of vector calculations. A lot of projects also focused on time complexity of code and big O notation. This was extremely important for code computing hundreds of thousands of vector intersections - so I understand the importance of implementing efficient algorithms to produce fast code.

For much of the code I have written it was necessary to test it. Testing code can help you find edge cases that you need to support, and ensure any code you ship is in fully working order. A blessing for reducing stress when it comes to bug testing.

I have learnt many other skills across these projects, which I will try and summarise below:
 - Don't repeat code; compartmentalise!
 - Simple code is better code - easier to write, easier for someone else to understand
 - Commenting code is essential for having others (and yourself!) undestand your code
 - AI can do incredible things; but weigh up the time investement needed for the outcome it will give
 - The skills needed to efficiently debug code when you aren't getting the outcome you expected.




# What I have done
Below I have summarised what I did in each of my projects and what I learned from them.


## MATLAB Recursion (2017/18) [Programming and discrete mathematics] 44 KB
This was my first foray into MATLAB and recursion - not massively complex or lengthy code, but I learnt how a computer efficiently would calculate exponentials or polynomials.

I also learnt the importance of writing and running unit tests to ensure that ones code meets all base and edge cases.



## Implementing the Lambda Calculus (2018/19) [Functional Programming] 144 KB
The Lambda Calculus is a system of mathematical logic for expressing computation. Definitely not a simple topic to understand, this project had me understand the lambda calculus on paper and then write an implementation in Haskell - which was pseudocode-esque but had capability for writing my own types. 

This was one of my first links between the logic required in the Mathematical side of my degree and my coding skills for modules I took from the Computer Science department.



## Linked Lists (2018/19) [Data Structures and Algorithms] 708 KB
Being my first implementation of Java, I learnt about linked lists - having a generic object point to another object of the same type to create a list of objects. Then when it came to thinking about how we would traverse this list and moreso searching, we looked at skip lists. Skip Lists help us jump to a section in the list that the entry we are looking for is likely to be (as skip lists must be sorted).

This is the first time I became familiar with Object Orientated Programming (OOP) and subsequently fell in love with it!



## String Manipulation & Social Networks (2018/19) [Data Structures and Algorithms] 904 KB
I learnt about string manipulation in Java, and how different methods of the string object use different computation even when giving the same output. This led us into analyzing the time complexity of a function and how important this is when dealing with large amounts of data.

We also constructed a simple social network object which allowed a user to add/remove friends as well as search for friends using binary search. This was very useful as it showed me how to implement objects within objects.



## Missionaries & Cannibals (2018/19) [Artificial Intelligence and Machine Learning] 820 KB
This was my first project relating to AI - was a swift but thorough introduction into Python. The Missionaries and Cannibals problem is a 'Toy Problem', a famous example being a farmer has to try transport 2 chickens and 1 fox across a river. I implemented breadth first search to generate all possible states from the current state (e.g. we could move a chicken or a fox across the river as a first move) to see which one was a valid move. This would eventually bring us a solution.



## Eight Queens (2018/19) [Artificial Intelligence and Machine Learning] 192 KB
A quick project for how to position 8 queens on a chess board such that no queen can attack another queen. This implemented a hill-climbing search, which works by being able to give any given state a 'score' and moves to states with higher scores until a maximum is reached.



## Sudoku Solver (2018/19) [Artificial Intelligence and Machine Learning] 232 KB
This was an AI sudoku solver that finds solutions to sudokus by permuting the board and finding the most likely candidates that will give us a solution. The project had us try to create an efficient sudoku solver by showing the execution time for our code for easy and harder sudoko puzzles.



## Black Box (2018/19) [Artificial Intelligence and Machine Learning] 88 KB
This project was to create a classifier - we were given a training set to train our AI. I chose to use linear regression to train my classifier.

To look at the perfomance of the fitted model I looked at *accuracy*, *precision* and *recall* - I was able to get these values from *training_data* by training on 4750 rows of data and testing on 250 rows of data. Accuracy is self explanatory - how many the fitted model got correct - precision is how correct the fitted model is - what's the chance that when it makes a prediction, that prediction is correct - and recall denotes how useful the model is - in the example of spam/ham emails, the recall denotes how many spam emails are correctly identified. When running this test I got Accuracy=93.2%, Precision=96.1%, Recall=91.1% - which is very good!

I considered using Naive Bayes, but I wanted to try a different classifier and I didn't get it working as well as I could have in coursework 3 so I wanted to see if I could improve with something else. Could've used k-nearest neighbour, but there is 38 different features and it may have been difficult to find the optimum k.



## Connect 3 (2018/19) [Artificial Intelligence and Machine Learning] 533 KB
A smaller version of Connect 4. This project had me use Q-Learning to teach an AI on how to optimally play Connect 3. It runs through many many games using random moves to where it gathers information on every possible state of the game. Depending on how 'useful' a certain state is to reaching a win, it gives states a certain 'score' in a Q-Matrix.

After training, when playing against someone on any given state, the AI will make a move so the board gets taken to a state with the highest possible score.



## Realistic Raytracer (2020/21) [Advanced Computer Graphics] 1.7 MB
I created from the ground up a 3D renderer that can render 3D objects (given by a collection of vertices to form triangles) and applies lighting; both natural lighting and that from light objects.

This was my introduction to C++, where I learnt basics of reading files and solidified my understanding of OOP. It gave me an excellent understanding of vector calculations, as I had to create efficient code for raycasting against multiple objects (3D Meshes alongside more geometric objects e.g. 3D Planes, Spheres, Cuboids..). This project required a lot of debugging!



## Monty Hall Problem (2020/21) [Communicating Maths] 9.4 MB
This was a project that was written in LUA and produced in ROBLOX! By this point I had developed many ROBLOX projects, and our objective was to create any type of content of our choosing to educate young people on mathematics. I created an interactive game for children to play to teach them about the Monty Hall Problem, and the majority of the module was a focus on evaluating the success of our work. For me, this resulted in surveying users that played the game to get feedback, so I could write up how I would choose to improve the game. 

In the folder for this repository, I have included the section of my report focusing on this project.

The game can be found here: https://www.roblox.com/games/6550260745/The-Goat-Car-Problem (Could not name it the Monty Hall Problem as the ROBLOX filter didn't like it!)
