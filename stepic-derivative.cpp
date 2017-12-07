//
// Created by goforbroke on 05.12.17.
//

// g++ --std=c++11 -o stepic-derivative stepic-derivative.cpp

#include <iostream>
#include <list>
#include <vector>

std::vector<std::string> strSplit(std::string str, std::vector<std::string> delimiters) {
    std::vector<std::string> r;

    size_t pos = 0;
    std::string delimiter;

    do {
        pos = str.length();

        for (auto &it : delimiters) {
            size_t p = str.find(it);
            if (p < pos && p != std::string::npos) {
                pos = p;
                delimiter = it;
            }
        }

        r.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    } while (str.length() > 0);

    return r;
}

template<typename T, typename P>
T remove_if(T beg, T end, P pred) {
    T dest = beg;
    for (T itr = beg; itr != end; ++itr)
        if (!pred(*itr))
            *(dest++) = *itr;
    return dest;
}

class DerivativeMember {
    static DerivativeMember *tmp_member;
    static std::vector<std::string> tmp_parts;

public:
    int multiply;
    int degree;

    std::string toString() {
        if (0 == degree) return std::to_string(multiply);
        if (1 == degree) return std::to_string(multiply) + "*x";
        return std::to_string(multiply) + "*x^" + std::to_string(degree);
    }

    static DerivativeMember *fromString(std::string &str) {
        tmp_member = new DerivativeMember();
        tmp_parts = strSplit(str, {"*x^", "*x", "x^", "x"});
//        std::cout << tmp_parts[0] << std::endl;
        if (tmp_parts[0] == "-")
            tmp_member->multiply = -1;
        else if (tmp_parts[0].empty())
            tmp_member->multiply = 1;
        else
            tmp_member->multiply = std::atoi(tmp_parts[0].c_str());
        tmp_member->degree = (tmp_parts.size() > 1) ? std::atoi(tmp_parts[1].c_str()) : 1;
        return tmp_member;
    }
};

DerivativeMember *DerivativeMember::tmp_member;
std::vector<std::string> DerivativeMember::tmp_parts;

bool is_number(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

//template<class TBase>
class DerivativeVector {
private:
    std::list<DerivativeMember *> m_parts;

    static std::vector<std::string> strSplit(std::string str) {
        std::vector<std::string> delimiters = {"+", "-"};
        std::vector<std::string> r;

        size_t pos = 0;
        size_t p;
        std::string delimiter;
        std::string old_delimiter;

        std::vector<std::string>::iterator it;

        do {
            pos = str.length();

            for (it = delimiters.begin(); it != delimiters.end(); it++) {
                p = str.find(*it);
                if (p < pos && p != std::string::npos) {
                    pos = p;
                    delimiter = *it;
                }
            }

            r.push_back((old_delimiter == "-" ? "-" : "") + str.substr(0, pos));
            str.erase(0, pos + delimiter.length());

            old_delimiter = delimiter;
        } while (str.length() > 0);

        return r;
    }

public:
    void appendAll(std::string &strParts) {

        strParts.erase(remove_if(strParts.begin(), strParts.end(), isspace), strParts.end());
        std::vector<std::string> parts = DerivativeVector::strSplit(strParts);

        for (auto &part : parts) {
            if (part.empty() || is_number(part)) continue;

            m_parts.push_back(DerivativeMember::fromString(part));
        }
    }

    void runDerivative() {
        for (auto it = m_parts.begin(); it != m_parts.end();) {
            DerivativeMember *m = (*it);
            if (0 == m->degree) {
                it = m_parts.erase(it);
            } else {
                m->multiply *= m->degree;
                m->degree--;
                it++;
            }
        }
    }

    void simplify() {
        DerivativeMember *m;
        std::list<DerivativeMember *> tmp_parts;

        DerivativeMember *fixed;

        for (auto it1 = m_parts.begin(); it1 != m_parts.end();) {

            fixed = (*it1);

            for (auto it2 = m_parts.begin(); it2 != m_parts.end();) {
                if (it1 == it2) {
                    ++it2;
                    continue;
                }
                if ((*it1)->degree == (*it2)->degree) {
                    fixed->multiply += (*it2)->multiply;
                    it2 = m_parts.erase(it2);
                    continue;
                }
                ++it2;
            }

            it1 = m_parts.erase(it1);

            tmp_parts.push_back(fixed);
        }

        for (auto &m_part : m_parts) {
            delete m_part;
        }
        m_parts.clear();
        m_parts = tmp_parts;
    }

    void sortByDegree() {
        // todo:
    }

    std::string toString() {
        std::string r;
        for (auto it = m_parts.begin(); it != m_parts.end(); it++) {
            r += ((it != m_parts.begin() && (*it)->multiply >= 0) ? "+" : "") + (*it)->toString();
        }
        return r;
    }
};

std::string derivative(std::string polynomial) {
    auto *v = new DerivativeVector;
    v->appendAll(polynomial);
    v->simplify();
    v->runDerivative();
    v->sortByDegree();
    const std::string &res = (std::string) v->toString();
    delete (v);
    return res;
}

int main(int argc, char **argv) {
//    std::cout << derivative("200*x^99 - 200*x^5 + 10*x^10 + 2*x^5");
//    std::cout << derivative("x^2+x") << " == 2*x+1" << std::endl;
//    std::cout << derivative("2*x^100+100*x^2") << " == 200*x^99+200*x" << std::endl;
//    std::cout << derivative("x^10000+x+1") << " == 10000*x^9999+1" << std::endl;
//    std::cout << derivative("x^10000+x+1") << " == 10000*x^9999+1" << std::endl;
    std::cout << derivative("-x^2-x^3") << " == -3*x^2-2*x" << std::endl;

//    std::string formula;
//    std::cin >> formula;
//    std::cout << derivative(formula);

    return 0;
}
