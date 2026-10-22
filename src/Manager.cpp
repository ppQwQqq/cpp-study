// =========================================================================
// Manager 的实现文件
// =========================================================================
#include "Manager.h"
#include <iostream>  
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
    // 在析构函数里，千万千万不能让异常跑出去（否则程序直接崩溃闪退）。
    // 所以我们用 try-catch 把 save() 包起来，哪怕存盘失败，也只打印警告，安全退出。
    try {
        save(); 
        std::cout << "\n[系统] 数据已自动保存至 " << m_file_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "\n[系统警告] 自动保存失败: " << e.what() << std::endl;
    }
    // 【语法】const std::exception& e：捕获标准异常基类的常量引用。
    // 使用引用是为了防止对象切片（保留多态性），使用 const 是因为不需要修改异常对象。
}

void Manager::add(const std::string& desc) 
{
    // 【语法】emplace 是 C++11 引入的容器操作。
    // 它接收构造函数的参数，直接在 map 内部原地构造 pair，避免了临时对象的创建和拷贝。
    // 【语法拆解】m_tasks.emplace(m_next_id, Task(m_next_id, desc)); 
    // 1. m_next_id (第一个参数)：这是哈希表的【键 (Key)】，类型是 int。
    // 2. Task(m_next_id, desc) (第二个参数)：这是哈希表的【值 (Value)】。
     // 【核心语法】std::make_unique<Task>(m_next_id, desc)
    // 1. 它在堆（Heap）内存上动态分配内存，并调用 Task 的构造函数。
    // 2. 它返回一个 std::unique_ptr<Task> 类型的智能指针对象。
    // 3. emplace 会直接把这对“键值对”原地构造进哈希表中，零拷贝。
    m_tasks.emplace(m_next_id,std::make_unique<Task>(m_next_id, desc));
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
    
    // 【语法】范围 for 循环遍历 map。
    // 每次迭代返回一个 pair 对象。
    // const auto& pair：auto 推导为 std::pair<const int, Task>，& 避免拷贝整个 pair，const 保证只读。
    for (const auto& pair : m_tasks) 
    {
        // 【核心语法】pair.second 现在的类型是 std::unique_ptr<Task>，它是个指针对象。
        // 1. 所以不能再用 . 访问成员（如 pair.second.getDesc() 是错的）。
        // 2. 必须用 -> 访问（如 pair.second->getDesc()）。

        //用 const auto& 绑定智能指针
        const auto& task = pair.second;
        std::string status = task->isCompleted() ? "[已完成] " : "[未完成] ";
        std::cout << "[" << task->getId() << "] " << status << task->getDesc() << "\n";
    }
}

void Manager::done(int id) 
{
    // 【语法】find() 返回一个迭代器（iterator）。
    // 如果找到了，迭代器指向该键值对；如果没找到，返回 m_tasks.end()。
    auto it=m_tasks.find(id);
    if(it!=m_tasks.end())
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

void Manager::save() const 
{
    // 【输出流】ofstream (Output File Stream)。
    // 打开文件准备写入，如果文件不存在会自动创建，存在则会清空重写。
    std::ofstream file(m_file_path); 
    
    // 【防呆设计】如果打开失败（比如文件被其他程序占用），抛出异常终止操作。
    if (!file) throw std::runtime_error("无法打开文件进行保存: " + m_file_path);

    // 遍历
    for (const auto& pair : m_tasks) 
    {
        const auto& task = pair.second;
        file << task->getId() << "|" 
             << task->getDesc() << "|" 
             << (task->isCompleted() ? "1" : "0") << "\n";
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

            m_tasks.emplace(id,Task(id, desc, completed)); // 放进货架
            
            if (id >= m_next_id)
            {
                m_next_id=id+1;
            }
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
    // 【语法】erase(key) 会删除指定的键，并返回被删除的元素数量（size_t 类型）。
    // 如果键存在，返回 1；如果不存在，返回 0。这比先用 find 再 erase 更高效。
   int cout=m_tasks.erase(id);
   if(cout==0)
    {
        std::cout << "未找到 ID 为 " << id << " 的任务。\n";
    }
    else
    {
        std::cout << "任务 [" << id << "] 已成功删除！\n";
    }
}