#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Task.h"

class Manager 
{
private: 
    std::unordered_map<int, std::unique_ptr<Task>> m_tasks;
    int m_next_id = 1;         
    std::string m_file_path;
       
public:
    Manager(const std::string& file_path);
    ~Manager(); 

    // 【修改】增加 priority 参数
    void add(const std::string& desc, Priority priority = Priority::MEDIUM); 
    
    void show() const;                 
    void show_sorted() const; // 【新增】
    void done(int id);                
    void save() const;                
    void load();
    void remove(int id);
};