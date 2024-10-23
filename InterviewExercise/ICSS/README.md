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

- Create 13 “price” objects to store the operator ID and price. The corresponding prefixes of this price will be represented by the structure of the Trie. Refer to lines 166-171 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L166) and lines 6-19 of [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out#L6).

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/2.jpg)

- Create 17 “node” objects for these 13 prefixes. We need to set the corresponding vaules in each node to display the parent-child relationship between these 17 nodes. Refer to lines 46-74 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L46) and lines 21-38 of [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out#L21).

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/3.jpg)

- Create 17 prefix list objects for these 17 nodes. Each prefix list object contains the prices of operators with the same prefix which belong to the node with the same index. Refer to lines 46-74 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L46) and lines 40-57 of [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out#L40).

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/4.jpg)

- For example: prefix is "46732". Refer to lines 76-103 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L76)
  1. The first number is "4". From the root node (nodes[0]), we find out the value in the nodes[0][4] is 5. This means we need to move to nodes[5]. We check prefixs[5], the price list is empty, it means no operator has prefix "4".
  2. Next number is "6". We find out the value in the nodes[5][6] is 6. This means we need to move to nodes[6]. We check prefixs[6], there are two prices[2] and prices[10] were found. So, for the prefix "46", operator 1's price is prices[2] = 0.17, operator 2's price is prices[10] = 0.2.
  3. 
  4. 
  5. 
  6. 

- After getting the longest prefix-matching for each operator (if there is a match), we only need to check each returned item to find the one with the lowest price. Refer to lines 206-214 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L206)

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/5.jpg)

## Test Cases:

- In addition, I added two test cases, one to test the situation where the operator cannot be found.

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/7.jpg)

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/9.jpg)

- The other to test the case where the input is empty.

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/8.jpg)

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/10.jpg)

## In previous work and study, I have used Trie in the following examples:

- In the first page of my resume, in the "Project experience" section, project named "User Intent Recognition - Latest News Recommendation". Refer to lines 62~210 of [Storage.h](https://github.com/daleiyang/Projects/blob/main/LatestNewsRecommendation/service/Storage.h#L62). You will see a Trie implementation with a similar coding style.

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/6.jpg)