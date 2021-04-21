#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node{
   Node* next;
   Node* prev;
   int value;
   int key;
   Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
   Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{
   
   protected: 
   map<int,Node*> mp; //map the key to the node in the linked list
   int cp;  //capacity
   Node* tail; // double linked list tail pointer
   Node* head; // double linked list head pointer
   virtual void set(int, int) = 0; //set function
   virtual int get(int) = 0; //get function

};

class LRUCache : public Cache{
   public:
      int cur_size=0;
      LRUCache(int capacity)
      { 
         cp=capacity;
         head=nullptr;
         tail=nullptr;
      }

      // void removeTail()
      // {
      //    //Currently assumes that there are atleast two nodes
      //    Node* prev_node = tail->prev;
      //    prev_node->next = nullptr;
      //    tail->prev = nullptr;
      //    tail = prev_node;
      // }

      // void removeHead()
      // {
      //    //Currently assumes that there are atleast two nodes
      //    Node* next_node = head->next;
      //    next_node->prev = nullptr;
      //    head->next = nullptr;
      //    head = next_node;
      // }

      void removeNode(Node* node)
      {
         // if(node==tail)
         // {
         //    removeTail();
         //    return;
         // }
         // if(node==head)
         // {
         //    removeHead();
         //    return;
         // }

         Node* prev_node = node->prev;
         Node* next_node = node->next;
         if(prev_node!=nullptr)
            prev_node->next = next_node;
         if(next_node!=nullptr)
            next_node->prev = prev_node;
         node->prev = nullptr;
         node->next = nullptr;
      }

      void prependNode(Node* node)
      {
         // Use only after removing
         node->next = head;
         node->prev = nullptr;
         head->prev = node;
         head = node;
      }

      void moveToHead(Node* node)
      {
         if(node==head) return;
         removeNode(node);
         prependNode(node);
      }

      void addNewNodeToHead(int key, int value)
      {
         // (Node* p, Node* n, int k, int val)
         Node* node = new Node(nullptr, head, key, value);
         head->prev = node;
         head = node;
         mp[key] = node;
         cur_size++;
      }

      void deleteNode(Node* node)
      {
         removeNode(node);
         delete node;
         cur_size--;
      }

      void set(int key, int val) override
      {
         if(mp.find(key)!=mp.end())
         {
            Node* node = mp[key];
            node->value = val;
            moveToHead(node);
            return;
         }
         if(cur_size==0)
         {
            Node* node = new Node(key, val);
            head = node;
            tail = node;
            return;
         }
         if(cur_size==cp)
         {
            deleteNode(tail);
         }
         addNewNodeToHead(key, val);
      }

      int get(int key) override
      {
         if(mp.find(key)==mp.end()) return -1;  //if key doesn't exist return -1;
         Node* node = mp[key];      //obtain the node corresponding to the key
         int ret = node->value;     //obtain the value of that node
         moveToHead(node);    //move the node to head, since it is MRU
         return ret;          //return the value
      }
   
};

int main() {
   int n, capacity,i;
   cin >> n >> capacity;
   LRUCache l(capacity);
   for(i=0;i<n;i++) {
      string command;
      cin >> command;
      if(command == "get") {
         int key;
         cin >> key;
         cout << l.get(key) << endl;
      } 
      else if(command == "set") {
         int key, value;
         cin >> key >> value;
         l.set(key,value);
      }
   }
   return 0;
}
