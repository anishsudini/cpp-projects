#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <mutex>
#include <thread>
#include "poly.h"

void polynomial::internal_canonical_form() {
    for (auto iter = terms.begin(); iter != terms.end();) {
        if (iter->second == 0) {
            iter = terms.erase(iter);
        } else {
            ++iter;
        }
    }
}

polynomial::polynomial() {
    terms[0] = 0;
}

polynomial::polynomial(const polynomial &other) : terms(other.terms) {}

void polynomial::print() const {
    for (const auto &[p, c] : terms) {
        std::cout << c << "x^" << p << " ";
    }
    std::cout << std::endl;
}

polynomial &polynomial::operator=(const polynomial &other) {
    if (this != &other) {
        terms = other.terms;
    }
    return *this;
}

polynomial operator+(const polynomial &x, const polynomial &y) {
    polynomial result = x;
    for (const auto &[p, c] : y.terms) {
        result.terms[p] += c;
    }
    result.internal_canonical_form();
    return result;
}

polynomial operator+(const polynomial &x, int y) {
    polynomial result = x;
    result.terms[0] += y;
    return result;
}

polynomial operator+(int x, const polynomial &y) {
    return y + x;
}


polynomial operator*(const polynomial &x, const polynomial &y) {
    polynomial result;

    size_t thread_count = std::thread::hardware_concurrency();
    std::vector<std::unordered_map<power, coeff>> thread_results(thread_count);

    size_t chunk_size = (x.terms.size() + thread_count - 1) / thread_count;

    auto multiply_chunk = [&](auto start, auto end, size_t thread_id) {
        auto &local_result = thread_results[thread_id];
        for (auto iter = start; iter != end; ++iter) {
            for (const auto &[p_y, c_y] : y.terms) {
                power new_power = iter->first + p_y;
                coeff new_coeff = iter->second * c_y;
                local_result[new_power] += new_coeff;
            }
        }
    };

    std::vector<std::thread> threads;
    auto iter = x.terms.begin();
    for (size_t i = 0; i < thread_count; ++i) {
        auto start = iter;
        std::advance(iter, std::min(chunk_size, static_cast<size_t>(std::distance(iter, x.terms.end()))));
        threads.emplace_back(multiply_chunk, start, iter, i);
    }

    for (auto &thread : threads) {
        thread.join();
    }

    for (const auto &local_result : thread_results) {
        for (const auto &[p, c] : local_result) {
            result.terms[p] += c;
        }
    }

    result.internal_canonical_form();
    return result;
}

polynomial operator*(const polynomial &x, int y) {
    polynomial result;
    for (const auto &[p, c] : x.terms) {
        result.terms[p] = c * y;
    }
    result.internal_canonical_form();
    return result;
}

polynomial operator*(int x, const polynomial &y) {
    return y * x;
}
    
polynomial operator%(const polynomial &x, const polynomial &y) {
    if(y.terms.empty() || y.terms.rbegin()->second == 0) {
        throw std::invalid_argument("Division by 0 polynomial.");
    }

    polynomial r = x; //remainder
    polynomial divisor = y;

    while (!r.terms.empty() && r.terms.rbegin()->first >= divisor.terms.rbegin()->first) {
        power diff = r.terms.rbegin()->first - divisor.terms.rbegin()->first;
        coeff ratio = r.terms.rbegin()->second / divisor.terms.rbegin()->second;

        polynomial term;
        term.terms[diff] = ratio;

        r = r + (term * divisor * -1);
        r.internal_canonical_form();
    }

    return r;
}

size_t polynomial::find_degree_of() {
    return terms.empty() ? 0 : terms.rbegin()->first;
}

std::vector<std::pair<power, coeff>> polynomial::canonical_form() const {
    if (terms.empty()) {
        return {{0, 0}};
    }
    std::vector<std::pair<power, coeff>> result(terms.begin(), terms.end());
    std::reverse(result.begin(), result.end());
    return result;
}