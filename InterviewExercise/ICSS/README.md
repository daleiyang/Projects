## Submitter
- Dalei Yang
- [2024 Programming Exercise](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/2024%20Programming%20Exercise.pdf)
- Source Code: [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c)
- Test Cases  Input: [trie.in](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.in)
- Test Cases Result: [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out)

- [Resume](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/Dalei%20Yang%20Resume%2024.10.18.V2.pdf)
- [Cover Letter](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/Cover%20Letter.txt)

## Algorithm
- I use a Trie (prefix tree) to solve this problem.

- I use test cases in "2024 Programming Exercises" to demonstrate my implementation.

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/1.jpg)

- Create 13 “price” objects to store the operator ID and price. The corresponding prefixes of this price will be represented by the structure of the Trie. Refer to lines 166~171 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L166) and Refer to [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out).

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/2.jpg)

- Create 17 “node” objects for these 13 prefixes. We need to set the corresponding vaules in each node to display the parent-child relationship between these 17 nodes. Refer to [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out).

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/3.jpg)

- Create 17 price list objects for these 17 nodes. Each price list object contains the prices of operators with the same prefix which belong to this node. Refer to [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out).

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/4.jpg)

- For example: the prefix "46732".

- After getting the longest prefix-matching for each operator (if there is a match), we only need to check each returned item to find the one with the lowest price.

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/5.jpg)

## In previous work and study, I have used Trie in the following examples:

- In the first page of my resume, in the "Project experience" section, project named "User Intent Recognition - Latest News Recommendation". Refer to lines 62~210 of [Storage.h](https://github.com/daleiyang/Projects/blob/main/LatestNewsRecommendation/service/Storage.h#L62). You will see a Trie implementation with a similar coding style.

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/6.jpg)