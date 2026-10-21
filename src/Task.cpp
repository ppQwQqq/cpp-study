// ==========================================
// Task 类实现文件（源文件）
// ==========================================
#include "Task.h"

// 【语法】Task::Task：作用域解析运算符 ::，表示后面的 Task 函数属于 Task 类，而不是全局函数。
// 【语法】初始化列表 : m_id(id), m_desc(desc)：
// 在对象内存分配完成后、函数体执行前，直接调用成员变量的构造函数进行初始化，效率高于在 {} 内赋值。
Task::Task(int id, const std::string& desc, bool completed) 
    : m_id(id), m_desc(desc), m_completed(completed) 
{
}

int Task::getId() const 
{
    return m_id;
}

std::string Task::getDesc() const 
{
    return m_desc;
}

bool Task::isCompleted() const 
{
    return m_completed;
}

void Task::markCompleted() 
{
    m_completed = true;
}