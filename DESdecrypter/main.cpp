#include <iostream>
#include <vector>
#include <iomanip>
#include <unistd.h>
#include "DictionaryAttack.h"

using namespace std;

void printSpeedup(vector<string> passwords, vector<long> sequentialTimes, vector<long> parallelTimes,
                  vector<float> speedup, string method);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Too few arguments!" << endl;
        return 1;
    }

    int runsPerPassword;
    runsPerPassword = atoi(argv[1]);

    if (runsPerPassword <= 0) {
        cout << "Number of runs must be a positive integer" << endl;
        return 1;
    }

    vector<string> passwordsToDecrypt = {"aaghrslq", "aaghyjat", "aagiezpy", "aagilpmw"};
    string salt = "k2";

    DictionaryAttack dictionaryAttack(passwordsToDecrypt, salt);

    vector<long> sequentialTimes, autoParallelTimes, manualParallelTimes;
    vector<float> autoSpeedup, manualSpeedup;
    vector<int> numberOfThreads = {4}; //,3,4,5,6,7,8,9,10,20,40,60,80,100,200,500,1000,5000,10000};

    sequentialTimes = dictionaryAttack.sequential(runsPerPassword);

    for (int t : numberOfThreads) {
        cout << endl;
        cout << "Number of threads used: " << t << endl;

        autoParallelTimes = dictionaryAttack.autoParallel(runsPerPassword, t);
        autoSpeedup = dictionaryAttack.computeSpeedUp(sequentialTimes, autoParallelTimes);
        printSpeedup(passwordsToDecrypt, sequentialTimes, autoParallelTimes, autoSpeedup, "automatic parallel");
        usleep(5000000);

        manualParallelTimes = dictionaryAttack.manualParallel(runsPerPassword, t);
        manualSpeedup = dictionaryAttack.computeSpeedUp(sequentialTimes, manualParallelTimes);
        printSpeedup(passwordsToDecrypt, sequentialTimes, manualParallelTimes, manualSpeedup, "manual parallel");
        usleep(5000000);

    }

    return 0;
}

void printSpeedup(vector<string> passwords, vector<long> sequentialTimes, vector<long> parallelTimes,
        vector<float> speedup, string method) {

    cout << "Using " << method << " method: " << endl;

    for (int i = 0; i < passwords.size(); i++) {
        cout << "Password #" << i+1 << ": " << passwords[i] << " - ";
        cout << "Seq. time " << sequentialTimes[i] << "ns - Par. time " << parallelTimes[i] << " ns - ";
        cout << "Speed up " << fixed << setprecision(7) << speedup[i] << endl;
    }
}
