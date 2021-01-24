#include <iostream>
#include <cmath>
#include <vector>
#include <thread>

struct Factor{
  unsigned long long index = 0;
  std::vector<int> factors() const{
    auto tmp = index;
    size_t i = 0;
    std::vector<int> result;
    while (tmp >>= 1){
      ++i;
      if (tmp & 1) result.push_back(prime[i]);
    }
    return result;
  }
  double logvalue() const{
    auto tmp = index;
    size_t i = 0;
    long double result = 0;
    while (tmp >>= 1){
      ++i;
      if (tmp & 1) result += log10prime[i];
    }
    return result;
  }
  int last_3digits() const{
    auto tmp = index;
    size_t i = 0;
    int result = 0;
    while (tmp >>= 1){
      ++i;
      if (tmp & 1){
        if (result == 0) result = prime[i];
        else{
          result = multi_digits[result][prime[i]];
        }
      }
    }
    return result;
  }
  bool pre3digit_test() const{
    
    double test = logvalue();
    if (test < 3.0) return true;
    
    int digits = last_3digits();
    int inv_dig = (digits%10) * 100;
    digits/=10;
    inv_dig += (digits%10) * 10 + digits/10;
    double lowest = log10digits[inv_dig] - 1e-6;
    double highest = inv_dig == 999 ? log10digits[100] + 1 + 1e-6 : log10digits[inv_dig+1] + 1e-6;
    
    return (test > lowest && test < highest);
  }
  Factor next() const{
    Factor tmp = *this;
    tmp.index += 2;
  }
}



int prime[64] = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137,
139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211,
223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283,
293, 307};
double log10prime[64];
double log10digits[1000];

int multi_digits[1000][1000];

long long result_index[64];

void generate_preparation(){
  for (size_t i = 0; i < 64; ++i)
    log10prime[i] = log10(double(prime[i]))
  log10twodigits[0] = 0;
  log10twodigits[1] = 0;
  for (size_t i = 2; i < 1000; ++i)
    log10twodigits[i] = log10(double(i))
  for (int i = 0; i < 1000; ++i){
    for(int j = 0; j < 1000; ++j){
      multi_twodigits[i][j] = (i*j)%1000;
    }
  }
}
