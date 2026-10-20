// =========================================================================
// 程序入口。只负责跟用户聊天，脏活累活都交给 Manager 去做。
// =========================================================================
#include <iostream>
#include <string>
#include "Manager.h"

int main() 
{
    // 【RAII 机制】在栈上创建对象 mgr。
    // 当 main 函数结束时，mgr 会自动销毁，触发它的析构函数 ~Manager() 自动存盘。
    Manager mgr("tasks.db"); 
    int choice;

    while (true) // 死循环，直到用户选择退出
    {
        std::cout << "\n=== 待办事项 ===\n";
        std::cout << "1. 查看任务\n";
        std::cout << "2. 添加任务\n";
        std::cout << "3. 标记完成\n";
        std::cout << "4. 退出\n"; 
        std::cout << "请选择: ";
        
        std::cin >> choice; // 把用户输入的数字读进 choice

        if (choice == 1) 
        {
            mgr.show();
        } 
        else if (choice == 2) 
        {
            std::cout << "输入任务描述: ";
            std::string desc;
            
            // 【史诗级大坑！必看！】
            // 刚才 std::cin >> choice; 敲完回车后，那个“回车符(\n)”还留在内存里。
            // 如果不加 ignore() 把它吃掉，下面的 getline 会直接读到一个空气，导致程序跳过输入。
            std::cin.ignore(); 
            
            // getline 能读取包含空格的一整句话（比如 "去食堂 吃 饭"）
            std::getline(std::cin, desc); 
            mgr.add(desc);
        } 
        else if (choice == 3) 
        {
            std::cout << "输入要完成的任务 ID: ";
            int id;
            std::cin >> id;
            mgr.done(id);
        }
        else if (choice == 4) 
        {
            std::cout << "输入要删除的任务 ID: ";
            int id;
            std::cin >> id;
            mgr.remove(id);
        }
        else if (choice == 5) 
        {
            std::cout << "正在退出...\n";
            break; // 跳出循环，结束程序，触发析构存盘
        } 
        else 
        {
            std::cout << "输入错误，请重新选。\n";
        }
    }
    return 0;
}