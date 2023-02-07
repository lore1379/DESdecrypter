#include <iostream>
#include <vector>
#include <iomanip>
#include "DictionaryAttack.h"

using namespace std;

void printSpeedup(vector<string> passwords, vector<long> sequentialTimes, vector<long> parallelTimes,
                  vector<float> speedup, string method) {

    cout << "Using " << method << " method: " << endl;

    for (int i = 0; i < passwords.size(); i++) {
        cout << "Password #" << i+1 << ": " << passwords[i] << " - ";
        cout << "Seq. time " << sequentialTimes[i] << "ns - Par. time " << parallelTimes[i] << " ns - ";
        cout << "Speed up " << fixed << setprecision(7) << speedup[i] << endl;
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Too few arguments! Insert the number of runs" << endl;
        return 1;
    }

    int experimentRuns = atoi(argv[1]);

    if (experimentRuns <= 0) {
        cout << "Number of runs must be a positive integer" << endl;
        return 1;
    }

    vector<string> passwordsToEncrypt = {"password", "sky_surf", "skutterr", "hotel333"};
    string salt = "k2";

    DictionaryAttack dictionaryAttack(passwordsToEncrypt, salt);

    vector<long> sequentialTimes, autoParallelTimes, manualParallelTimes;
    vector<float> autoSpeedup, manualSpeedup;
    vector<int> numberOfThreads = {10};

    sequentialTimes = dictionaryAttack.sequential(experimentRuns);

    for (int t : numberOfThreads) {
        cout << "Number of threads used: " << t << endl;

        autoParallelTimes = dictionaryAttack.autoParallel(experimentRuns, t);
        autoSpeedup = dictionaryAttack.computeSpeedUp(sequentialTimes, autoParallelTimes);
        printSpeedup(passwordsToEncrypt, sequentialTimes, autoParallelTimes, autoSpeedup, "automatic parallel");

        manualParallelTimes = dictionaryAttack.manualParallel(experimentRuns, t);
        manualSpeedup = dictionaryAttack.computeSpeedUp(sequentialTimes, manualParallelTimes);
        printSpeedup(passwordsToEncrypt, sequentialTimes, manualParallelTimes, manualSpeedup, "manual parallel");
    }

    return 0;
}