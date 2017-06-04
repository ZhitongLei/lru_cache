#include <benchmark/benchmark.h>
#include <string>
#include <algorithm>  //for std::generate_n
#include <sstream>
#include <unistd.h>
#include "lru_cache.h"

using namespace std;

string uint2str(unsigned int num) 
{
    ostringstream oss;
    oss << num;
    return oss.str();
}

static void BM_uint2str(benchmark::State& state) {
    unsigned int num = 1234;
    while (state.KeepRunning()) {
        (void) uint2str(num);
        //usleep(10);
    }
}

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

static void BM_random_string(benchmark::State& state) {
    size_t length = 30;
    srand(time(NULL)); 
    while (state.KeepRunning()) {
        string random_key = random_string(length);
    }
}

static void BM_cache_insert(benchmark::State& state) {
     const size_t kCacheCapacity = 100 *1024 *1024;
     Cache *cache = NewLRUCache(kCacheCapacity);
     srand(time(NULL)); 
     string random_key = random_string(13); 
     string random_value = random_string(29);
     while (state.KeepRunning()) {
         Cache::Node *n = cache->Insert(random_key, random_value.data(), random_value.length(), nullptr);
         cache->Release(n);
     }
    delete cache;
    cache = nullptr;
}

// Register the function as a benchmark
BENCHMARK(BM_uint2str);
BENCHMARK(BM_random_string);
BENCHMARK(BM_cache_insert);
BENCHMARK(BM_cache_insert)->Threads(10)->UseRealTime();
BENCHMARK_MAIN();
