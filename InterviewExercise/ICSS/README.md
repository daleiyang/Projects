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

- Create 13 “price” objects to store the operator ID and price. The corresponding 13 prefixes of these prices will be represented by the structure of the Trie. Refer to lines 166-171 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L166) and lines 6-19 of [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out#L6).

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/2.jpg)

- Create 17 “node” objects for these 13 prefixes. We need to set the corresponding vaule in each node to show the parent-child relationship between these 17 nodes. Refer to lines 46-74 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L46) and lines 21-38 of [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out#L21).

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/3.jpg)

- Create 17 prefix objects for these 17 nodes. Each prefix object contains a list of price object IDs which belong to the operators that declare this prefix in their price lists. Refer to lines 46-74 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L46) and lines 40-57 of [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out#L40).

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/4.jpg)

- For example: phone number is "4673212345". Refer to lines 76-103 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L76)
  1. The first number is "4". From the root node (nodes[0]), we find out the value in the nodes[0][4] is 5. This means we need to move to nodes[5]. We check prefixs[5], the price list is empty, it means no operator has prefix "4".
  2. Next number is "6". We find out the value in the nodes[5][6] is 6. This means we need to move to nodes[6]. We check prefixs[6] and find prices[2] and prices[10]. So, for the prefix "46", operator 1's price is prices[2] = 0.17, operator 2's price is prices[10] = 0.2.
  3. Next number is "7". We find out the value in the nodes[6][7] is 12. This means we need to move to nodes[12]. We check prefixs[12] and find prices[11]. So, for the prefix "467", operator 2's price is prices[11] = 1.0. We discard prices[10] = 0.2 for operator 2.
  4. Next number is "3". We find out the value in the nodes[12][3] is 13. This means we need to move to nodes[13]. We check prefixs[13] and find prices[6]. So, for the prefix "4673", operator 1's price is prices[6] = 0.9. We discard prices[2] = 0.17 for operator 1.
  5. Next number is "2". We find out the value in the nodes[13][2] is 14. This means we need to move to nodes[14]. We check prefixs[14] and find prices[7]. So, for the prefix "46732", operator 1's price is prices[7] = 1.1. We discard prices[6] = 0.9 for operator 1.
  6. Next number is "1". We find out the value in the nodes[14][1] is 0. So, we stop here.

- After getting the longest prefix-matching for each operator (if there is a match), we only need to check each returned item to find the one with the lowest price. Refer to lines 206-214 of [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c#L206)

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/5.jpg)

## Test Cases:

- Additionally, I've added two test cases, one to cover the case where the operator is not found.

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/7.jpg)

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/9.jpg)

- Another cover the case where the input is empty.

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/8.jpg)

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/10.jpg)

## In previous work and study, I have used Trie in the following examples:

- In the "Project Experience" section on the first page of my resume, the project is called "User Intent Recognition - Latest News Recommendation". Refer to lines 62-210 of [Storage.h](https://github.com/daleiyang/Projects/blob/main/LatestNewsRecommendation/service/Storage.h#L62). You will see a Trie implementation with a similar coding style.

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/6.jpg)