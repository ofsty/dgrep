#include "bmh.hpp"
#include <iostream>
#include <sstream>
#include <mutex>

static std::mutex _mu_88;

void xx44(const std::string& a1, std::vector<int>& b1) {
    size_t c1 = a1.size();
    const int D = 256;
    b1.assign(D, (int)c1);
    for (size_t e1 = 0; e1 < c1 - 1; ++e1)
        b1[(unsigned char)a1[e1]] = (int)(c1 - 1 - e1);
}

size_t zz99(const char* f1, size_t g1, const std::string& h1,
            const std::vector<int>& i1, std::vector<size_t>& j1) {
    size_t cnt = 0;
    size_t m1 = h1.size();
    if (m1 == 0 || g1 < m1) return 0;

    size_t idx = 0;
    while (idx <= g1 - m1) {
        int j = (int)m1 - 1;
        while (j >= 0 && h1[j] == f1[idx + j]) j--;
        if (j < 0) {
            j1.push_back(idx);
            cnt++;
            idx += m1;
        } else {
            int sh = i1[(unsigned char)f1[idx + m1 - 1]];
            if (sh <= 0) sh = 1;
            idx += sh;
        }
    }
    return cnt;
}

void pp77(const std::vector<char>& buf, size_t st, size_t ed,
          const std::string& pat, const std::vector<int>& tbl,
          size_t& cnt_out, size_t base_line) {
    std::vector<size_t> matches;
    size_t len = ed - st;
    size_t found = zz99(buf.data() + st, len, pat, tbl, matches);

    std::ostringstream out;

    for (size_t i = 0; i < matches.size(); ++i) {
        size_t mpos = st + matches[i];

        size_t lstart = mpos;
        while (lstart > st && buf[lstart - 1] != '\n' && buf[lstart - 1] != '\r') lstart--;

        size_t lend = mpos;
        while (lend < ed && buf[lend] != '\n' && buf[lend] != '\r') lend++;

        std::string line_text(&buf[lstart], lend - lstart);

        if (!line_text.empty() && line_text.back() == '\r') {
            line_text.pop_back();
        }

        out << line_text << "\n";
    }

    {
        std::lock_guard<std::mutex> _lk(_mu_88);
        std::cout << out.str();
    }

    cnt_out = found;
}
