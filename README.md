# University Projects
This is a collation of all the coding projects that I have completed while studying for my bachelors in Mathematical Sciences from the University of Bath (which included some modules from the Computer Science department)


## What I learnt over the course of my degree (in regard to programming+computing)
blah


## What I have done
Below I have summarised what I did in each of my projects and what I learned from them.

##### MATLAB Recursion (2017/18) [Programming and discrete mathematics]
This was my first foray into MATLAB and recursion - not massively complex or lengthy code, but I learnt how a computer efficiently would calculate exponentials or polynomials.

I also learnt the importance of writing and running unit tests to ensure that ones code meets all base and edge cases.


##### Implementing the Lambda Calculus (2018/19) [Functional Programming]
The Lambda Calculus is a system of mathematical logic for expressing computation. Definitely not a simple topic to understand, this project had me understand the lambda calculus on paper and then write an implementation in Haskell - which was pseudocode-esque but had capability for writing my own types. 

This was one of my first links between the logic required in the Mathematical side of my degree and my coding skills for modules I took from the Computer Science department.


##### Linked Lists (2018/19) [Data Structures and Algorithms]
Being my first implementation of Java, I learnt about linked lists - having a generic object point to another object of the same type to create a list of objects. Then when it came to thinking about how we would traverse this list and moreso searching, we looked at skip lists. Skip Lists help us jump to a section in the list that the entry we are looking for is likely to be (as skip lists must be sorted).

This is the first time I became familiar with Object Orientated Programming (OOP) and subsequently fell in love with it!


##### String Manipulation & Social Networks (2018/19) [Data Structures and Algorithms]
I learnt about string manipulation in Java, and how different methods of the string object use different computation even when giving the same output. This led us into analyzing the time complexity of a function and how important this is when dealing with large amounts of data.

We also constructed a simple social network object which allowed a user to add/remove friends as well as search for friends using binary search. This was very useful as it showed me how to implement objects within objects.


##### Missionaries & Cannibals (2018/19) [Artificial Intelligence and Machine Learning]
This was my first project relating to AI - was a swift but thorough introduction into Python. The Missionaries and Cannibals problem is a 'Toy Problem', a famous example being a farmer has to try transport 2 chickens and 1 fox across a river. I implemented breadth first search to generate all possible states from the current state (e.g. we could move a chicken or a fox across the river as a first move) to see which one was a valid move. This would eventually bring us a solution.


##### Eight Queens (2018/19) [Artificial Intelligence and Machine Learning]
A quick project for how to position 8 queens on a chess board such that no queen can attack another queen. This implemented a hill-climbing search, which works by being able to give any given state a 'score' and moves to states with higher scores until a maximum is reached.


##### Connect 3 (2018/19) [Artificial Intelligence and Machine Learning]
A smaller version of Connect 4. This project had me use Q-Learning to teach an AI on how to optimally play Connect 3. It runs through many many games using random moves to where it gathers information on every possible state of the game. Depending on how 'useful' a certain state is to reaching a win, it gives states a certain 'score' in a Q-Matrix.

After training, when playing against someone on any given state, the AI will make a move so the board gets taken to a state with the highest possible score.


##### Realistic Raytracer (2020/21) [Advanced Computer Graphics]
I created from the ground up a 3D renderer that can render 3D objects (given by a collection of vertices to form triangles) and applies lighting; both natural lighting and that from light objects.

This was my introduction to C++, where I learnt basics of reading files and solidified my understanding of OOP. It gave me an excellent understanding of vector calculations, as I had to create efficient code for raycasting against multiple objects (3D Meshes alongside more geometric objects e.g. 3D Planes, Spheres, Cuboids..)