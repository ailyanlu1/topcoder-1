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
#include <numeric>
using namespace std;

// type alias
typedef long long LL;

typedef vector < int > VI;
typedef unordered_map < int, int > MAPII;
typedef unordered_set < int > SETI;
typedef pair< int , int > II;
typedef tuple< int, int, int > III;

// repetition
#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define REP(i,n)  for(int i=0;i<(n);++i)
#define FORR(x,arr) for(auto& x:arr)
#define SZ(a) int((a).size())

// DP
#define MINUS(dp) memset(dp, -1, sizeof(dp))
#define ZERO(dp) memset(dp, 0, sizeof(dp))

// debug cout
#define dump(x)  cout << #x << " = " << (x) << endl;
#define dump2(x,y)  cout << #x << " = " << (x) << ", " << #y << " = " << (y) << endl;
#define dump3(x,y,z)  cout << #x << " = " << (x) << ", " << #y << " = " << (y) << ", " << #z << " = " << (z) << endl;
#define dump4(x,y,z,a)  cout << #x << " = " << (x) << ", " << #y << " = " << (y) << ", " << #z << " = " << (z) << ", " << #a << " = " << (a) << endl;
#define dumpAR(ar) FORR(x,(ar)) { cout << x << ','; } cout << endl;

/*
 
 10/11/2017
 
 5:44-6:42 pause
 8:00-9:30 give up
 
 10/12/2017
 
 6:30-7:30,8:55-10:16 read editorials:
  - https://apps.topcoder.com/wiki/display/tc/SRM+614
    - https://apps.topcoder.com/forums/?module=Thread&threadID=814614&start=0
  - http://kmjp.hatenablog.jp/entry/2014/04/01/0930
  - https://www.slideshare.net/EmKjp/srm614-34320184
  - http://torus711.hatenablog.com/entry/20140331/1396285929
 
 Firstly find reachable cells and number of steps to reach goal.
 
 Obviously { (x%N, x%M) : 0<=x } is reachable cells.
 We can also find number of steps `K` in below constraints:
   K≡GX (mod N)
   K≡GY (mod M)

 Solution of modular linear equation is represented as K % Q.
 
 There are two ways to solve this:
   1. brute force Q and find K (This is feasible because N,M<=10)
   2. solve linear congruence by mod inverse (extended GCD)

 Now we figure out that reachable cell is { (x%N, x%M) : 0<=x<=Q }.
 
 So there are Q reachable unique cells.
 We can form graph with Q cells that is connected by undirected edge.
 
 In next step, we want to compute expected value.
 
 Suppose E[x] shows expected vaue of first time to reach at vertex `x`.
 We can build recurrence relation:
   E[x] = (E[x-1] + E[x+1])/2 + 1, x=0..Q
   E[0] = 0
 
 One possible solution of this system of linear equations is making E[x] = a[x]*E[1]+b[x] as described in official editorial.
 
 base case: a[0]=b[0]=0, a[1]=1 b[1]=0
 E[x]
 = 2*E[x-1]-E[x-2]-2
 = 2*(a[x-1]*E[1]+b[x-1])-(a[x-2]*E[1]+b[x-2])-2
 = (2*a[x-1]-a[x-2]) * E[1] + 2*b[x-1]-b[x-2]-2

 E[x] = a[x]*E[1]+b[x]
 => a[x] = 2*a[x-1]-a[x-2], b[x] = 2*b[x-1]-b[x-2]-2
 
 E[0] = E[Q] = 0 = a[Q]*E[1]+b[Q]
 => E[1] = -b[Q]/a[Q]
 
 res = E[K] = a[K]*E[1]+b[K]
 
 Actually E[1] = Q-1 and E[K] = K*(Q-K) as shown in articles of "hitting time"
 
 Another solution is to build linear equations from relationship of expected value and solve it
   - linear equation can be built from expected value
     - ant book 4-1 complex math problem (problem "Random Walk")
   - linear equation can be solved by Gauss-Jordan elimination
   - in this particular problem, we can compute variable in O(1) by analysis
 
 10:16-10:39,22:30-22:37 add solution by solving modular linear equation by brute force search and linear equation of expected value in adhoc way described in official editorial.
 
 * References
 
 Solver of modular linear equations:
  - ant book 4-1 complex math problem
  - CLRS 31.4 modular linear equations

 Solver of linear equations by Gaussian elimination (Gauss-Jordan elimination):
  - https://en.wikipedia.org/wiki/Gaussian_elimination
  - ant book 4-1 complex math problem
  - CLRS 28.1 Solving systems of linear equations
 
 First hitting time or one dimensional randome walk:
  - http://galton.uchicago.edu/~lalley/Courses/312/RW.pdf
  - https://en.wikipedia.org/wiki/First-hitting-time_model
  - https://en.wikipedia.org/wiki/Hitting_time
  - https://math.stackexchange.com/questions/261889/expectation-of-stopping-time-w-r-t-a-brownian-motion
 
 summary:
  - look at size constraint carefully
  - there are some smart techniques unerneath this problem though, we can avoid it by analysis or brute force
   - this is typical div2 hard in this perspective
 
 */

int gcd(int a, int b) { return b==0?a:gcd(b,a%b); }

