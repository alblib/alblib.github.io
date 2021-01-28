#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <boost/multiprecision/cpp_int.hpp>
#include <thread>
#include <ctime>
#include <atomic>
#include <functional>
#include <sstream>
#include <string>
#include <utility>
#include <stdexcept>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using namespace boost::multiprecision::literals;

// Largest n for P_n to be accomodated: n <= 43, Log2[P_n]<=256;
//current: n=22: 0.52s ->0.26s

#define LOG_EPSILON 1e-12

// MARK: - Progress Bar

std::string secondToString(double seconds){
    std::stringstream ss;
    if (seconds < 60) {
        ss << seconds;
    }else if (seconds < 3600){
        ss << int(seconds / 60) << "m" << std::setfill('0') << std::setw(2) << int(seconds) % 60 << "s";
    }else if (seconds < 86400){
        auto s = int(seconds);
        auto m = s / 60;
        s %= 60;
        auto h = m / 60;
        m %= 60;
        ss << h << "h" << std::setfill('0') << std::setw(2) <<  m << "m" << std::setfill('0') << std::setw(2) << s << "s";
    }else{
        auto m = int(seconds/60);
        auto h = m/60;
        m %= 60;
        auto d = h / 24;
        h %= 24;
        ss << d << "d" << h << "h" << std::setfill('0') << std::setw(2) <<  m << "m";
    }
    return ss.str();
}

class ProgressBar{
    std::atomic<int> completed;
    int total;
    const clock_t begin_time = clock();
    
    int lastBar = 0;
    int lastStrLen = 0;
    std::string lastStr;
public:
    ProgressBar(int total): completed(0), total(total) {}
    void addOne(){
        ++completed;
        auto untilNow = double( clock () - begin_time ) /  CLOCKS_PER_SEC;
        auto estimate = untilNow / (double(completed.load())/double(total));
        std::string timeStr = "["+secondToString(untilNow)+"/"+secondToString(estimate)+"]";
        int currentBar = double(completed.load())/double(total)*32;
        if (lastStrLen != 0){
            for (size_t i = 0; i < 32 - lastBar + 1 + lastStrLen; ++i)
                cout << "\b";
            for (size_t i = 0; i < 32 - lastBar + 1 + lastStrLen; ++i)
                cout << " ";
            for (size_t i = 0; i < 32 - lastBar + 1 + lastStrLen; ++i)
                cout << "\b";
        }
        for (size_t i = 0; i < currentBar - lastBar; ++i)
            cout << "|";
        for (size_t i = 0; i <= 32 - currentBar; ++i)
            cout << " ";
        if (completed.load() == total) timeStr = "";
        cout << timeStr;
        lastStr = timeStr;
        lastStrLen = lastStr.size();
        lastBar = currentBar;
        cout.flush();
    }
};

// MARK: - Basic functions:
template <class Integral>
Integral reversed(Integral original)
{
    Integral result = 0;
    while (original > 0){
        result = result * 10 +  original % 10;
        original /= 10;
    }
    return result;
}

template <class Integral>
bool is_palindromic(Integral arg)
{
    return reversed(arg) == arg;
}

template <class Integral>
std::vector<Integral> list_of_primes(Integral number){
    if (number < 1) return std::vector<Integral>();
    std::vector<Integral> result = {2};
    for (Integral i = 3; result.size() < number; i+=2){
        if (std::all_of(result.cbegin(), result.cend(), [i](Integral a){return i % a != 0;}))
            result.push_back(i);
    }
    return result;
}


template <class U, class T>
std::vector<U> vector_cast(const std::vector<T> x){
    std::vector<U> result(x.size());
    auto it1 = x.cbegin();
    auto it2 = result.begin();
    while (it1 != x.cend()){
        *it2 = static_cast<U>(*it1);
        ++it1; ++it2;
    }
    return result;
}

template <class T, class UnaryOperation>
auto apply_vector(UnaryOperation unary_op, const std::vector<T>& operand){
    std::vector<decltype(unary_op(std::declval<T>()))> result(operand.size());
    std::transform(operand.begin(), operand.end(), result.begin(), unary_op);
    return result;
}

auto log10int = [](const int& x){return std::log10(double(x));};

template <class T, class BinaryOperation, class InputIt, class OutputIt>
OutputIt fibonacci(InputIt begin, InputIt end, OutputIt output, BinaryOperation operation, T beginning){
    InputIt it = begin;
    while (it != end){
        *output = beginning = operation(beginning, *it);
        ++it;
        ++output;
    }
    return output;
}

template <class T, class BinaryOperation>
vector<T> fibonacci_vector(const vector<T>& inputs, BinaryOperation operation, T beginning){
    vector<T> result(inputs.size());
    fibonacci(inputs.begin(), inputs.end(), result.begin(), operation, beginning);
    return result;
}

