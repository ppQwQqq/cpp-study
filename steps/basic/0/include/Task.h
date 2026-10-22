#pragma once
#include <string>

// 【新增】强类型枚举：定义优先级
enum class Priority 
{
    HIGH = 1,   
    MEDIUM = 2, 
    LOW = 3     
};

class Task 
{
private: 
    int m_id;
    std::string m_desc;
    bool m_completed;
    Priority m_priority; // 【新增】优先级属性

public:
    // 【修改】增加 priority 参数
    Task(int id, const std::string& desc, Priority priority = Priority::MEDIUM, bool completed = false);
    
    int getId() const;
    std::string getDesc() const;
    bool isCompleted() const;
    Priority getPriority() const; // 【新增】

    void markCompleted();
};