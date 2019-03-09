#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;
    
    // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    struct Node{
        char mkey;
        std::vector<ValueType> data;
        std::vector<Node*> children;
    };
    Node* root;
    void destroy(Node* p);
    void insertHelper(std::string key, ValueType value, Node* p);
    void findHelper(std::string key, bool exactMatchOnly, std::vector<ValueType>& result, Node* p, bool used = false) const;
};

template<typename ValueType>
Trie<ValueType>::Trie(){
    root = new Node;
}

template<typename ValueType>
Trie<ValueType>::~Trie(){
    destroy(root);
    delete root;
}

template<typename ValueType>
void Trie<ValueType>::destroy(Node* p){
    if(p->children.size()==0)
        return;
    Node* current;
    /*
    for(int i = 0; i < p->children.size(); i++){
        if(p->children[i]->children.size()==0){
            delete p->children[i];
            p->children.erase(p->children.begin()+i);
        }
        else
            destroy(p->children[i]);
    }
     */
    auto it = p->children.begin();
    while(it != p->children.end()){
        current = *it;
        if(current->children.size()==0){
            delete current;
            it = p->children.erase(it);
        } else {
            destroy(current);
            it++;
        }
    }
}

template<typename ValueType>
void Trie<ValueType>::reset(){
    destroy(root);
    delete root;
    root = new Node;
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string &key, const ValueType &value){
    insertHelper(key, value, root);
}

template<typename ValueType>
void Trie<ValueType>::insertHelper(std::string key, ValueType value, Node *p){
    char ckey = key[0];
    if(key.size()==1){
        for(int i = 0; i < p->children.size(); i++){
            if(p->children[i]->mkey==ckey){
                p->children[i]->data.push_back(value);
                return;
            }
        }
        Node* newNode = new Node;
        newNode->data.push_back(value);
        newNode->mkey = key[0];
        p->children.push_back(newNode);
        return;
    }
    for(int i = 0; i < p->children.size(); i++){
        if(ckey==p->children[i]->mkey){
            std::string custring = key.substr(1,key.size()-1);
            insertHelper(custring, value, p->children[i]);
            return;
        }
    }
    Node* newNode;
    Node* current = p;
    for(int i = 0; i < key.size(); i++){
        newNode = new Node;
        newNode->mkey = key[i];
        current->children.push_back(newNode);
        current = newNode;
    }
    current->data.push_back(value);
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string &key, bool exactMatchOnly) const{
    std::vector<ValueType> result;
    for(int i = 0; i < root->children.size(); i++)
        if(root->children[i]->mkey==key[0])
            findHelper(key.substr(1,key.size()-1), exactMatchOnly, result, root->children[i]);
    return result;
}

template<typename ValueType>
void Trie<ValueType>::findHelper(std::string key, bool exactMatchOnly, std::vector<ValueType> &result, Node* p, bool used) const{
    char ckey = key[0];
    if(key.size()==1){
        if(exactMatchOnly||used){
            for(int i = 0; i < p->children.size(); i++){
                if(p->children[i]->mkey==ckey){
                    for(int j = 0; j < p->children[i]->data.size(); j++)
                        result.push_back(p->children[i]->data[j]);
                }
            }
        } else {
            for(int i = 0; i < p->children.size(); i++){
                for(int j = 0; j < p->children[i]->data.size(); j++)
                    result.push_back(p->children[i]->data[j]);
            }
        }
        return;
    }
    if(p->children.size()==0)
        return;
    
    std::string custring = key.substr(1,key.size()-1);
    for(int i = 0; i < p->children.size(); i++){
        if(p->children[i]->mkey==ckey)
            findHelper(custring, exactMatchOnly, result, p->children[i], used);
        else if(!exactMatchOnly&&!used)
            findHelper(custring, exactMatchOnly, result, p->children[i], true);
    }
}

#endif // TRIE_INCLUDED
