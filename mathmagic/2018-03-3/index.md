[***Problem 2018-03-3.***](https://erich-friedman.github.io/mathmagic/0318.html) Define the *n*<sup>th</sup> **primorial** P<sub>*n*</sub> to be the product of the first *n* primes.
What is the largest palindromic factor of P<sub>*n*</sub>?

***Attempt***
We only aim for up to *n*=64 because it has already complexity of 2<sup>64</sup> and will not end till the end of the UNIVERSE!
In bruteforce approach, we might obtain every possible multiplication of prime numbers (2<sup>n</sup>) and test if it is palindromic.
There may be two approaches to reduce computation. (1) To reduce testing complexity by implementing pre-condition. (2) To implement fast pre-test function to simplify each failed thread.

***Simple Test Functions***
First, we are pursuing the largest factor for each *n*, we can ignore values smaller than the answer for *n*-1. We may use logarithms to compare because addition is much faster than multiplication. Secondly, it would be helpful to test palindromicity rapidly by comparing first digit and last digit and so forth. The fastest way to do this may be to use logarithm. We can prepare the set of the logs of all the prime numbers with maximum precision. Simply adding those logs gives the brief values. Also, if the precision is not enough, we can obtain the last digit by multiplication under modulus 10. Multiplication table modulus 10 also should be prepared in advance to reduce calculation time. If you want to test up to 2 digits, table should be modulus 100.

***Code***
C++ code is [here](https://github.com/alblib/alblib.github.io/tree/main/mathmagic/2018-03-3). Multi-threaded. Much faster on multi-core cpu.

Too many threads gives error. We may create number of threads order of physical cores.

***Results***
max palindromic factor of *n*th primorial with base *b*

|*n*|base=2|3|4|5|6|7|8|9|10|11|12|
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
|**1**|1|2|2|2|2|2|2|2|2|2|2|
|**2**|3|2|3|6|3|6|6|6|6|6|6|
|**3**|15|10|15|6|5|6|6|30|6|a<sub>11</sub>|a<sub>12</sub>|
|**4**|21|10|105|6|35|6|105|70|7|181<sub>11</sub>|a<sub>12</sub>|
|**5**|231|10|2310|6|385|6|154|154|77|181<sub>11</sub>|b<sub>12</sub>|
|**6**|2145|910|2730|78|910|78|10010|2730|6006|181<sub>11</sub>|959<sub>12</sub>|
|**7**|36465|910|39270|119|3927|221|10010|34034|6006|424<sub>11</sub>|2992<sub>12</sub>|
|**8**|36465|910|39270|1482|25935|8151|570570|34034|969969|46a64<sub>11</sub>|272272<sub>12</sub>|
|**9**|36465|910|39270|1482|25935|8151|570570|34034|969969|46a64<sub>11</sub>|272272<sub>12</sub>|
|**10**|170085|910|56695|572286|25935|8151|570570|140645505|969969|1558551<sub>11</sub>|27499472<sub>12</sub>|
|**11**|682341|15314|9333170|682341|474145|8151|570570|140645505|36399363|1558551<sub>11</sub>|27499472<sub>12</sub>|
|**12**|682341|15314|9333170|32132058|11962951|8151|1028859|292777485|1346776431|4a8a8a4<sub>11</sub>|27499472<sub>12</sub>|
|**13**|682341|711022|19084065|32132058|11962951|12710|1028859|292777485|1346776431|7279727<sub>11</sub>|2124334212<sub>12</sub>|
|**14**|9017745|711022|19084065|180682818|430989559|453435|1028859|292777485|1346776431|7279727<sub>11</sub>|2124334212<sub>12</sub>|
|**15**|9017745|1545830|19084065|180682818|1399955865|453435|1028859|292777485|5330660335|7279727<sub>11</sub>|621a33a126<sub>12</sub>|
|**16**|25934755|750925942|242087835|180682818|23161921405|453435|85587845|292777485|5330660335|a08780a<sub>11</sub>|621a33a126<sub>12</sub>|
|**17**|7131078955|750925942|242087835|180682818|23161921405|453435|105148502|292777485|5330660335|a08780a<sub>11</sub>|a44b11b44a<sub>12</sub>|
|**18**|7131078955|750925942|242087835|7297598726|1279595348486|418308171|105148502|292777485|5041813181405|256a8a652<sub>11</sub>|a44b11b44a<sub>12</sub>|
|**19**|7131078955|750925942|10856665930|20111649558|1279595348486|418308171|105148502|2781165310|5041813181405|78a454a87<sub>11</sub>|a44b11b44a<sub>12</sub>|
|**20**|7131078955|750925942|58461938535|20111649558|1279595348486|418308171|1705982817|119662141170|5041813181405|1a4580a0854a1<sub>11</sub>|a44b11b44a<sub>12</sub>
|**21**|264419295855|7196514689|58461938535|20111649558|1279595348486|418308171|198342404666|712319400146|5041813181405|1a4580a0854a1<sub>11</sub>|62942924926<sub>12</sub>
|**22**|264419295855|346843585270|1256269018209|20111649558|1279595348486|1564319685|198342404666|712319400146|74833599533847||
|**23**|51748213506621|39369163618330|1256269018209|445518781197|78963385110610|24210247258|198342404666|995167010838|74833599533847||
|**24**| | | | | | | | |74833599533847||
|**25**| | | | | | | | |74833599533847||
|**26**| | | | | | | | |74833599533847||
|**27**| | | | | | | | |74833599533847||
|**28**| | | | | | | | |74833599533847||
|**29**| | | | | | | | |502073167761370205||
|**30**| | | | | | | | |5521159517777159511255||
|**31**| | | | | | | | |5521159517777159511255||
|**32**| | | | | | | | |5521159517777159511255||
|**33**| | | | | | | | |5521159517777159511255||
|**34**| | | | | | | | |5521159517777159511255||
|**35**| | | | | | | | |5521159517777159511255||
|**36**| | | | | | | | |5521159517777159511255||
|**37**| | | | | | | | |5521159517777159511255||
|**38**| | | | | | | | |7826676911771196766287||
|**39**| | | | | | | | |7826676911771196766287||
|**40**| | | | | | | | | |
