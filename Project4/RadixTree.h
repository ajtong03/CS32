#ifndef RadixTree_h
#define RadixTree_h
#include <iostream>
#include <algorithm>

template <typename ValueType>

class RadixTree
{
public:
    RadixTree()
    {
        
    }

    ~RadixTree()
    {
        
    }
    
    void insert(std::string key, const ValueType& value)
    {
        insertHelper(m_root, key, value);
    }
    
    ValueType* search(std::string key) const
    {
        return searchHelp(m_root, key);
    }
    
    
private:
    struct Node
    {
        bool end;
        ValueType* m_value;
        Node* m_children[128];
        std::string m_segment[128];
        //std::string m_segment;
        
        Node()
        {
            for(int i = 0; i < 128; ++i)
            {
                m_children[i] = nullptr;
            }
        }
        ~Node()
        {
            for(int i = 0; i < 128; ++i)
                delete m_children[i];
        }
    };
    
    Node* m_root = new Node;
    
    void insertHelper(Node* root, std::string str, const ValueType& value)
    {
        if(str == "") //end of str has been reached
        {
            root->end = true;
            root->m_value = new ValueType(value);// = value;
            return;
        }
        
        int currentLetter = str[0]; //retrieve ascii code for the first letter in the string
        if(root->m_children[currentLetter] == nullptr) //there is no path to this letter in the string
        {
            root->m_children[currentLetter] = new Node;
            root->m_segment[currentLetter] = str;
            insertHelper(root->m_children[currentLetter], "", value);
            return;
        }
        
        //determine whether str matches a current string in the tree fully or partially
        
        std::string currentString = root->m_segment[currentLetter];
        int matchIndex = 0;

        //determing up to what index currentString and str match
        while(matchIndex < std::min(str.length(), currentString.length()) && currentString[matchIndex] == str[matchIndex])
            ++matchIndex;
        
        //str is contained within currentString so currentString needs to be broken down
        
        //part of str is equal to currentString
        if(matchIndex == currentString.length())
        {
            insertHelper(root->m_children[currentLetter], str.substr(matchIndex), value);
        }
        //str is equal to part of currentString
        else if(matchIndex == str.length())
        {
            Node* temp = new Node;
            temp->m_children[currentString[matchIndex]] = root->m_children[currentLetter];
            temp->m_segment[currentString[matchIndex]] = currentString.substr(matchIndex);
            root->m_children[currentLetter] = temp;
            root->m_segment[currentLetter] = str;
            insertHelper(root->m_children[currentLetter], "", value);
        }
        //part of str is contained in part of currentString
        else
        {
            Node* newNode = new Node;
            root->m_segment[currentLetter] = str.substr(0, matchIndex); //segment holds the string segment that is the same in both str and currentString
            newNode->m_children[currentString[matchIndex]] = root->m_children[currentLetter];
            newNode->m_segment[currentString[matchIndex]] = currentString.substr(matchIndex);
            root->m_children[currentLetter] = newNode;
            insertHelper(root->m_children[currentLetter], str.substr(matchIndex), value);
        }
    }

    ValueType* searchHelp(Node* root, std::string key) const
    {
        //if key is an empty string and root is the end of a string, return root's value. if key is empty string and not the end of a string, return nullptr
        if(key.empty())
        {
            if(root->end)
            {
                return root->m_value;
            }
            else
                return nullptr;
        }
        int currentLetter = key[0]; //variable to hold first character of key
        if(root->m_children[currentLetter] == nullptr) //if there's no string stemming from root beginning with currentLetter so return nullptr
            return nullptr;
        
        std::string currentString = root->m_segment[currentLetter]; //variable to hold the current string
        
        size_t length = std::min(currentString.length(), key.length()); //variable to hold the string with smaller size between currentString and key
       
        if(currentString.substr(0, length) != key.substr(0, length)) //no match between the substrings of currentString and key
            return nullptr;
        if(currentString.length() == length) //search for the latter part of key that is not part of currentString
            return searchHelp(root->m_children[currentLetter], key.substr(currentString.length()));
        else
            return nullptr; //all paths have not returned a value so nullptr is returned.
    }
    
};



#endif /* RadixTree_h */

