#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct trie {
    std::vector<trie*> dic = std::vector<trie*>(26, nullptr);  
    bool flag = false; 

};

void insert(trie* root, const std::string& s) {
    trie* node = root;
    for (char c : s) {
        int index = c - 'a';
        if (node->dic[index] == nullptr) {
            node->dic[index] = new trie();
        }
        node = node->dic[index];
    }
    node->flag = true;
}

bool search (trie* root, const std::string& s){
    trie* node = root;

    for(char c : s){
        int index = c - 'a';
        if(node->dic[index]==nullptr){
            return false;
        }
        node = node->dic[index];
    }

    return node->flag;
}

bool starts_with(trie* root, char c) {
    int index = c - 'a';
    if (index < 0 || index >= 26) return false;
    return root->dic[index] != nullptr;
}


int main(){
    trie* root = new trie();

    insert(root,"apple");
    insert(root,"apps");
    insert(root,"alien");
    //insert(root,"bats");

    if(search(root,"apple")) std::cout<<"apple exists\n";
    else std::cout<<"word does not exist\n";

    if(search(root,"appe")) std::cout<<"appe exists\n";
    else std::cout<<"word does not exist\n";

    if(starts_with(root,'a')) std::cout<<"words starts with A exist\n";
    else std::cout<<"words do not start with A\n";

    if(starts_with(root,'b')) std::cout<<"words starts with B exist\n";
    else std::cout<<"words do not start with B\n";


    return 0;
}
