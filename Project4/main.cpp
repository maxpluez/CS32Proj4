#include <iostream>
#include "Trie.h"
#include <vector>
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    Trie<int>* tp = new Trie<int>;
    tp->insert("hip", 3);
    tp->insert("hi", 5);
    tp->insert("cat", 7);
    tp->insert("hip", 6);
    vector<int> v = tp->find("catt", false);
    for(int i=0;i<v.size();i++)
        cout<<v[i]<<endl;
    delete tp;
    
    std::cout << "Hello, World!\n";
    return 0;
}
