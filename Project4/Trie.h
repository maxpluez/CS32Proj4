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
}

//Recursively destruct the Trie
template<typename ValueType>
void Trie<ValueType>::destroy(Node* p){
    if(p->children.size()==0){
        delete p;
        return;
    }
    Node* current;
    for(auto it = p->children.begin();it != p->children.end();it++){
        current = *it;
        destroy(current);
    }
    delete p;
}

template<typename ValueType>
void Trie<ValueType>::reset(){
    destroy(root);
    root = new Node;
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string &key, const ValueType &value){
    insertHelper(key, value, root);
}

//Recursively insert an item
template<typename ValueType>
void Trie<ValueType>::insertHelper(std::string key, ValueType value, Node *p){
    char ckey = key[0]; //key's length -- for every level. Take out the first value for process
    //base case: key's size == 1 so we are about to insert the data
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
        //The only place recursion occur: there exist the first character of the key in the Trie. (if not, we will create a whole branch starting from this character)
        if(ckey==p->children[i]->mkey){
            std::string custring = key.substr(1,key.size()-1);
            insertHelper(custring, value, p->children[i]);
            return;
        }
    }
    //Another base case: creating a whole branch if key.size()!=1 and the first character of key doesn't already exist
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
    //because first letter can never be different, so we traverse through root's children to prevent the first letter being different
    for(int i = 0; i < root->children.size(); i++)
        if(root->children[i]->mkey==key[0])
            findHelper(key.substr(1,key.size()-1), exactMatchOnly, result, root->children[i]);
    return result;
}

template<typename ValueType>
void Trie<ValueType>::findHelper(std::string key, bool exactMatchOnly, std::vector<ValueType> &result, Node* p, bool used) const{
    char ckey = key[0];
    
    //Base case 1: only one character in the key so if found, must be in its children
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
    
    //base case 2: when we reach the leaf and not found
    if(p->children.size()==0)
        return;
    
    //recursion
    std::string custring = key.substr(1,key.size()-1);
    for(int i = 0; i < p->children.size(); i++){
        if(p->children[i]->mkey==ckey)
            findHelper(custring, exactMatchOnly, result, p->children[i], used);
        else if(!exactMatchOnly&&!used)
            findHelper(custring, exactMatchOnly, result, p->children[i], true);
    }
}

#endif // TRIE_INCLUDED
