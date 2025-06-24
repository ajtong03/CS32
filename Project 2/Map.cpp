#include "Map.h"

Map::Map()
{
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
}

Map::~Map()
{
    Node* temp = m_head; //temp points to m_head; temp is a local variable so it is destroyed when function finishes executing
    Node* p; //p will be destroyed when function finishes executing
    while(temp != nullptr)
    {
        p = temp->next; //p points to element following temp
        delete temp; //temp is deleted
        temp = p; //temp is p
    }
}

Map::Map(const Map& m)
{
    m_size = m.size();
    if(m_size == 0) //check if m is empty
    {
        m_head = nullptr;
        m_tail = nullptr;
    }
    else
    {
        //create first Node of list
        Node* current = new Node;
        current->next = nullptr;
        current->previous = nullptr;
        current->m_key = m.m_head->m_key;
        current->m_value = m.m_head->m_value;
        m_head = current; //make m_head point to first Node
        
        for(Node* p = m.m_head->next; p != nullptr; p = p->next) //iterate through m and create new Node in this for each element
        {
            Node* temp = new Node; //create new Node
            temp->m_key = p->m_key; //assign key value from m to temp
            temp->m_value = p->m_value;//assign value from m to temp
            temp->previous = current; //make temp's previous pointer point to the node inserted before it
            temp->next = nullptr; //make temp's next pointer point to nullptr
            current->next = temp; //current's next pointer points to temp
            current = temp; //current now points to element temp points to so the Map can continue to be built
        }
        m_tail = current;//m_tail points to last element of Map
    }
    
}

Map& Map::operator=(const Map& rhs)
{
    if(this != &rhs)
    {
        Map temp(rhs); //create copy of rhs
        swap(temp);//swap this with rhs
    }
    return *this;
}

bool Map::empty() const
{
    if(m_head == nullptr && m_tail == nullptr) //no Nodes in Map
        return true;
    return false;
}

int Map::size() const
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) //MEMORY LEAK??????
{
    if(contains(key))//return false if key already exists in the Map
        return false;
    Node* temp = new Node; //create new Node that holds key and value
    temp->m_key = key;
    temp->m_value = value;
    if(m_head == nullptr && m_tail == nullptr) //if Map is empty
    {
        m_head = temp; //set head and tail to point to temp
        m_tail = temp;
        temp->next = nullptr; //next and previous point to nullptr
        temp->previous = nullptr;
    }
    
    else if(m_head->m_key > key) //if need to insert at beginning of Map
    {
        temp->next = m_head; //temp's next points to m_head
        temp->previous = nullptr; //previous points to nullptr
        m_head->previous = temp; //adjust m_head's previous pointer
        m_head = temp; //set m_head to point to temp
    }
    else if(m_tail->m_key < key) //if need to insert at end of map
    {
        temp->next = nullptr; //set temp's next to nullpointer
        temp->previous = m_tail; //temp's previous points to m_tail
        m_tail->next = temp; //adjust m_tail's next
        m_tail = temp; //set m_tail to now point to temp
    }

    else //inserting between two Nodes
    {
        Node* p = m_head; //create temporary pointer to iterate through Map
        for(p = m_head; p->next != nullptr && p->next->m_key < key; p = p->next)//iterate until find spot of insertion; insert in ascending order
            ;
        
        //insert between two elements
        temp->next = p->next; //adjust temp's next and previous pointers to point to next and previous of p
        temp->previous = p;
        p->next = temp; //adjust p's next pointer
        temp->next->previous = temp; //make following Node's previous point to temp
    }
    m_size++; //increment size
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    Node* p; //temporary pointer
    for(p = m_head; p != nullptr && p->m_key != key; p = p->next)//iterate until nullptr reached or Node with key found
        ;
    if(p == nullptr)//key doesn't exist in Map
        return false;
    else
        p->m_value = value;//p points to Node with key; update value of that Node
    return true;
    
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if(update(key, value))
        return true; //if update returns true, key exists in Map. return true (m_value has been updated)
    return insert(key, value); //insert key, value pair into map, return whether or not it is successful
}

bool Map::erase(const KeyType& key)
{
    if(m_size == 0)
        return false;
    else if(m_size == 1) //if only one Node in Map
    {
        delete m_head; //delete that Node
        m_head = nullptr; //m_head and m_tail now point to nullptr
        m_tail = nullptr;
    }
    else if(key == m_head->m_key) //check to see if the first node's key value is equal to key
    {
        Node* temp = m_head; //create a temporary pointer to head
        m_head = temp->next; //make head point to the Node following head
        m_head->previous = nullptr; //assign new head's previous pointer to nullptr
        delete temp; //delete the Node head originally pointed to
    }
    else if (key == m_tail->m_key) //check if last node's key value is equal to key
    {
        Node* temp = m_tail; //create temporary pointer to tail
        m_tail = temp->previous; //make tail point to Node in front of tail
        m_tail->next = nullptr; //make tail's next pointer point to nullptr
        delete temp; //delete the Node tail originally pointed to
    }
    else
    {
        Node* temp; //create temporary pointer
        for(temp = m_head; temp != nullptr && temp->m_key != key; temp = temp->next) //iterate until nullptr reached or Node with key found
            ;
        if(temp == nullptr) //key doesn't exist
            return false;
        else
        {
            temp->previous->next = temp->next; //set (Node before p)'s next pointer to Node following p
            temp->next->previous = temp->previous; //set (Node after p)'s previous pointer to Node before p
            delete temp; //delete Node that p was pointing to
        }
    }
    m_size--; //decrement size
    return true;
}

