#include "Manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>    // 【新增】
#include <algorithm> // 【新增】

Manager::Manager(const std::string& file_path) : m_file_path(file_path) 
{
    load();
}

Manager::~Manager() 
{
    try 
    {
        save(); 
        std::cout << "\n[系统] 数据已自动保存至 " << m_file_path << std::endl;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "\n[系统警告] 自动保存失败: " << e.what() << std::endl;
    }
}

// 【修改】增加优先级传参
void Manager::add(const std::string& desc, Priority priority) 
{
    m_tasks.emplace(m_next_id, std::make_unique<Task>(m_next_id, desc, priority));
    m_next_id++;
    save();
    std::cout << "添加成功！\n";
}

// 【新增】按优先级排序展示
void Manager::show_sorted() const 
{
    if (m_tasks.empty()) 
    {
        std::cout << "当前没有任务。\n";
        return;
    }

    std::vector<Task*> task_ptrs;
    for (const auto& pair : m_tasks) 
    {
        task_ptrs.push_back(pair.second.get()); 
    }

    std::sort(task_ptrs.begin(), task_ptrs.end(), 
        [](const Task* a, const Task* b) 
        {
            return a->getPriority() < b->getPriority(); 
        }
    );

    for (const auto* task : task_ptrs) 
    {
        std::string status = task->isCompleted() ? "[已完成] " : "[未完成] ";
        
        std::string pri_str;
        if (task->getPriority() == Priority::HIGH) pri_str = "[高] ";
        else if (task->getPriority() == Priority::MEDIUM) pri_str = "[中] ";
        else pri_str = "[低] ";

        std::cout << "[" << task->getId() << "] " << pri_str << status << task->getDesc() << "\n";
    }
}

void Manager::done(int id) 
{
    auto it = m_tasks.find(id); 
    if (it != m_tasks.end()) 
    {
        it->second->markCompleted(); 
        save(); 
        std::cout << "任务 [" << id << "] 已标记为完成！\n";
    } 
    else 
    {
        std::cout << "未找到 ID 为 " << id << " 的任务。\n";
    }
}

void Manager::remove(int id) 
{
    int count = m_tasks.erase(id);
    if (count > 0) 
    {
        save();
        std::cout << "任务 [" << id << "] 已成功删除！\n";
    } 
    else 
    {
        std::cout << "未找到 ID 为 " << id << " 的任务，无法删除。\n";
    }
}

// 【修改】存盘时加入优先级
void Manager::save() const 
{
    std::ofstream file(m_file_path); 
    if (!file) throw std::runtime_error("无法打开文件进行保存: " + m_file_path);

    for (const auto& pair : m_tasks) 
    {
        int pri_num = static_cast<int>(pair.second->getPriority()); // 【新增】
        
        file << pair.first << "|" 
             << pair.second->getDesc() << "|" 
             << (pair.second->isCompleted() ? "1" : "0") << "|"
             << pri_num << "\n"; // 【修改】末尾追加了优先级
    }
}

// 【修改】读盘时还原优先级
void Manager::load() 
{
    std::ifstream file(m_file_path);
    if (!file.is_open()) return; 

    m_tasks.clear(); 
    m_next_id = 1;

    std::string line;
    while (std::getline(file, line)) 
    {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string item;

        try 
        {
            std::getline(ss, item, '|'); 
            int id = std::stoi(item);

            std::getline(ss, item, '|'); 
            std::string desc = item;

            std::getline(ss, item, '|'); 
            bool completed = (item == "1");

            // 【新增】处理优先级解析
            Priority pri = Priority::MEDIUM;
            if (std::getline(ss, item, '|')) 
            {
                int p = std::stoi(item);
                if (p == 1) pri = Priority::HIGH;
                else if (p == 3) pri = Priority::LOW;
            }

            m_tasks.emplace(id, std::make_unique<Task>(id, desc, pri, completed));
            
            if (id >= m_next_id) m_next_id = id + 1;
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "【警告】解析出错，跳过此行: " << line << "\n";
        }
    }
}

void Manager::show() const 
{
    if (m_tasks.empty()) 
    { 
        std::cout << "当前没有任务。\n"; 
        return; 
    }
    
    for (const auto& pair : m_tasks) 
    {
        std::string status = pair.second->isCompleted() ? "[已完成] " : "[未完成] ";
        std::cout << "[" << pair.first << "] " << status << pair.second->getDesc() << "\n";
    }
}