#include <iostream>
#include "Trie.h"
#include "provided.h"
#include <vector>
#include <map>
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    Trie<int>* tp = new Trie<int>;
    tp->insert("hip", 10);
    tp->insert("hip", 20);
    tp->insert("hit", 1);
    tp->insert("hit", 2);
    tp->insert("hi", 9);
    tp->insert("hi", 17);
    tp->insert("hat", 7);
    tp->insert("hat", 8);
    tp->insert("hat", 9);
    tp->insert("to", 22);
    tp->insert("to", 23);
    tp->insert("tap", 19);
    tp->insert("tap", 6);
    tp->insert("tap", 32);
    vector<int> v = tp->find("catt", false);
    std::vector<int>    result1    =    tp->find("hit",    true);
    std::vector<int>    result2 =    tp->find("hit",    false);
    std::vector<int>    result3 =    tp->find("sit",    false);
    for(int i=0;i<result3.size();i++)
        cout<<result3[i]<<endl;
    delete tp;
    
    map<int,int> m;
    m[1]=10;
    
    
    GenomeMatcher g(3);
    Genome g1("Genome 1", "CGGTGTACNACGACTGGGGATAGAATATCTTGACGTCGTACCGGTTGTAGTCGTTCGACCGAAGGGTTCCGCGCCAGTAC");
    Genome g2("Genome 2", "TAACAGAGCGGTNATATTGTTACGAATCACGTGCGAGACTTAGAGCCAGAATATGAAGTAGTGATTCAGCAACCAAGCGG");
    Genome g3("Genome 3", "TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA");
    g.addGenome(g1);
    g.addGenome(g2);
    g.addGenome(g3);
    
    std::vector<DNAMatch> matches;
    bool result;
    result    =    g.findGenomesWithThisDNA("GAAG",    5,    true,    matches);
    
    std::cout << "Hello, World!\n";
    return 0;
}
