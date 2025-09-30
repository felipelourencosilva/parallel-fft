#include <bits/stdc++.h>
using namespace std;

signed main() {
    ios::sync_with_stdio(0); cin.tie(0);
    random_device rd;
    mt19937 gen(rd());

    // n: degree of first polynomial
    // m: degree of second polynomial
    // maxRand: max value for the coefficients
    int n, m, maxRand; cin >> n >> m >> maxRand;
    int minRand = 1;

    uniform_int_distribution<int> distribution(minRand, maxRand);

    vector<int> a(n+1), b(m+1);
    for(int &i : a) i = distribution(gen);
    for(int &i : b) i = distribution(gen);

    cout << "a = "; for (int i : a) cout << i << " "; cout << endl;
    cout << "b = "; for (int i : b) cout << i << " "; cout << endl;

    vector<int> c(n+m+1);
    for (int i = 0; i < a.size(); i++)
        for (int j = 0; j < b.size(); j++)
            c[i+j] += a[i] * b[j];

    cout << "c = "; for (int i : c) cout << i << " "; cout << endl;

    return 0;
}