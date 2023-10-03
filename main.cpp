#include <iostream>
#include <vector>
#include <queue>

template <class T>
using Vector = std::vector <T>;

template <class T>
using Matrix = Vector <Vector <T> >;
/**
 * @brief readMatrix
 * reads all matrix elements
 * @param x
 * the matrix
 */
template <class T>
void readMatrix (Matrix<T>& x) {
    for (int i = 0; i < x.size(); ++i) {
        for (int j = 0; j < x[i].size(); ++j) {
            std::cin >> x[i][j];
        }
    }
}

template <class T>
/**
 * @brief isCompatiable
 * checks if all dimensions of matrices are equal, i.e. both are square matrix of equal size
 * @param x1 matrix no1
 * @param x2 matrix no2
 * @return
 * true if they are equal sized, false otherwise
 */
bool isCompatiable (const Matrix<T>& x1, const Matrix<T>& x2) {
    int n = x1.size();
    if (x2.size() != n) return false;
    for (int i = 0; i < x1.size(); ++i) {
        if (x1[i].size() != n || x2[i].size() != n) {
            return false;
        }
    }
    return true;
}

/**
 * @brief stable
 * solves the stable marriage problem
 * @param mMat
 * men matrix, all i men prefer most j=0 then j=1... j=n m[i][0], m[i][1]... m[i][n]
 * @param wMat
 * women matrix
 * @return
 * correspondence, each i element specifies 0 based index of his pair woman
 */
Vector <int> stable (const Matrix<int>& mMat, const Matrix<int>& wMat) {
    //std::cout << "started\n";
    if (!isCompatiable (mMat, wMat)) {
        return Vector<int>();
    }
    //std::cout << "compatiable\n";
    int n = mMat.size();

    //mens pending proposals
    Vector <std::queue <int> > mProp (n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mProp[i].push(mMat[i][j]);
        }
    }
    //free mens queue
    std::queue <int> mFree;
    for (int i = 0; i < n; ++i) {
        mFree.push (i);
    }
    //womans preference for each men, bigger value is better
    Matrix <int> wPref(n, Vector <int>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (wMat[i][j] < 0 || wMat[i][j] >= n) {
                std::cerr << "Range check error: wMat value is out of range" << std::endl;
                return Vector<int>();
            }
            wPref[i].at (wMat[i][j]) = n-j-1;
        }
    }
    //mens and womens result is still not found
    Vector <int> mRes(n, -1);
    Vector <int> wRes(n, -1);

    while (!mFree.empty()) {
        int t = mFree.front();
        mFree.pop();
        int x = mProp[t].front();
        mProp[t].pop();
        if (x < 0 || x >= n || t < 0 || t >= n) {
            std::cerr << "Range check error in main loop" << std::endl;
            return Vector<int>();
        }
        if (wRes[x] < 0) {
            wRes[x] = t;
            mRes[t] = x;
        } else if (wPref[x][wRes[x]] < wPref[x][t]) {
            mFree.push (wRes[x]);
            mRes [wRes[x]] = -1;
            wRes[x] = t;
            mRes[t] = x;
        } else {
            mFree.push (t);
        }
    }
    return mRes;
}

int main()
{
    int n;
    std::cin >> n;
    Matrix<int> m(n, Vector<int>(n));
    Matrix<int> w(n, Vector<int>(n));
    readMatrix (m);
    readMatrix (w);
    //std::cout << "m, w size " << m.size() << " " << w.size() << std::endl;
    Vector <int> mar = stable (m,w);
    std::cout << mar.size() << std::endl;
    for (int i = 0; i < mar.size(); ++i) {
        std::cout << mar[i] << " ";
    }
    return 0;
}

