#include <bits/stdc++.h>
using namespace std;

signed main() {
    ios::sync_with_stdio(0); cin.tie(0);

    // n: degree of first polynomial
    // m: degree of second polynomial
    int n, m; cin >> n >> m;
    vector<int> a(n+1), b(m+1);
    for (int &i : a) cin >> i;
    for (int &i : b) cin >> i;

    /*cout << "a = "; for (int i : a) cout << i << " "; cout << endl;
    cout << "b = "; for (int i : b) cout << i << " "; cout << endl;*/

    auto start = chrono::high_resolution_clock::now();
    vector<int> c(n+m+1);
    for (int i = 0; i < a.size(); i++)
        for (int j = 0; j < b.size(); j++)
            c[i+j] += a[i] * b[j];
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Time to multiply polys of sizes " << a.size() << " and " << b.size() << " (in ms): " << duration.count() << endl;
    //cout << "c = "; for (int i : c) cout << i << " "; cout << endl;
    
    return 0;
}