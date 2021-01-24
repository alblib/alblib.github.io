***Problem 2018-03-3.*** Define the *n*<sup>th</sup> **primorial** P<sub>*n*</sub> to be the product of the first *n* primes.
What is the largest palindromic factor of P<sub>*n*</sub>?

***Attempt***
We only aim for up to *n*=64 because it has already complexity of 2<sup>64</sup> and will not end till the end of the UNIVERSE!
In bruteforce approach, we might obtain every possible multiplication of prime numbers (2<sup>n</sup>) and test if it is palindromic.
There may be two approaches to reduce computation. (1) To reduce testing complexity by implementing pre-condition. (2) To implement fast pre-test function to simplify each failed thread.

***Simple Test Functions***
First, we are pursuing the largest factor for each *n*, we can ignore values smaller than the answer for *n*-1. Secondly, it would be helpful to test palindromicity rapidly by comparing first digit and last digit and so forth. The fastest way to do this may be to use logarithm. We can prepare the set of the logs of all the prime numbers with maximum precision. Simply adding those logs gives the brief values. Also, if the precision is not enough, we can obtain the last digit by multiplication under modulus 10. Multiplication table modulus 10 also should be prepared in advance to reduce calculation time. If you want to test up to 2 digits, table should be modulus 100.