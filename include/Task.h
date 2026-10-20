// =========================================================================
// Task 类的头文件（菜单）。只放“声明”（我有什么），不写“实现”（具体怎么做）
// =========================================================================
#pragma once // 【防止重复包含】告诉编译器，这个文件在同一个项目里只读一次

#include <string> // 引入标准字符串库，因为下面用到了 std::string

class Task 
{
private: 
    // 【封装】private 表示这些数据只能类内部自己用，外部不能直接改
    // 【命名】m_ 前缀代表 Member（成员变量），一眼就能和外面的传参区分开
    int m_id;              // 任务的唯一身份证号
    std::string m_desc;    // 任务的具体描述内容
    bool m_completed;      // 任务是否完成（true完成，false未完成）

public:
    // 【构造函数】
    // 1. const std::string& desc：传大对象必须用“常量引用”，避免深拷贝(提速)，且承诺绝对不改它。
    // 2. completed = false：默认参数。如果在外面只传了 id 和 desc，它自动默认为未完成。
    Task(int id, const std::string& desc, bool completed = false);
    
    // 【只读查询函数】末尾的 const 极其重要！
    // 它是对编译器的承诺：我这个函数只负责把数据交出去，绝对不会修改类里的任何数据。
    int getId() const;
    std::string getDesc() const;
    bool isCompleted() const; 
    
    // 【修改函数】这里没有 const，说明它要“修改”内部数据（把状态变成完成）
    void markCompleted();
};