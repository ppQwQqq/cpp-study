// =========================================================================
// Manager 的实现文件。最硬核的业务逻辑全在这里。
// =========================================================================
#include "Manager.h"
#include <iostream>  // 用于 std::cout 打印
#include <fstream>   // 【文件流】ofstream(写文件) 和 ifstream(读文件) 都靠它
#include <sstream>   // 【字符串流】解析文本的切菜神器
#include <stdexcept> // 【异常库】用于 throw 报错

// 【构造函数实现】使用初始化列表把路径存进去
// 紧接着在大括号里调用 load()。说明程序一启动，立刻从硬盘读数据。
Manager::Manager(const std::string& file_path) : m_file_path(file_path) 
{
    load();
}

// 【析构函数实现】程序结束准备退出时，自动跑这里
Manager::~Manager() 
{
    // 【血泪教训】在析构函数里，千万千万不能让异常跑出去（否则程序直接崩溃闪退）。
    // 所以我们用 try-catch 把 save() 包起来，哪怕存盘失败，也只打印警告，安全退出。
    try {
        save(); 
        std::cout << "\n[系统] 数据已自动保存至 " << m_file_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "\n[系统警告] 自动保存失败: " << e.what() << std::endl;
    }
}

void Manager::add(const std::string& desc) 
{
    // 【C++11 现代写法】emplace_back 
    // 它比 push_back 更牛：它直接在数组尾部“原地”造一个 Task，不需要先造好再复制，省性能。
    m_tasks.emplace_back(m_next_id, desc);
    m_next_id++; // 编号用掉了，下一个编号 +1
    
    save(); // 【实时保存】只要加了任务，立刻写进硬盘。哪怕你直接按 Ctrl+C 杀进程，数据也绝不丢。
    std::cout << "添加成功！\n";
}

void Manager::show() const 
{
    if (m_tasks.empty()) { // 如果 vector 是空的
        std::cout << "当前没有任务。\n";
        return;
    }
    
    // 【范围 for 循环】把 m_tasks 里的任务挨个拿出来叫 task
    // const auto& task：不复制对象（&），且只读不修改（const），是极致性能的写法。
    for (const auto& task : m_tasks) 
    {
        // 三目运算符：条件 ? 结果1 : 结果2。如果完成就返回"[已完成] "，否则返回"[未完成] "。
        std::string status = task.isCompleted() ? "[已完成] " : "[未完成] ";
        std::cout << "[" << task.getId() << "] " << status << task.getDesc() << "\n";
    }
}

void Manager::done(int id) 
{
    // 【核心区别】这里用的是 auto& task，【没有】const！
    // 因为我们要去修改它（调用 markCompleted），如果加了 const 编译器会报错。
    for (auto& task : m_tasks) 
    {
        if (task.getId() == id) 
        {
            task.markCompleted(); // 找到对应任务，修改它的状态
            save(); // 状态变了，立刻存盘
            std::cout << "任务 [" << id << "] 已标记为完成！\n";
            return; // 找到了并改完，直接 return 结束整个函数，不要再往后找了
        }
    }
    // 如果整个循环跑完了还没 return，说明没找到这个 ID
    std::cout << "未找到 ID 为 " << id << " 的任务。\n";
}

void Manager::save() const 
{
    // 【输出流】ofstream (Output File Stream)。
    // 打开文件准备写入，如果文件不存在会自动创建，存在则会清空重写。
    std::ofstream file(m_file_path); 
    
    // 【防呆设计】如果打开失败（比如文件被其他程序占用），抛出异常终止操作。
    if (!file) throw std::runtime_error("无法打开文件进行保存: " + m_file_path);

    // 遍历所有任务，写进文件里
    for (const auto& task : m_tasks) 
    {
        // 格式：1|去吃饭|0 
        // << 的作用就像一根吸管，把数据吸入文件流
        file << task.getId() << "|" 
             << task.getDesc() << "|" 
             << (task.isCompleted() ? "1" : "0") << "\n";
    }
    // 【RAII 魔法】不需要手动写 file.close()！
    // 函数结束时，file 对象会被自动销毁，它的析构函数会自动帮你把文件安全关闭。
}

void Manager::load() 
{
    // 【输入流】ifstream (Input File Stream)，从硬盘把数据吸进程序
    std::ifstream file(m_file_path);
    
    // 如果第一次运行，没找到文件，直接 return 结束函数，不用报错
    if (!file.is_open()) return; 

    m_tasks.clear(); // 清空旧货架
    m_next_id = 1;   // ID 从头算

    std::string line;
    // std::getline：从 file 里读一整行，遇到回车符停下，存进 line 里。
    // 只要文件还有内容，这个 while 就会一直读。
    while (std::getline(file, line)) 
    {
        if (line.empty()) continue; // 如果读到空行，直接跳过

        // 【切菜神器】std::stringstream 
        // 把读到的整行字符串（如 "1|去吃饭|0"），包装成一个“类似文件的流”，方便切分。
        std::stringstream ss(line);
        std::string item;

        try {
            // std::getline(ss, item, '|')：
            // 从 ss 里读字符，一旦遇到 '|' 就停下，把前面的内容塞进 item。
            std::getline(ss, item, '|'); 
            int id = std::stoi(item); // std::stoi：String TO Integer，把 "1" 变成数字 1

            std::getline(ss, item, '|'); 
            std::string desc = item; // 切出描述

            std::getline(ss, item, '|'); 
            bool completed = (item == "1"); // 如果是 "1"，就是 true

            m_tasks.emplace_back(id, desc, completed); // 放进货架
            
            if (id >= m_next_id) m_next_id = id + 1; // 保证 ID 不重复
        } 
        catch (const std::exception& e) {
            // 如果文件里某一行被别人恶意改成了乱码（如 "abc|1|0"），stoi 会崩溃。
            // 这个 catch 能接住错误，跳过这一行，保证程序不闪退。
            std::cerr << "【警告】解析出错，跳过此行: " << line << "\n";
        }
    }
}

void Manager::remove(int id)
{
    for(auto it=m_tasks.begin();it!=m_tasks.end();++it)
    {
        if(it->getId()==id)
        {
            it=m_tasks.erase(it);
        }
        save();
        std::cout << "任务 [" << id << "] 已成功删除！\n";
        return;
    }
    std::cout << "未找到 ID 为 " << id << " 的任务。\n";
}