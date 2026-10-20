// =========================================================================
// Manager 类的头文件（总经理）。它负责管理所有的 Task。
// =========================================================================
#pragma once
#include <vector>   // 【容器】我们要用 vector（动态数组）当货架装任务
#include <string>
#include "Task.h"   // 必须包含 Task，不然货架里不知道装什么

class Manager 
{
private:
    // 【大货架】m_tasks 是一个动态数组，里面只能放 Task 类型的对象。
    // 你往里面塞多少个，它自己会动态扩容，永远不会越界。
    std::vector<Task> m_tasks; 
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
