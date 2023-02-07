//
// Created by lorenzo on 29/08/22.
//

#include <cstdio>
#include <vector>
#include <string>
#include <crypt.h>
#include <string.h>
#include <iostream>
#include <chrono>
#include <omp.h>
#include <cmath>
#include "DictionaryAttack.h"

using namespace std;

DictionaryAttack::DictionaryAttack(vector<string> passwordsToEncrypt, string salt) {
    this->salt = salt;
    // reserve vector size to avoid dynamic allocation
    encryptedPasswords.reserve(passwordsToEncrypt.size());

    for (string& p : passwordsToEncrypt){
        encryptedPasswords.push_back(crypt(p.c_str(), salt.c_str()));
    }

    // read from file, vector cointaining all plaintexts in dictionary
    ifstream dictionary;
    // path to the dictionary
    dictionary.open("/home/lorenzo/ParallelComputing/DESdecrypter/rockyoucommon.txt");
    string line;

    while (getline(dictionary, line)) {
        dictionaryVector.push_back(line);
    }

    dictionary.close();
}

vector<long> DictionaryAttack::sequential(int experimentRuns){
    vector<long> executionTimes;
    executionTimes.reserve(encryptedPasswords.size());
    vector<long> runTimes;
    runTimes.reserve((unsigned long)experimentRuns);

    for (string& wantedPassword : encryptedPasswords){
        runTimes.clear();

        for (int i = 0; i < experimentRuns; i++) {
            auto start = chrono::steady_clock::now();

            for (string& p : dictionaryVector) {
                string encryptionAttempt(crypt(p.c_str(), salt.c_str()));

                if (wantedPassword == encryptionAttempt) {
                    //cout << "Password found: " << p << " - From hash: " << encryptionAttempt << endl;
                    break;
                }
            }

            auto stop = chrono::steady_clock::now();
            auto elapsed_time = chrono::duration_cast<chrono::nanoseconds>(stop - start);
            runTimes.push_back(elapsed_time.count());
        }

        executionTimes.push_back(getMean(runTimes));
    }

    return executionTimes;
}

vector<long> DictionaryAttack::autoParallel(int runsPerPassword, int numberOfThreads) {

    vector<long> executionTimes;
    executionTimes.reserve(encryptedPasswords.size());
    vector<long> runTimes;
    runTimes.reserve((unsigned long)runsPerPassword);

    for (string& wantedPassword : encryptedPasswords){
        runTimes.clear();

        for (int i = 0; i < runsPerPassword; i++) {

            volatile bool passFound = false;
            auto start = chrono::steady_clock::now();

            #pragma omp parallel default(none) shared(dictionaryVector, passFound, wantedPassword) num_threads(numberOfThreads)
            {
                // crypt_r is threadsafe, threads not contending crypt func
                struct crypt_data data;
                data.initialized = 0;

                #pragma omp for firstprivate(dictionaryVector)
                for (int i = 0; i < dictionaryVector.size(); i++) {
                    if (passFound) continue;

                    char *encryptionAttempt = crypt_r(dictionaryVector[i].c_str(), salt.c_str(), &data);

                    if (strcmp(encryptionAttempt, wantedPassword.c_str()) == 0) {
                        passFound = true;
                    }
                }
            }

            auto stop = chrono::steady_clock::now();
            auto elapsed_time = chrono::duration_cast<chrono::nanoseconds>(stop - start);
            runTimes.push_back(elapsed_time.count());
        }

        executionTimes.push_back(getMean(runTimes));
    }

    return executionTimes;

}

vector<long> DictionaryAttack::manualParallel(int runsPerPassword, int numberOfThreads) {
    vector<long> executionTimes;
    executionTimes.reserve(encryptedPasswords.size());
    vector<long> runTimes;
    runTimes.reserve((unsigned long)runsPerPassword);

    for (string& wantedPassword : encryptedPasswords) {
        runTimes.clear();

        for (int i = 0; i < runsPerPassword; i++) {
            int chunkPerThread = static_cast<int>(ceil((double)dictionaryVector.size() / (double)numberOfThreads));
            volatile bool passFound = false;

            auto start = chrono::steady_clock::now();

            #pragma omp parallel default(none) shared(passFound, chunkPerThread, wantedPassword) num_threads(numberOfThreads)
            {
                struct crypt_data data;
                data.initialized = 0;

                int numThread = omp_get_thread_num();

                for (int i = chunkPerThread * numThread; i < (numThread + 1) * chunkPerThread; i++) {
                    if (!passFound && i < dictionaryVector.size()) {

                        char *encryptionAttempt = crypt_r(dictionaryVector[i].c_str(), salt.c_str(), &data);

                        if (strcmp(encryptionAttempt, wantedPassword.c_str()) == 0) {
                            passFound = true;
                            break;
                        }
                    } else {
                        break;
                    }
                }
            }

            auto stop = chrono::steady_clock::now();
            auto elapsed_time = chrono::duration_cast<chrono::nanoseconds>(stop - start);
            runTimes.push_back(elapsed_time.count());
        }

        executionTimes.push_back(getMean(runTimes));
    }

    return executionTimes;

}

long DictionaryAttack::getMean(vector<long> values) {
    long sum = 0;

    for (long v : values) {
        sum += v;
    }

    return sum / values.size();
}

vector<float> DictionaryAttack::computeSpeedUp(vector<long> sequentialTimes, vector<long> parallelTimes) {
    vector<float> speedups;
    speedups.reserve(sequentialTimes.size());

    for (int i=0; i < sequentialTimes.size(); i++) {
        speedups.push_back((float)sequentialTimes[i] / (float)parallelTimes[i]);
    }

    return speedups;
}