cache.h

    class Cache {
    public:
	struct Handle { };  // Cache中一条记录到句柄，用户使用后需要自己Release

        virtual Insert()
        virtual LookUp()
        virtual Release()   // 释放句柄，引用计数减1
        virtual Value()
        ...
    private:
        void LRU_Remove(Handle* e);
        void LRU_Append(Handle* e);
        void Unref(Handle* e);
    };
    
    extern Cache* NewLRUCache(size_t capacity);

    
cache.cc

    namespace {
	struct LRUHandle {	// hash表一条记录的句柄
	  void* value;
	  void (*deleter)(const Slice&, void* value);
	  LRUHandle* next_hash;	// 指向相同hash值的下一条记录
	  LRUHandle* next;
	  LRUHandle* prev;
	  size_t charge;      // TODO(opt): Only allow uint32_t?
	  size_t key_length;
	  uint32_t refs;
	  uint32_t hash;      // Hash of key(); used for fast sharding and comparisons
	  char key_data[1];   // Beginning of key
	};

        class HandleTable {
        public:
            LRUHandle* Insert(LRUHandle* h)
            LRUHandle* Lookup(const Slice& key, uint32_t hash)
            LRUHandle* Remove(const Slice& key, uint32_t hash)
        };
        
        class LRUCache {
        public:
            Cache::Handle* Insert()
            Cache::Handle* LookUp()
            void Insert()
	private:
	    HandleTable table_;
	};


	class ShardedLRUCache : public Cache {  //继承 Cache的接口
	private:	    
	    LRUCache shard_[kNumShards];	//多个hash表，减小锁粒度
	public:
	    explicit ShardedLRUCache(size_t capacity);
	    virtual Handle* Insert();
	    virtual Handle* Lookup();
	    virtual void Release(Handle* handle);
	};
    }

    // 类工厂方法，返回指针，用户可替换为自己到Cache
    Cache* NewLRUCache(size_t capacity) {
        return new ShardedLRUCache(capacity);
    }

    
    
