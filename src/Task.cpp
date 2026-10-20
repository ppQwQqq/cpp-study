// =========================================================================
// Task 类的实现文件。这里负责把 Task.h 里写的“大话”落实。
// =========================================================================
#include "Task.h" // 必须先包含对应的头文件，编译器才知道 Task 长什么样

// 【知识点 1：类外定义】Task:: 表示下面这个函数属于 Task 类。
// 【知识点 2：初始化列表】冒号后面的 : m_id(id), m_desc(desc), m_completed(completed)
// 作用：在对象出生的那一瞬间，直接把参数塞进成员变量里。效率极高，是大厂标配写法。
Task::Task(int id, const std::string& desc, bool completed) 
    : m_id(id), m_desc(desc), m_completed(completed) 
{
    // 大括号里不用写赋值了，因为初始化列表已经干完了。
}

// 实现 getId。注意：.h 里写了 const，这里也绝对不能漏掉！
int Task::getId() const 
{
    return m_id; // 返回内部的数据
}

std::string Task::getDesc() const 
{
    return m_desc;
}

bool Task::isCompleted() const 
{
    return m_completed;
}

// 实现修改状态。这里没有 const。
void Task::markCompleted() 
{
    m_completed = true; // 把状态强行改成 true
}