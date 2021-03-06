#include <fstream> // for greed plugin
#include <iostream>
#include <algorithm> // max,min
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <iostream>
#include <utility>
#include <cctype>
#include <queue>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <limits.h>
#include <cstring>
#include <tuple>
#include <cassert>
using namespace std;

// type alias
typedef unsigned long UL;
typedef long long LL;

typedef vector < bool > VB;
typedef vector < int > VI;
typedef vector < string > VS;
typedef vector < vector < int > > VVI;
typedef unordered_map < int, int > MAPII;
typedef unordered_map < string, int > MAPSI;
typedef unordered_set < int > SETI;
typedef unordered_set < string > SETS;

// repetition
#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define ROF(i,a,b) for(int i=(a);i>=(b);--i)
#define REP(i,n)  for(int i=0;i<(n);++i)
#define FORR(x,arr) for(auto& x:arr)
#define SZ(a) int((a).size())
#define REPS(i,arr)  for(int i=0; i<(arr).size(); ++i)

// vector
#define PUSH(s, e) (s).push_back(e)
#define POP(s) (s).pop_back()
#define REVERSE(c) reverse((c).begin(),(c).end())
#define SORT(c) sort((c).begin(),(c).end())
#define CONCAT(a,b) (a).insert((a).begin(), (b).begin(),(b).end())
#define ERASE(arr,i) (arr).erase((arr).begin()+i)

// pair/tuple
typedef pair< int , int > II;
typedef vector<II> VII;
#define MP make_pair
#define _1 first
#define _2 second
typedef tuple< int , int, int > III;
#define MT make_tuple

// matrix
vector< pair < int, int > >  moves = { {0,1},{0,-1},{1,0},{-1,0} };

// priority queue
#define MAX_PQ(T) priority_queue<T>
#define MIN_PQ(T) priority_queue<T,vector<T>,greater<T>>

// binary search
#define LB(a,n,k) lower_bound(a,a+(n),(k))
#define UB(a,n,k) upper_bound(a,a+(n),(k))

// dp
#define DP(dp,i,j) int dp[i][j]
#define FILL(dp,x) memset(dp, x, sizeof(dp))
#define MINUS(dp) memset(dp, -1, sizeof(dp))
#define ZERO(dp) memset(dp, 0, sizeof(dp))

// debug cerr
#define dump(x)  cerr << #x << " = " << (x) << endl;
#define dump2(x,y)  cerr << #x << " = " << (x) << ", " << #y << " = " << (y) << endl;
#define dump3(x,y,z)  cerr << #x << " = " << (x) << ", " << #y << " = " << (y) << ", " << #z << " = " << (z) << endl;
#define dump4(x,y,z,a)  cerr << #x << " = " << (x) << ", " << #y << " = " << (y) << ", " << #z << " = " << (z) << ", " << #a << " = " << (a) << endl;
#define dumpAR(ar) FORR(x,(ar)) { cout << x << ','; } cerr << endl;

/*
 6/29/2017
 
 8:29 start
 9:05 got it. +1 for each even/odd
 9:33 hmmm, ok not necessarily +1...  gave up
 
 6/30/2017
 
 read edtorials..
 https://www.topcoder.com/blog/single-round-match-714-editorials/
 http://kmjp.hatenablog.jp/entry/2017/05/10/1000
 
 20:20-20:40 passed samples
 20:44 passed system tests
 */

class NAddOdd {
public:
  long long solve(long long L, long long R, int K) {
    return dfs(R, K) - dfs(L-1, K);
  }
  
  unordered_map <LL, LL> memo;
  LL dfs(LL x, LL K) {
    if (x<=K) return 0;
    if (memo.count(x)) return memo[x];
    
    LL res = 0;
    
    LL ce = x/2 - K/2;
    LL co = (x+1)/2 - (K+1)/2;
    assert(co==x-K-ce);
    
    res += ce + dfs(x/2, K);
    res += 2*co + dfs((x+K)/2, K);
    
    return memo[x] = res;
  }
};
class NAddOdd_wrong {
  public:
  long long solve(long long L, long long R, int K) {
    if (L<=K) L = K+1;
    if (L==R) return helper(L,K);
    if (L>R) return 0;
    
    LL s1 = L;
    LL s2 = L+1;
    
    LL e1,e2;
    if (L%2==R%2) {
      e1 = R;
      e2 = R-1;
    } else {
      e1 = R-1;
      e2 = R;
    }
    e1 = max(s1,e1);
    e2 = max(s2,e2);
    
    dump2(s1,e1);
    dump2(s2,e2);
    
    LL c1=(e1-s1)/2, c2=(e2-s2)/2;
    
    dump2(c1,helper(s1,K));
    dump2(c2,helper(s2,K));
    
    if (K==74935) {
      REP(i,100) {
        dump4(s1+2*i,helper(s1+2*i,K),s2+2*i,helper(s2+2*i,K));
      }
    }
    
    LL res = (c1+1)*helper(s1,K) + c1*(c1-1)/2 + (c2+1)*helper(s2,K) + c2*(c2-1)/2;
    return res;
  }
  
  LL helper(LL n, int K) {
    if (n<=K) return 0;
    
    if (n%2) return helper(n+K, K) + 1;
    else return helper(n/2, K) + 1;
  }
};

// CUT begin
ifstream data("NAddOdd.sample");

string next_line() {
    string s;
    getline(data, s);
    return s;
}

template <typename T> void from_stream(T &t) {
    stringstream ss(next_line());
    ss >> t;
}

void from_stream(string &s) {
    s = next_line();
}

template <typename T>
string to_string(T t) {
    stringstream s;
    s << t;
    return s.str();
}

string to_string(string t) {
    return "\"" + t + "\"";
}

bool do_test(long long L, long long R, int K, long long __expected) {
    time_t startClock = clock();
    NAddOdd *instance = new NAddOdd();
    long long __result = instance->solve(L, R, K);
    double elapsed = (double)(clock() - startClock) / CLOCKS_PER_SEC;
    delete instance;

    if (__result == __expected) {
        cout << "PASSED!" << " (" << elapsed << " seconds)" << endl;
        return true;
    }
    else {
        cout << "FAILED!" << " (" << elapsed << " seconds)" << endl;
        cout << "           Expected: " << to_string(__expected) << endl;
        cout << "           Received: " << to_string(__result) << endl;
        return false;
    }
}

int run_test(bool mainProcess, const set<int> &case_set, const string command) {
    int cases = 0, passed = 0;
    while (true) {
        if (next_line().find("--") != 0)
            break;
        long long L;
        from_stream(L);
        long long R;
        from_stream(R);
        int K;
        from_stream(K);
        next_line();
        long long __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases - 1) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... ";
        if ( do_test(L, R, K, __answer)) {
            passed++;
        }
    }
    if (mainProcess) {
        cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;
        int T = time(NULL) - 1498750115;
        double PT = T / 60.0, TT = 75.0;
        cout << "Time   : " << T / 60 << " minutes " << T % 60 << " secs" << endl;
        cout << "Score  : " << 500 * (0.3 + (0.7 * TT * TT) / (10.0 * PT * PT + TT * TT)) << " points" << endl;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
    set<int> cases;
    bool mainProcess = true;
    for (int i = 1; i < argc; ++i) {
        if ( string(argv[i]) == "-") {
            mainProcess = false;
        } else {
            cases.insert(atoi(argv[i]));
        }
    }
    if (mainProcess) {
        cout << "NAddOdd (500 Points)" << endl << endl;
    }
    return run_test(mainProcess, cases, argv[0]);
}
// CUT end
