## Submitter
- Dalei Yang
- [2024 Programming Exercise](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/2024%20Programming%20Exercise.pdf)
- Source Code: [trie.c](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.c)
- Test Case Input: [trie.in](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.in)
- Test Result: [trie.out](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/trie.out)

- [Resume](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/Dalei%20Yang%20Resume%2024.10.18.V2.pdf)
- [Cover Letter](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/Cover%20Letter.txt)

## Algorithm
- I use a Trie (prefix tree) to solve this problem.

- I used test cases in "2024 Programming Exercises" to demonstrate my implementation.

![alt tag](https://github.com/daleiyang/Projects/blob/main/InterviewExercise/ICSS/pics/1.jpg)

- The combination of linkId, clcId, sbp in the figure above becomes the key value of the business logic, with a size of 54 bytes.
- The code in the figure below is the process of generating a 54-byte key value based on business logic, refer to [KeyIn54BitCASHashTable.cs](https://github.com/daleiyang/LockFreeHashTable/blob/master/CASHashTable/KeyIn54BitCASHashTable.cs).

![alt tag](https://github.com/daleiyang/LockFreeHashTable/raw/master/KeyGen.png)

- In securities trading system, the value is 64-bit integer instead of 256 byte array in my demo. For performance reasons, 64-bit integers are the most efficient choice.
- According to the number of keys in the business logic, select a prime number as the length of the hash table so that the load factor is 0.5. This can control the average number of hash table lookups to 1.1.

## Algorithms
- The TrySet, TryGet and TryDelete functions in [KeyIn54BitCASHashTable.cs](https://github.com/daleiyang/LockFreeHashTable/blob/master/CASHashTable/KeyIn54BitCASHashTable.cs) are the entry points.
- [KeyIn54BitCASHashTableBase.cs](https://github.com/daleiyang/LockFreeHashTable/blob/master/CASHashTable/KeyIn54BitCASHashTableBase.cs)  contains detailed comments explaining the principles of each bit operation and how to use the CAS API to read, add, update, and delete data.
- The “do... . while” loop in the figure below is a typical CAS API usage. 

![alt tag](https://github.com/daleiyang/LockFreeHashTable/raw/master/CAS.png)

## Performance
- Envrionment: Dell Z440 work station, 16 GB memory, 8 core CPU; 
- Hash table setting: key is int64, value is 256 byte array.
- Loads 3 million key-value pairs into a lock-free hash table in 4 seconds using a single process.
- 30 threads are started simultaneously. 10 are used to randomly get a record, 10 are used to randomly update a record, and 10 are used to randomly delete a record.

|Operation|Lock-Free Hash Table (operations per second)|.Net Concurrent Dictionary (operations per second)|Comparison|
|:----------|----------:|----------:|----------:|
|Get|7,113,400|1,681,929|<font color="red">4.2X</font>|
|Add/Update|8,927,004|240,321|<font color="red">37.1X</font>|
|Delete|13,566,043|245,884|<font color="red">55.2X</font>|

## Performance test report [[PerfTestingResults.xlsx]](https://github.com/daleiyang/LockFreeHashTable/raw/master/CASHashTable/PerfTestingResults.xlsx) summary

### Test Case One:  
- A key is randomly selected for each call across all processes.
- 10 “read” processes, 10 “update” processes and 10 "delete” processes are started and ended at the same time.

#### Test result for 10 “read” processes

![alt tag](https://github.com/daleiyang/LockFreeHashTable/raw/master/Get%20Random.jpg)
- [Get Attemps]：Total number of “TryGet” calls. Each time “TryGet”  is called, a random key is selected from the 3 million keys.
- [API Call Elapsed Time]：The time consumed by the “TryGet” function, excluding the time consumed by other auxiliary logic.
- [Get RPS/One Thread]：Number of times the "TryGet" function is executed per second.
- [Get API Call Elapsed Time/100,000,000 Attemps]：Time consumed per 100 million "TryGet" calls.
- [Get Successfully]：The number of successful value retrievals.
- [Get Successfully Percentage]：The number of successful value retrievals as a percentage of the total number of attempts.
- [Is Deleted]：The number of times the target key has been deleted.
- [Is Deleted Percentage]：The number of times the target key has been deleted as a percentage of the total number of attempts.
- [Result Match]：Data correctness verification ensures that the value obtained is consistent with the original value.
- [Result Match Percentage]：Percentage of data that is correct.
- [Test Elapsed Time]：Total test duration.

#### Test result for 10 “update” processes

![alt tag](https://github.com/daleiyang/LockFreeHashTable/raw/master/Update%20Random.jpg)
- [Update Attemps]：Total number of “TrySet” calls. Each time “TrySet”  is called, a random key is selected from the 3 million keys.
- [API Call Elapsed Time]：The time consumed by the “TrySet” function, excluding the time consumed by other auxiliary logic.
- [Update RPS/Thread]：Number of times the "TrySet" function is executed per second.
- [Update API Call Elapsed Time/100,000,000 Attemps]：Time consumed per 100 million "TrySet" calls.
- [Update Successfully]：Number of successful "TrySet".
- [Test Elapsed Time]：Total test duration.

#### Test result for 10 “delete” processes

![alt tag](https://github.com/daleiyang/LockFreeHashTable/raw/master/Delete%20Random.jpg)
- [Delete Attemps]：Total number of “TryDelete" calls. Each time “TryDelete”  is called, a random key is selected from the 3 million keys.
- [API Call Elapsed Time]：The time consumed by the “TryDelete” function, excluding the time consumed by other auxiliary logic.
- [Delete RPS/Thread]：Number of times the "TryDelete" function is executed per second.
- [Delete API Call Elapsed Time/100,000,000 Attemps]：Time consumed per 100 million "TryDelete" calls.
- [Delete Successfully]：Number of successful "TryDelete".
- [Delete Successfully Percentage]：The number of times the target key has been deleted as a percentage of the total number of attempts.
- [Is Deleted]：The number of times the target key has been deleted.
- [Is Deleted Percentage]：The target key has been deleted times as a percentage of the total number of attempts.
- [Test Elapsed Time]：Total test duration.

### ### Test Case two:  
- All processes select the same key on every call.
- 10 “read” processes, 10 “update” processes and 10 "delete” processes are started and ended at the same time.

#### Test result for 10 “read” processes

![alt tag](https://github.com/daleiyang/LockFreeHashTable/raw/master/Get%20One.jpg)

#### Test result for 10 “update” processes

![alt tag](https://github.com/daleiyang/LockFreeHashTable/raw/master/Update%20One.jpg)

#### Test result for 10 “delete” processes

![alt tag](https://github.com/daleiyang/LockFreeHashTable/raw/master/Delete%20One.jpg)

## Test Code
- [KeyIn54BitCASHashTableFunctionalTest.cs](https://github.com/daleiyang/LockFreeHashTable/blob/master/Test/KeyIn54BitCASHashTableFunctionalTest.cs) in the test project is a functional test that includes the verification of the correctness of addition, deletion, modification, and query in various situations.

- [KeyIn54BitCASHashTablePerfTest.cs ](https://github.com/daleiyang/LockFreeHashTable/blob/master/Test/KeyIn54BitCASHashTablePerfTest.cs) is a stress test. For the test method and result analysis, please refer to the section “Performance test report summary” above.