bool Map::contains(const KeyType& key) const
{
    Node* p; //create temporary pointer
    for(p = m_head; p != nullptr && p->m_key != key; p = p->next) //iterate until nullptr reached or Node with key value equal to key is reached
        ;
    if(p == nullptr)//return false if Map doesn't contain key
        return false;
    return true; //return true if Map contains key
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* p; //create temporary pointer
    for(p = m_head; p != nullptr && p->m_key != key; p = p->next) //iterate until nullptr reached or Node with key value equal to key is reached
        ;
    if(p == nullptr) //return false if key doesn't exist
        return false;
    value = p->m_value;//if key exists, assign value the value stored at the Node p is pointing to
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    //check if i is a valid value
    if(i < 0 || i >= m_size)
        return false;
    
    Node* p = m_head; //create a temporary pointer to point to head
    for(int j = 0; j < i; j++)
    {
        p = p->next; //iterate until the ith element is reached
    }
    key = p->m_key; //assign key the key value stored at the Node p is pointing to
    value = p->m_value; //assign value the value stored at the Node p is pointing to
    return true;
}

void Map::swap(Map& other)
{
    //swap sizes of Maps
    int size = m_size;
    m_size = other.m_size;
    other.m_size = size;
    
    Node* tempH = m_head; //create temporary pointer to this' head
    //make head point to head of other and other's head point to this' old head
    m_head = other.m_head;
    other.m_head = tempH;
    
    Node* tempT = m_tail; //create temporary pointer to this' tail
    //make tail point to tail of other and other's tail point to this' old tail
    m_tail = other.m_tail;
    other.m_tail = tempT;
    
}

bool merge(const Map& m1, const Map& m2, Map& result)
{
    Map temp; //create temporary Map to hold values that result should hold at end
    int size; //holds size of smaller Map
    bool sameVals = true; //keeps track of if all same keys in m1 and m2 map to the same value
    bool m1Small = false; //variable to help determing which Map is smaller when iterating through the Maps
    
    if(m1.size() < m2.size()) //compare sizes and assign smaller size to size
    {
        size = m1.size();
        m1Small = true;
    }
    else
        size = m2.size();
        
    for(int i = 0; i < size; i++) //iterate through smaller Map
    {
        KeyType key;
        ValueType value;
        ValueType value2;
        
        m1.get(i, key, value); //get the ith key of m1
        
        if(m2.get(key, value2)) //checks if that key exists in m2, and if it does, stores value of that element in value2
        {
            if(value == value2) //if values are equal, insert that key-value pair into map
                temp.insert(key, value);
            else
                sameVals = false; //if values aren't equal, sameVals is false
        }
        else //key doesn't exist in both maps
        {
            temp.insert(key, value); //insert ith element of m1 into result
            KeyType key2;
            m2.get(i, key2, value2); //retrieve key and value of ith element of m2
            temp.insert(key2, value2); //insert ith element of m2 into result
        }
        
    }
    KeyType key;
    ValueType value;
    if(m1Small) //if m1 is smaller, iterate through rest of m2 and add elements to result
    {
        for(int i = size; i < m2.size(); i++)
        {
            m2.get(i, key, value);
            if(!m1.contains(key)) //ensure that if m1 and m2 contain same key that maps to different value, it's not added to resulting Map
                temp.insert(key, value);
        }
    }
    else //if m1 is larger, iterate through rest of m1 and add elements to result
    {
        for(int i = size; i < m1.size(); i++)
        {
            m1.get(i, key, value);
            if(!m2.contains(key)) //don't add elements that are contained in m2
                temp.insert(key, value);
        }
    }
    result.swap(temp);//result now contains values in temp; temp is destroyed at end of function since it's a local variable
    return sameVals; //return whether all same keys mapped to same values
}

void reassign(const Map& m, Map& result)
{
    Map temp = m; //create copy of m and store in temporary Map
    
    for(int i = 0; i < temp.size() - 1; i++) //iterate until temp.size() - 1; -1 so we can check element at i+1 without bounds issue
    {
        KeyType k;
        ValueType v;
        temp.get(i, k, v);//retrieve key and value stored at ith Node
        
        KeyType k2;
        ValueType v2;
        temp.get(i + 1, k2, v2); //retrieve key and value stored at i+1th Node
        
        temp.update(k, v2); //insert value from i+1th Node into ith Node
        temp.update(k2, v); //insert value from ith Node into i+1th Node
    }
    result.swap(temp);//swap result with temp; result now contains reassigned values
}

