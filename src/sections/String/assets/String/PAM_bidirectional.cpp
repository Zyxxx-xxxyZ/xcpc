// 之前 PAM 的板子中，通过设置位置 0 为哨兵节点，因此不需要限制范围
// 本板子采用 while 循环中限制范围的做法来规避风险
const int N = 200005;
struct PAM_bidirectional {
    int ch[N][26], len[N], fail[N], tot = 1;
    int last_front = 1, last_back = 1;  // 维护前后端指针
    char s[N * 2];                      // 双向字符数组，首尾留足 buffer
    int L = N, R = N - 1;               // 初始指针居中

    void init() {
        memset(ch, 0, sizeof(ch));
        memset(len, 0, sizeof(len));
        memset(fail, 0, sizeof(fail));
        tot = 1;
        last_front = last_back = 1;
        L = N; R = N - 1;
        fail[0] = 1; len[1] = -1;
    }

    void push_back(char c) {
        s[++R] = c;
        int v = c - 'a', p = last_back;
        while (R - len[p] - 1 < L || s[R - len[p] - 1] != s[R]) p = fail[p];
        if (!ch[p][v]) {
            int cur = ++tot, j = fail[p];
            len[cur] = len[p] + 2;
            while (R - len[j] - 1 < L || s[R - len[j] - 1] != s[R]) j = fail[j];
            fail[cur] = ch[j][v];
            ch[p][v] = cur;
        }
        last_back = ch[p][v];
        if (len[last_back] == R - L + 1) last_front = last_back;
    }

    void push_front(char c) {
        s[--L] = c;
        int v = c - 'a', p = last_front;
        while (L + len[p] + 1 > R || s[L + len[p] + 1] != s[L]) p = fail[p];
        if (!ch[p][v]) {
            int cur = ++tot, j = fail[p];
            len[cur] = len[p] + 2;
            while (L + len[j] + 1 > R || s[L + len[j] + 1] != s[L]) j = fail[j];
            fail[cur] = ch[j][v];
            ch[p][v] = cur;
        }
        last_front = ch[p][v];
        if (len[last_front] == R - L + 1) last_back = last_front;
    }
};