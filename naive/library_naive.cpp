#include <bits/stdc++.h>
#include <pthread.h>
#include "../utils/thread_pool.cpp"
using namespace std;
#define int long long

/*
Each thread will compute a range of the vector 'c'. If instead each thread computed
a range of the 'i' loop, then, we would need to use mutexes to protect the access
c[i+j] += ...
The problem is that the work is not evenly distributed on ranges of 'c'. Therefore,
we will compute, for each position 'k', how many pairs (i, j) exist such that i+j=k.
*/

const int NUM_TASKS = 8;
const int NUM_THREADS = 8;
ThreadPool threadPool(NUM_THREADS);

vector<int> a, b, c;
vector<pair<int, int>> work;

// computes multiplication on range [l, r) of 'c'
void f(pair<int, int> &range) {
    auto[l, r] = range;
    for (int k = l; k < r; k++)
        for (int i = work[k].first; i <= work[k].second; i++)
            c[k] += a[i] * b[k-i];
}

int computeWork() {
    assert(a.size());
    int n = a.size();
    int m = b.size();
    work.assign(n+m-1, {0, -1});

    // work[k]: how many pairs (i,j) satisfy i+j=k
    int sum = 0;
    for (int k = 0; k < work.size(); k++) {
        // smallest 'i' such that there is a 'j' such that i+j==k
        int l = min(k, n-1);
        int r = min(k, m-1);
        int smallest = k-r;
        int largest = l;
        if (smallest <= largest) {
            work[k] = {smallest, largest};
            sum += largest - smallest + 1;
        }
    }

    return sum;
}

vector<pair<int, int>> getPartitions(int sum) {
    long long target = (sum+NUM_TASKS-1) / NUM_TASKS;
    vector<pair<int, int>> ret(NUM_TASKS);
    int prev = 0;
    int i = 0;
    for (int t = 0; t < NUM_TASKS; t++) {
        long long curSum = 0;
        while (curSum < target && i < work.size()) {
            curSum += work[i].second - work[i].first + 1;
            i++;
        }
        ret[t] = {prev, i};
        prev = i;
    }
    return ret;
}

signed main() {
    ios::sync_with_stdio(0); cin.tie(0);

    // n: degree of first polynomial
    // m: degree of second polynomial
    int n, m; cin >> n >> m;
    a.resize(n+1); b.resize(m+1);
    for (int &i : a) cin >> i;
    for (int &i : b) cin >> i;

    int sum = computeWork();
    vector<pair<int, int>> partitions = getPartitions(sum);

    auto start = chrono::high_resolution_clock::now();
    c.assign(n+m+1, 0);

    vector<int> ids(NUM_TASKS);
    for (int t = 0; t < NUM_TASKS; t++)
        ids[t] = threadPool.execute(f, partitions[t]);
    for (int t = 0; t < NUM_TASKS; t++)
        threadPool.join(ids[t]);
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Time to multiply polys of sizes " << a.size() << " and " << b.size() << " (in ms): " << duration.count() << endl;
    //cout << "c = "; for (int i : c) cout << i << " "; cout << endl;
    
    return 0;
}