// brute force
class TorusSailingEasy {
public:
  double expectedTime(int N, int M, int GX, int GY) {
    int Q=N*M;
    int x=0,y=0;
    FOR(i,1,N*M) {
      ++x,++y;
      x%=M,y%=N;
      if(x==0&&y==0) Q=min(Q,i);
    }
    int K=-1;
    FOR(j,1,Q) if(j%N==GX&&j%M==GY) K=j;
    if(K==-1) return -1.0;
    
    vector<double> A(Q+1),B(Q+1);
    A[0]=B[0]=0;
    A[1]=1,B[1]=0;
    FOR(i,2,Q+1) {
      A[i]=2*A[i-1]-A[i-2];
      B[i]=2*B[i-1]-B[i-2]-2;
    }
    double E1=-B[Q]/A[Q];
    return A[K]*E1+B[K];
  }
};

tuple<int,int,int> gcd_extended(int a, int b) {
  if (b==0) return make_tuple(a, 1, 0);
  
  int d2,x2,y2;
  tie(d2,x2,y2) = gcd_extended(b, a%b);
  return make_tuple(d2,y2,x2-(a/b)*y2);
}

int extgcd(int a, int b, int& x, int& y) { int d = a;
  if (b != 0) {
    d = extgcd(b, a % b, y, x);
    y -= (a / b) * x; } else {
      x = 1; y = 0; }
  return d;
}
int mod_inverse(int a, int m) { int x, y;
  extgcd(a, m, x, y);
  return (m + x % m) % m;
}

pair<int, int> linear_congruence(const vector<int>& A, const vector<int>& B,
                                 const vector<int>& M) {
  int x = 0, m = 1;
  for (int i = 0; i < A.size(); i++) {
    int a = A[i] * m, b = B[i] - A[i] * x, d = gcd(M[i], a); if (b % d != 0) return make_pair(0, -1); // 解がない
    int t = b / d * mod_inverse(a / d, M[i] / d) % (M[i] / d); x = x + m * t;
    m *= M[i] / d;
  }
  return make_pair(x % m, m);
}

const double EPS = 1E-8;
typedef vector<double> vec;
typedef vector<vec> mat;

vec gauss_jordan(const mat& A, const vec& b) {
  int n = A.size();
  mat B(n, vec(n + 1));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) B[i][j] = A[i][j]; // 行列Aの後ろにbを並べ同時に処理する
  for (int i = 0; i < n; i++) B[i][n] = b[i];
  for (int i = 0; i < n; i++) {
    // 注目している変数の係数の絶対値が大きい式をi番目に持ってくる
    int pivot = i;
    for (int j = i; j < n; j++) {
      if (abs(B[j][i]) > abs(B[pivot][i])) pivot = j; }
    swap(B[i], B[pivot]);
    // 解がないか、一意でない
    if (abs(B[i][i]) < EPS) return vec();
    // 注目している変数の係数を1にする
    for (int j = i + 1; j <= n; j++) B[i][j] /= B[i][i]; for (int j = 0; j < n; j++) {
      if (i != j) {
        // j番目の式からi番目の変数を消去
        for (int k = i + 1; k <= n; k++) B[j][k] -= B[j][i] * B[i][k];
      } }
  }
  vec x(n);
  // 後ろに並べたbが解になる
  for (int i = 0; i < n; i++) x[i] = B[i][n]; return x;
}

//int dp[51];
class TorusSailingEasy_org {
  public:
  double expectedTime(int N, int M, int GX, int GY) {
    /*
    if(N<M) swap(N,M),swap(GX,GY);
    if(gcd(N,M)!=1&&abs(GX-GY)%gcd(N,M)) return -1.0;
    */
    
    auto p=linear_congruence({1,1},{GX,GY},{N,M});
    if(p.first==0&&p.second==-1) return -1.0;
    if(p.first<0) p.first+=p.second;
    dump3(p.first,p.second,p.second-p.first);
    return p.first*(p.second-p.first);
    
    /*
    int MM=p.second;
    
    mat A(MM,vec(MM,0)); vec b(MM,0);
    REP(x,MM) {
      if(x==p.first) {
        A[x][x]=1;
        continue;
      }
      int move=0;
      if(x>1) A[x][x-1]=-1,move++;
      if(x<MM-1) A[x][x+1]=-1,move++;
      b[x]=A[x][x]=move;
    }
    
    vec res = gauss_jordan(A,b);
    
    return res[0];*/
  }
};

// CUT begin
ifstream data("TorusSailingEasy.sample");

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

bool double_equal(const double &a, const double &b) { return b==b && a==a && fabs(b - a) <= 1e-9 * max(1.0, fabs(a) ); }

bool do_test(int N, int M, int goalX, int goalY, double __expected) {
    time_t startClock = clock();
    TorusSailingEasy *instance = new TorusSailingEasy();
    double __result = instance->expectedTime(N, M, goalX, goalY);
    double elapsed = (double)(clock() - startClock) / CLOCKS_PER_SEC;
    delete instance;

    if (double_equal(__expected, __result)) {
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
        int N;
        from_stream(N);
        int M;
        from_stream(M);
        int goalX;
        from_stream(goalX);
        int goalY;
        from_stream(goalY);
        next_line();
        double __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases - 1) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... ";
        if ( do_test(N, M, goalX, goalY, __answer)) {
            passed++;
        }
    }
    if (mainProcess) {
        cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;
        int T = time(NULL) - 1507725878;
        double PT = T / 60.0, TT = 75.0;
        cout << "Time   : " << T / 60 << " minutes " << T % 60 << " secs" << endl;
        cout << "Score  : " << 1000 * (0.3 + (0.7 * TT * TT) / (10.0 * PT * PT + TT * TT)) << " points" << endl;
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
        cout << "TorusSailingEasy (1000 Points)" << endl << endl;
    }
    return run_test(mainProcess, cases, argv[0]);
}
// CUT end
