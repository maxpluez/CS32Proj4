#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    string mname;
    string msequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
    mname=nm;
    msequence=sequence;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes)
{
    genomes.clear();
    if(!genomeSource)
        return false;  // This compiles, but may not be correct
    char c;
    if(!genomeSource.get(c))
        return false;
    if(c!='>')
        return false;
    string nm;
    getline(genomeSource, nm);
    string se;
    string current;
    while(getline(genomeSource, current)){
        c = current[0];
        if(c=='>'){
            Genome g(nm, se);
            genomes.push_back(g);
            nm = current.substr(1, current.size()-1);
            se = "";
        } else {
            for(int i = 0; i < current.size(); i++)
                toupper(current[i]);
            se+=current;
        }
    }
    Genome g(nm, se);
    genomes.push_back(g);
    return true;
}

int GenomeImpl::length() const
{
    return msequence.size();
}

string GenomeImpl::name() const
{
    return mname;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
    if(position<0||length<0||position+length>msequence.size())
        return false;
    fragment = "";
    for(int i = position; i < length; i++)
        fragment+=msequence[i];
    return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}

