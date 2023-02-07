//
// Created by lorenzo on 29/08/22.
//

#ifndef DESDECRYPTER_DICTIONARYATTACK_H
#define DESDECRYPTER_DICTIONARYATTACK_H

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class DictionaryAttack {
public:
    DictionaryAttack(vector<string> passwordsToEncrypt, string salt);
    vector<long> sequential(int experimentRuns);
    vector<long> autoParallel(int runsPerPassword, int numberOfThreads);
    vector<long> manualParallel(int runsPerPassword, int numberOfThreads);
    long getMean(vector<long> values);
    vector<float> computeSpeedUp(vector<long> sequentialTimes, vector<long> parallelTimes);

private:
    vector<string> dictionaryVector;
    vector<string> encryptedPasswords;
    string salt;
};


#endif //DESDECRYPTER_DICTIONARYATTACK_H
