# DES decrypter in C++: sequential version vs parallel version with OpenMP

Mid-Term assignment for Parallel Computing course. It compares a sequential approach against two parallel ones with OpenMP to observe the benefit of using parallelism in password decryption. The algorithm implements a "Dictionary Attack" to discover a password encrypted with DES algorithm. It only requires an integer as input, that is the number of times the program has to run to get the mean of execution times and have a better estimation. 

Detailed explaination of the project can be found [here](Report.pdf).

Requirements to run this program are:

* crypt library to use the crypt and crypt_r functions
* OpenMP C framework to handle parallel sections inside the code
