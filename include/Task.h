// ==========================================
// Task 类声明文件（头文件）
// ==========================================
#pragma once // 【语法】预处理指令，保证该头文件在一个编译单元中只被展开一次，防止重复定义

#include <string>

class Task 
{
private: 
    // 【语法】m_ 前缀是命名规范，代表 Member（成员变量），用于区分函数形参
    int m_id;
    std::string m_desc;
    bool m_completed = false; // 【语法】C++11 类内初始化：允许在声明成员变量时直接给默认值

public:
    // 【语法】const std::string& desc：
    // 1. & 代表引用，避免实参到形参的深拷贝（提升性能）
    // 2. const 代表常量引用，承诺函数内部不修改传入的字符串，并且可以接收字面量（如 "买饭"）
    Task(int id, const std::string& desc, bool completed = false);
    
    // 【语法】函数末尾的 const：修饰隐含的 this 指针，承诺该函数不会修改类的任何成员变量。
    // 如果在一个 const 成员函数里修改 m_id，编译器会直接报错。
    int getId() const;
    std::string getDesc() const;
    bool isCompleted() const;
    
    // 【语法】没有 const 修饰，说明这是一个会改变对象内部状态的函数
    void markCompleted();
};