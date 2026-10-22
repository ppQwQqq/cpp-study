// =========================================================================
// Manager 类的头文件（总经理）。它负责管理所有的 Task。
// =========================================================================
#pragma once
#include <vector>
#include <memory>
#include <unordered_map>   // 【语法】引入无序哈希表头文件。底层基于哈希表实现，查找时间复杂度为 O(1)
#include <string>
#include "Task.h"   // 必须包含 Task，不然货架里不知道装什么

class Manager 
{
private:
    // std::unordered_map<Key, Value>：
    // 这里的 Key 是 int，Value 是 Task 对象。
    // 容器中实际存储的是 std::pair<const int, Task> 类型的键值对。

    //【核心语法改造】std::unordered_map<int, std::unique_ptr<Task>>
    // 1. Key是int，不用改，因为 int 才 4 个字节，拷贝极快。
    // 2. Value变成了 std::unique_ptr<Task>（独占智能指针）,是一个智能指针的类（模板），它里面装着一个指向 Task 对象的指针。
    //auto ptr = std::make_unique<Task>(...);：这里的 ptr 是一个智能指针对象。
    // 3. 为什么不用 Task？因为 Task 对象如果很大（比如几 MB），放进 map 会发生深拷贝，
    //    而且如果程序异常退出，内存可能泄漏。
    // 4. 用了 unique_ptr 后，map 里只存一个 8 字节的指针（遥控器），并且当 map 销毁时，
    //    unique_ptr 会自动 delete 对应的 Task 对象（RAII 机制），绝不内存泄漏。

    std::unordered_map<int, std::unique_ptr<Task>> m_tasks;
    int m_next_id = 1;         // 下一个新任务该分配的编号
    std::string m_file_path;   // 数据保存的文件路径（如 "tasks.db"）

public:
    // 【构造函数】接收文件路径，顺便初始化成员变量
    Manager(const std::string& file_path);
    
    // 【析构函数】名字是 ~类名，没有参数，也没有返回值。
    // 对象被销毁（比如程序退出）时，会自动触发。我们用它来实现“自动存盘”（RAII）。
    ~Manager(); 

    void add(const std::string& desc); // 添加任务
    void show() const;                 // 打印任务，只读操作，加 const
    void done(int id);                 // 标记完成，会修改数据，不加 const 
    void save() const;                 // 存盘（只读内存写硬盘），加 const
    void load();                       // 读盘（把硬盘数据写入内存），会改数据，不加 const
    void remove(int id);
};
