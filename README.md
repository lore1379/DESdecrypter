# DES decrypter in C++: sequential version vs parallel version with OpenMP

Mid-Term assignment for Parallel Computing course. It compares a sequential approach against two parallel ones with OpenMP to observe the benefit of using parallelism in password decryption. The algorithm implements a "Dictionary Attack" to discover a password encrypted with DES algorithm.

Detailed explaination of the project can be found [here](Report.pdf).

Requirements to run this program are:

* crypt library to use the crypt and crypt_r functions
* OpenMP C framework to handle parallel sections inside the code