// MARK: - Initializes tables

using represent_type = boost::multiprecision::checked_uint512_t;
using mask_type = uint64_t;
using ordinal_type = size_t;

auto primes = list_of_primes(64);
vector<represent_type> primorials = fibonacci_vector<represent_type>(vector_cast<represent_type>(primes), std::multiplies(), 1);
vector<double> log10_of_primes = apply_vector(log10int, primes);
std::array<double, (1<<16) > log10_of_factor16; // 16: 512KB------log10 of factors of primorial_22. 32MB memory.
std::array<double, (1<<16) > log10_of_factor16_2;
std::array<double, (1<<16) > log10_of_factor16_3;
std::array<double, (1<<16) > log10_of_factor16_4;

double log10_of_3digits[1001];
int multiplication_mod_3digits[1000][1000];

void initialize(){
    //log10_of_factor22
    log10_of_factor16[0] = 0;
    for (uint32_t i = 1; i < 1<<16; ++i){
        size_t maxdigit_1 = sizeof(uint32_t)*8 - __builtin_clz(i) - 1;
        log10_of_factor16[i] = log10_of_factor16[i ^ (1<<(maxdigit_1))] + log10_of_primes[maxdigit_1];
    }
    log10_of_factor16_2[0] = 0;
    for (uint32_t i = 1; i < 1<<16; ++i){
        size_t maxdigit_1 = sizeof(uint32_t)*8 - __builtin_clz(i) - 1;
        log10_of_factor16_2[i] = log10_of_factor16_2[i ^ (1<<(maxdigit_1))] + log10_of_primes[maxdigit_1+16];
    }
    log10_of_factor16_3[0] = 0;
    for (uint32_t i = 1; i < 1<<16; ++i){
        size_t maxdigit_1 = sizeof(uint32_t)*8 - __builtin_clz(i) - 1;
        log10_of_factor16_3[i] = log10_of_factor16_3[i ^ (1<<(maxdigit_1))] + log10_of_primes[maxdigit_1+32];
    }
    log10_of_factor16_4[0] = 0;
    for (uint32_t i = 1; i < 1<<16; ++i){
        size_t maxdigit_1 = sizeof(uint32_t)*8 - __builtin_clz(i) - 1;
        log10_of_factor16_4[i] = log10_of_factor16_4[i ^ (1<<(maxdigit_1))] + log10_of_primes[maxdigit_1+48];
    }
    
    // log10 signatures
    for (size_t i = 0; i <= 1000; ++i)
        log10_of_3digits[i] = std::log10(static_cast<double>(i));
    // multiplication table mod 1000
    for (int i = 0; i < 1000; ++i){
        for (int j = i; j < 1000; ++j){
            multiplication_mod_3digits[i][j] = multiplication_mod_3digits[j][i] = (i*j)%1000;
        }
    }
}


struct Factor{
    using IndexType = uint64_t;
    IndexType index;
    Factor(): index(0){}
private:
    Factor(uint64_t f): index(f){}
    
public:
    friend Factor makeFactorWithIndex(uint64_t i);
    Factor next() const{
        Factor tmp = *this;
        ++(tmp.index);
        return tmp;
    }
    Factor previous() const{
        return Factor(index-1);
    }
    Factor& transform_previous(){
        --index;
        return *this;
    }
    Factor prev_skipping_smaller() const{
        auto tmp = (index + 1);
        if (tmp == 0) return Factor(0);
        tmp ^= (1 << __builtin_ctzll(tmp));
        return Factor(tmp);
    }
    Factor& transform_prev_skipping_smaller(size_t with_depth_stopping){
        if (!(index & 1uLL)) return transform_previous();
        if (++index == 0) return *this;
        index ^= (1 << std::min(static_cast<size_t>(__builtin_ctzll(index)), with_depth_stopping));
        return *this;
    }
    
    double log10value() const{
        uint64_t _index = index;
        return log10_of_factor16[_index & 0xffffuLL] + log10_of_factor16_2[_index >> 16 & 0xffffuLL]
        + log10_of_factor16_3[_index >> 32 & 0xffffuLL] + log10_of_factor16_4[_index >> 48] ;
    }
     
    represent_type value() const{
        represent_type result = 1;
        uint64_t _index = index;
        for (size_t i = 0; i < primes.size() && _index; ++i){
            result *= (_index & 1) ? primes[i] : 1;
            _index >>= 1;
        }
        return result;
    }
    int value_mod_1000() const{
        int result = 1;
        uint64_t _index = index;
        for (size_t i = 0; i < primes.size() && _index; ++i){
            if (_index & 1)
                result = multiplication_mod_3digits[result][primes[i]];
            _index >>= 1;
        }
        return result;
    }
    bool pre_palindromic_test() const{
        auto _3d = value_mod_1000();
        if (_3d % 10 == 0) return false;
        auto _r = reversed(_3d);
        double lowest = fmod(log10_of_3digits[_r],1) - LOG_EPSILON;
        double highest = fmod(log10_of_3digits[_r+1],1) + LOG_EPSILON;
        auto sig = fmod(log10value(), 1);
        return (lowest < sig && sig < highest);
    }
};
Factor makeFactorWithIndex(uint64_t i){
    return Factor(i);
}

