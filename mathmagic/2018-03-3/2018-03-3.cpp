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

using boost::multiprecision::uint256_t;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

// Largest n for P_n to be accomodated: n <= 43, Log2[P_n]<=256;


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

// MARK: Basic functions:
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

// MARK: - Initializes tables

auto primes = list_of_primes(43);
vector<uint256_t> primorials;
vector<double> log10_of_primes;

double log10_of_3digits[1001];
int multiplication_mod_3digits[1000][1000];

void initialize(){
    // primorials
    uint256_t prim = 1;
    for (const auto& a: primes)
        primorials.push_back(prim *= a);
    // log10 of primes
    for (const auto& p: primes)
        log10_of_primes.push_back(std::log10(static_cast<double>(p)));
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
    double log10value() const{
        double result = 0;
        uint64_t _index = index;
        for (size_t i = 0; i < primes.size() && _index; ++i){
            result += (_index & 1) ? log10_of_primes[i] : 0;
            _index >>= 1;
        }
        return result;
    }
    uint256_t value() const{
        uint256_t result = 1;
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
        double lowest = fmod(log10_of_3digits[_r],1) - 1e-6;
        double highest = fmod(log10_of_3digits[_r+1],1) + 1e-6;
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
    if (delta < -1e-6) return true;
    if (delta > 1e-6) return false;
    return a1.value() < a2.value();
}
bool operator > (const Factor& a1, const Factor& a2){
    return a2 < a1;
}

template <typename T, class Compare = std::less<T>>
class Maximum{
public:
    using element_type = T;
private:
    std::atomic<T> value;
    Compare _com;
public:
    Maximum(): value(T()), _com(Compare()){}
    Maximum(T _default): value(_default), _com(Compare()){}
    Maximum(T _default, Compare comparison): value(_default), _com(comparison){}
    T get () const{
        return value.load();
    }
    void put(const T& compare){
        T tmp = get();
        while (_com(tmp, compare) && !value.compare_exchange_weak(tmp, compare)){}
    }
};

void thread_cell(Maximum<Factor>& result, const int n, const int division_bit_depth, uint64_t threadIndex, ProgressBar& completedThread){
    
    const uint64_t startIndex = ((uint64_t(1) << division_bit_depth) | threadIndex) << (n - 1 - division_bit_depth);
    const uint64_t endIndex = ((uint64_t(1) << division_bit_depth) + 1 + threadIndex) << (n - 1 - division_bit_depth);
    for ( uint64_t i = startIndex; i < endIndex; ++i){
        auto temp = makeFactorWithIndex(i);
        if (temp.log10value() < result.get().log10value() - 1e-6) continue;
        if (!temp.pre_palindromic_test()) continue;
        auto val = temp.value();
        if (is_palindromic(val)){
            result.put(temp);
        }
    }
    completedThread.addOne();
}

void operation(Maximum<Factor>& result, const int n, const int division_bit_depth){
    std::atomic<uint64_t> completedThread;
    auto cnt = uint64_t(1) << division_bit_depth;
    ProgressBar progress(cnt);
    std::thread ths[cnt];
    for (size_t i = 0; i < cnt; ++i)
        ths[i] = std::thread(thread_cell, std::ref(result), n, division_bit_depth, i, std::ref(progress));
    
    for (size_t i = 0; i < cnt; ++i)
        ths[i].join();
}

void optimize_operation(Maximum<Factor>& result, const int n){
    /*
    const int maxThreadDepth = std::min( n / 2,
        std::maxlround(log2(double(std::thread::hardware_concurrency())) * 2)
    );*/
    const int threadDepth = std::min(std::max((n-20)/3, 0), n/2);
    operation(result, n, threadDepth);
}




int main(){
    initialize();
    
    
    cout << "====================================================" << std::endl;
    cout << " Obtain largest palindromic factor of nth primorial" << endl;
    
    int N;
    cout << "  n? > ";
    cin >> N;
    cout << "====================================================" << std::endl;
    cout << "0%\t||||||||||||||||||||||||||||||||\t100%" << std::endl;
    cout << "----------------------------------------------------" << std::endl;
    
    Maximum<Factor> result;
    for (int n = 6; n <= N; ++n){
        cout << "n = " << n << ":\t";
        cout.flush();
        const clock_t begin_time = clock();
        
        optimize_operation(result, n);
        //cout << " ["<< float( clock () - begin_time ) /  CLOCKS_PER_SEC << "] ";
        cout << "  " << result.get().value() << " [" << secondToString(float( clock () - begin_time ) /  CLOCKS_PER_SEC ) << "]" << std::endl;
        
    }
    cout << "====================================================" << std::endl;
    return 0;
}
