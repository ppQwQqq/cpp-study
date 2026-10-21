// =========================================================================
// Manager 类的头文件（总经理）。它负责管理所有的 Task。
// =========================================================================
#pragma once
#include <vector>
#include <unordered_map>   // 【语法】引入无序哈希表头文件。底层基于哈希表实现，查找时间复杂度为 O(1)
#include <string>
#include "Task.h"   // 必须包含 Task，不然货架里不知道装什么

class Manager 
{
private:
    // 【语法】std::unordered_map<Key, Value>：
    // 这里的 Key 是 int（任务 ID），Value 是 Task 对象。
    // 容器中实际存储的是 std::pair<const int, Task> 类型的键值对。
    std::unordered_map<int, Task> m_tasks;
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