bool operator == (const Factor& a1, const Factor& a2){
    return a1.index == a2.index;
}

bool operator < (const Factor& a1, const Factor& a2){
    double delta = a1.log10value() - a2.log10value();
    if (delta < -LOG_EPSILON) return true;
    if (delta > LOG_EPSILON) return false;
    return a1.value() < a2.value();
}
bool operator > (const Factor& a1, const Factor& a2){
    return a2 < a1;
}

void thread_cell(std::atomic<Factor>& result, const int n, const int division_bit_depth, uint64_t threadIndex, ProgressBar& completedThread){
    const int spanDepth = n - 1 - division_bit_depth;
    const uint64_t startIndex = ((uint64_t(1) << division_bit_depth) | threadIndex) << spanDepth;
    const uint64_t endIndex = ((uint64_t(1) << division_bit_depth) + 1 + threadIndex) << spanDepth;
    Factor temp_max = result.load();
    double temp_max_log = temp_max.log10value() - LOG_EPSILON;
    
    if (n < 5) throw std::runtime_error("too small n.");
    if (spanDepth < 0) throw std::runtime_error("too many thread division.");
    Factor f = makeFactorWithIndex(endIndex-1);
    while (f.index >= startIndex){
        if (((f.index & 4) >> 2) & f.index){ // multiple of 10
            f.transform_previous();
        } else if (f.log10value() < temp_max_log){
            f.transform_prev_skipping_smaller(spanDepth);
        }else if (f.pre_palindromic_test() && is_palindromic(f.value())){
            while(f > temp_max && !result.compare_exchange_weak(temp_max, f)){
                temp_max_log = temp_max.log10value() - LOG_EPSILON;
            }
            f.transform_prev_skipping_smaller(spanDepth);
        }else{
            f.transform_previous();
        }
    }
    completedThread.addOne();
}

void operation(std::atomic<Factor>& result, const int n, const int division_bit_depth){
    std::atomic<uint64_t> completedThread;
    auto cnt = uint64_t(1) << division_bit_depth;
    ProgressBar progress(cnt);
    std::thread ths[cnt];
    for (size_t i = 0; i < cnt; ++i)
        ths[i] = std::thread(thread_cell, std::ref(result), n, division_bit_depth, i, std::ref(progress));
    
    for (size_t i = 0; i < cnt; ++i)
        ths[i].join();
}

void optimize_operation(std::atomic<Factor>& result, const int n){
    /*
    const int maxThreadDepth = std::min( n / 2,
        std::maxlround(log2(double(std::thread::hardware_concurrency())) * 2)
    );*/
    const int threadDepth = std::min(std::min(std::max((n-23)/3, 0), n/2), int(std::ceil(std::log2(double(std::thread::hardware_concurrency() * 2 - 1)))));
    operation(result, n, threadDepth);
}




int main(){
    initialize();
    
    cout << "====================================================" << std::endl;
    cout << " Obtain largest palindromic factor of nth primorial" << endl;
    
    int N;
    cout << "  n? > ";
    cin >> N;
    
    std::atomic<Factor> result = Factor();
    int n = 6;
    
    if (N >= 35){
        cout << " jump known answers? (y/n) > ";
        string ans;
        cin >> ans;
        
        if (ans == "y" || ans == "Y"){
            n = 35;
            result.store(makeFactorWithIndex(0b100110101010000001011001111110));
            cout << "previous n = " << n-1 << " : " << result.load().value() << std::endl;
        }
    }
    
    cout << "====================================================" << std::endl;
    cout << "0%\t||||||||||||||||||||||||||||||||\t100%" << std::endl;
    cout << "----------------------------------------------------" << std::endl;
    
    //Maximum<Factor> result;
    for (/*int n = 6*/; n <= N; ++n){
        cout << "n = " << n << ":\t";
        cout.flush();
        const clock_t begin_time = clock();
        
        optimize_operation(result, n);
        //cout << " ["<< float( clock () - begin_time ) /  CLOCKS_PER_SEC << "] ";
        cout << "  " << result.load().value() << " [" << secondToString(float( clock () - begin_time ) /  CLOCKS_PER_SEC ) << "]" << std::endl;
        
    }
    cout << "====================================================" << std::endl;
    return 0;
}
