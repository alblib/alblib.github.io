# Math Problems
I here post solutions for several math problems, some of which are made by my own, others are made by others.
- [Solutions](mathmagic) to [Math magic](https://erich-friedman.github.io/mathmagic/) by [Erich Friedman](https://erich-friedman.github.io/)

# Google competition template code

```
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

using Result = ;

Result case(){
    
}

template<class Type>
void prints(ostream& os, const Type& op){
    os << op;
}

template<class Type>
void prints(ostream& os, const vector<Type>& op){
    for (o: op){
        os << o << " ";
    }
}

int main(){
    vector<Result> results;
    size_t T;
    cin >> T;
    for(size_t i = 0; i < T; ++i){
        results.push_back(case());
    }
    for(size_t i = 0; i < T; ++i){
        prints(os, op);
        os << endl;
    }
}
```
