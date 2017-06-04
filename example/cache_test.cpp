 #include <iostream>        
 #include <string>          
 #include <algorithm>  //for std::generate_n
 #include <stdlib.h>
 #include "lru_cache.h"
 
 using namespace std;
 
 std::string random_string( size_t length )
 {
     auto randchar = []() -> char    
     {
         const char charset[] =
         "0123456789"
         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"    
         "abcdefghijklmnopqrstuvwxyz";   
         const size_t max_index = (sizeof(charset) - 1);
         return charset[ rand() % max_index ];
     };
     std::string str(length,0);
     std::generate_n( str.begin(), length, randchar );
     return str;
 }

 void TestCache(Cache *cache) {
     srand(time(NULL)); 
     string random_key = random_string(13); 
     string random_value = random_string(29);
     printf("random_key: %s\n", random_key.data());
     printf("random_value: %s\n", random_value.data());
     Cache::Node *n = cache->Insert(random_key, random_value.data(), random_value.length(), nullptr);
     assert(n != nullptr);
     LRUNode *ln = reinterpret_cast<LRUNode*>(n);
     assert(ln->key() == random_key);
     assert(ln->refs == 2);
     cache->Release(n);
     assert(ln->refs == 1);

     n = cache->Lookup(random_key);
     ln = reinterpret_cast<LRUNode*>(n);
     printf("key: %s\n", ln->key().data());
     printf("value: %s\n", static_cast<char*>(ln->value));
     cache->Release(n);

     string new_value = random_string(29);
     n = cache->Insert(random_key, new_value.data(), new_value.length(), nullptr);
     cache->Release(n);
     
     n = cache->Lookup(random_key);
     ln = reinterpret_cast<LRUNode*>(n);
     printf("key: %s\n", ln->key().data());
     printf("new value: %s\n", static_cast<char*>(ln->value));
     cache->Release(n);

     cache->Earse(random_key);
     n = cache->Lookup(random_key);
     assert(n == nullptr);
 }
 
 int main(int argc, char **argv) {
     const size_t kCacheCapacity = 10 *1024 *1024;
     Cache *cache = NewLRUCache(kCacheCapacity);
     TestCache(cache);
     delete cache;
     return 0;
 }
