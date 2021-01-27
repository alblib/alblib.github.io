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

using boost::multiprecision::uint256_t;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using namespace boost::multiprecision::literals;

// Largest n for P_n to be accomodated: n <= 43, Log2[P_n]<=256;
//current: n=22: 0.52s


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

auto primes = list_of_primes(43);
vector<represent_type> primorials = fibonacci_vector<represent_type>(vector_cast<represent_type>(primes), std::multiplies(), 1);
vector<double> log10_of_primes = apply_vector(log10int, primes);
double log10_of_factor22[4194304]; // log10 of factors of primorial_22. 32MB memory.
double log10_of_nextfactor21[2097152]; // 16MB

double log10_of_3digits[1001];
int multiplication_mod_3digits[1000][1000];

void initialize(){
    //log10_of_factor22
    for (mask_type i = 0; i < 4194304; ++i){
        double result = 0;
        mask_type _index = i;
        for (size_t j = 0; j < 22 && _index; ++j){
            result += (_index & 1) ? log10_of_primes[j] : 0;
            _index >>= 1;
        }
        log10_of_factor22[i] = result;
    }
    for (mask_type i = 0; i < 2097152; ++i){
        double result = 0;
        mask_type _index = i;
        for (size_t j = 0; j < 21 && _index; ++j){
            result += (_index & 1) ? log10_of_primes[j+22] : 0;
            _index >>= 1;
        }
        log10_of_nextfactor21[i] = result;
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

static inline uint32_t log_2(const uint32_t x){if(x == 0) return 0;return (31 - __builtin_clz (x));}

struct Factor{
    using IndexType = uint64_t;
    IndexType index;
    Factor(): index(0){}
private:
    Factor(uint64_t f): index(f){}
    
    //static auto log10_of_primes = apply_vector(log10int, list_of_primes(64));
    static std::array<double, 1uLL << 24> log10_of_1st_factors24;
    //static std::array<double, 1uLL << 23> log10_of_2nd_factors23;
    //static std::array<double, 1uLL << 18> log10_of_3rd_factors18;
    
public:
    friend Factor makeFactorWithIndex(uint64_t i);
    Factor next() const{
        Factor tmp = *this;
        ++(tmp.index);
        return tmp;
    }
    /*
    double log10value() const{
        double result = 0;
        uint64_t _index = index;
        for (size_t i = 0; i < primes.size() && _index; ++i){
            result += (_index & 1) ? log10_of_primes[i] : 0;
            _index >>= 1;
        }
        return result;
    }*/
    
    double log10value() const{
        uint64_t _index = index;
        return log10_of_factor22[_index & 0x3fffffuLL] + log10_of_nextfactor21[_index >> 22];
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
        double lowest = fmod(log10_of_3digits[_r],1) - 1e-6;
        double highest = fmod(log10_of_3digits[_r+1],1) + 1e-6;
        auto sig = fmod(log10value(), 1);
        return (lowest < sig && sig < highest);
    }
};

 std::array<double, 1uLL << 24> Factor::log10_of_1st_factors24 = [](){
    std::array<double, 1uLL << 24> result;
    result[0] = 0;
    for (size_t i = 1; i < 1uLL<<24; ++i){
        size_t maxdigit_1 = sizeof(size_t)*8 - __builtin_clz(i) - 1;
        result[i] = result[i ^ (1<<(maxdigit_1))] + log10_of_primes[maxdigit_1];
    }
     cout<<"done";
    return result;
}();

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
    
    Maximum<Factor> result;
    int n = 6;
    
    if (N >= 35){
        cout << " jump known answers? (y/n) > ";
        string ans;
        cin >> ans;
        
        if (ans == "y" || ans == "Y"){
            n = 35;
            result.put(makeFactorWithIndex(0b100110101010000001011001111110));
            cout << "previous n = " << n-1 << " : " << result.get().value() << std::endl;
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
        cout << "  " << result.get().value() << " [" << secondToString(float( clock () - begin_time ) /  CLOCKS_PER_SEC ) << "]" << std::endl;
        
    }
    cout << "====================================================" << std::endl;
    return 0;
}
