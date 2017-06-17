# leveldb LRU cache 
从leveldb中剥离出来的LRU cache，逻辑与leveldb的cache一样，只是将Slice缓存std::string，hash函数换成[Murmur3](https://github.com/PeterScott/murmur3)

## leveldb中的使用场景
读取数据时，如果需要从level 0或更高层的文件读取数据，leveldb会将文件中包含key的block缓存到内存中。缓存的实现默认是LRU cache，也支持用户自定义实现。

用户只需继承Cache类，实现以下接口即可自定义Cache（对比leveldb略有改动，Slice改成std::string）
```
virtual Node* Insert(const std::string &key, const void *value, size_t charge,
                     void (*deleter)(const std::string &key, void *value)) = 0;
virtual Node* Lookup(const std::string &key) = 0;
virtual void Release(Node *n) = 0;
virtual void Earse(const std::string &key) = 0;
```

## 底层实现
底层使用hash表实现，使用链表解决hash冲突，产生相同hash值的key落在同一桶（链表）中，如下图，n1和n5，n4和n6，具有相同hash值。

![](https://github.com/ZhitongLei/lru_cache/blob/master/doc/hash-table.png)

### 自动扩容
当hash表元素个数大于桶数时，会自动扩容：
1. 计算新hash表所需的桶数，设置初始桶数为4，当桶数 < 元素个数时翻倍，直至桶数>元素个数
2. 按照计算的桶数，创建新hash表
3. 将旧hash中元素insert到新hash
4. 替换旧hash表

### LRU 实现
leveldb在每个节点增加了2个指针，prev和next指针，使得hash表中的元素都在同一个双向链表中，链表表头prev指向最新元素，next指向最老元素，当cache的空间使用完后，开始淘汰next指向元素：
![](https://github.com/ZhitongLei/lru_cache/blob/master/doc/double-link-list.png)

### 分shard减少读写冲突
为了减少读写冲突，LRUCache使用了16个hash表，也就是16shard，并发读写时，锁是shard级别的，避免了大范围的锁。
