#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

#define all(x) x.begin(), x.end()

const long double PI = 4*atanl(1);
using cd = complex<long double>;

void fft(vector<cd> &a) {
    int n = a.size();
    if (n == 1) return; // because a_0*z^0 = a_0 (evaluating at any point returns a_0)
    vector<cd> a0(n/2), a1(n/2);
    for (int i = 0; i < n/2; i++) {
        a0[i] = a[2*i];
        a1[i] = a[2*i+1];
    }
    fft(a0); fft(a1);
    long double alpha = 2*PI/n;
    cd omega(cos(alpha), sin(alpha));
    cd omega_i = 1;

    for (int i = 0; i < n/2; i++) {
        a[i] = a0[i] + omega_i * a1[i];
        a[i+n/2] = a0[i] - omega_i * a1[i]; // -omega_i*a1[i] == (-1)*omega_i*a1[i] == omega_(n/2)*omega_i*a1[i] == omega_(i+n/2)*a1[i]
        omega_i *= omega;
    }
}

void ifft(vector<cd> &a) { // DFT(DFT(c0, c1, c2, ..., c(n-1))) = (nc0, nc(n-1), nc(n-2), ..., nc1)
    fft(a);
    reverse(a.begin()+1, a.end());
    int n = a.size();
    for (cd &i : a)
        i /= n;
}

vector<int> a, b, c;

void* multiply(void* args) {
    vector<cd> fa(all(a)), fb(all(b));
    int n = 1;
    while (n < a.size() + b.size() - 1)
        n *= 2;
    fa.resize(n); fb.resize(n);
    fft(fa); fft(fb);

    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];

    ifft(fa);
    c.resize(n);
    for (int i = 0; i < n; i++)
        c[i] = round(fa[i].real());

    return nullptr;
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    // n: degree of first polynomial
    // m: degree of second polynomial
    int n, m; cin >> n >> m;
    a.resize(n+1); b.resize(m+1);
    for (int &i : a) cin >> i;
    for (int &i : b) cin >> i;

    cout << "a = "; for (int i : a) cout << i << " "; cout << endl;
    cout << "b = "; for (int i : b) cout << i << " "; cout << endl;

    auto start = chrono::high_resolution_clock::now();

    pthread_t thread;
    pthread_create(&thread, NULL, multiply, NULL);
    pthread_join(thread, NULL);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "c = "; for (int i : c) cout << i << " "; cout << endl;
    cout << "Time to multiply polys of sizes " << a.size() << " and " << b.size() << " (in ms): " << duration.count() << endl;

    return 0;
}