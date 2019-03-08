#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
    int minLength;
    Trie<pair<int,int>> mt;
    vector<Genome> gv;
    bool comp(const GenomeMatch& g1, const GenomeMatch& g2);
};

bool GenomeMatcherImpl::comp(const GenomeMatch &g1, const GenomeMatch &g2){
    if(g1.percentMatch>g2.percentMatch)
        return true;
    else if(g1.percentMatch==g2.percentMatch&&g1.genomeName<g2.genomeName)
        return true;
    return false;
}

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
    minLength = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    if(genome.length()<minLength)
        return;
    gv.push_back(genome);
    string current;
    for(int i = 0; i <= genome.length() - minLength; i++){
        if(genome.extract(i, minLength, current))
            mt.insert(current, pair<int,int>(gv.size()-1,i));
    }
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return minLength;  // This compiles, but may not be correct
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    if(minimumLength<minimumSearchLength())
        return false;
    if(fragment.size()<minimumLength||fragment.size()<minimumSearchLength())
        return false;
    
    matches.clear();
    vector<pair<int,int>> v = mt.find(fragment.substr(0,minimumSearchLength()),exactMatchOnly);
    bool used = false;
    string currents;
    for(int i = 0; i < v.size(); i++){
        if(!gv[v[i].first].extract(v[i].second, fragment.size(), currents))
            continue;
        int j;
        for(j = 0; j < currents.size(); j++){
            if(fragment[j]==currents[j])
                continue;
            if(exactMatchOnly)
                break;
            if(!used){
                used = true;
                continue;
            }
            break;
        }
        if(j>=minimumLength){
            DNAMatch d;
            d.genomeName = gv[v[i].first].name();
            d.length = j;
            d.position = v[i].second;
            bool flag = false;
            for(int k = 0; k < matches.size(); k++){
                if(matches[k].genomeName==gv[v[i].first].name()){
                    if(matches[k].length>=j){
                        flag = true;
                        break;
                    } else {
                        flag = true;
                        matches[k] = d;
                        break;
                    }
                }
            }
            if(!flag)
                matches.push_back(d);
        }
    }
    if(matches.size()==0)
        return false;
    
    /*
    for(int i = 0; i < matches.size() - 1; i++){
        for(int j = i+1; j < matches.size(); j++){
            if(matches[i].genomeName==matches[j].genomeName){
                if(matches[i].length >= matches[j].length){
                    matches.erase(matches.begin()+j);
                    j--;
                } else {
                    matches.erase(matches.begin()+i);
                    i--;
                    break;
                }
            }
        }
    }
     */
    
    return true;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    if(query.length()<fragmentMatchLength)
        return false;
    int S = query.length()/fragmentMatchLength;
    string currentSubQuery;
    vector<DNAMatch> v;
    map<string, int> numOfMatches;
    for(int i = 0; i < S; i++){
        query.extract(i*fragmentMatchLength, fragmentMatchLength, currentSubQuery);
        if(findGenomesWithThisDNA(currentSubQuery, fragmentMatchLength, exactMatchOnly, v)){
            for(int i = 0; i < v.size(); i++){
                numOfMatches[v[i].genomeName]+=1;
            }
        }
    }
    results.clear();
    for(map<string,int>::iterator it = numOfMatches.begin(); it != numOfMatches.end(); it++){
        int p = (it->second) / S;
        if(p>matchPercentThreshold){
            GenomeMatch g;
            g.genomeName = it->first;
            g.percentMatch = p;
            results.push_back(g);
        }
    }
    //sort(results.begin(), results.end(), comp);
    
    return true;  // This compiles, but may not be correct
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
