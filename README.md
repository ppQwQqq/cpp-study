# C++ 现代语法实战笔记（To-Do List 项目版）

> 结构：**知识点 + 项目真实代码 + 底层解析**
> 所有示例代码均取自 To-Do List 项目本身，可直接作为专属 C++ 字典查阅。

---

## 目录

- [C++ 现代语法实战笔记（To-Do List 项目版）](#c-现代语法实战笔记to-do-list-项目版)
  - [目录](#目录)
  - [第一章：基础语法与现代特性 ](#第一章基础语法与现代特性-)
    - [1. 自动类型推导 (`auto`) ](#1-自动类型推导-auto-)
    - [2. 范围 `for` 循环与常量引用 ](#2-范围-for-循环与常量引用-)
    - [3. 输入缓冲区清理 ](#3-输入缓冲区清理-)
  - [第二章：面向对象 (OOP) ](#第二章面向对象-oop-)
    - [1. 封装与命名规范 ](#1-封装与命名规范-)
    - [2. 构造函数与初始化列表 ](#2-构造函数与初始化列表-)
    - [3. 析构函数与 RAII 机制 ](#3-析构函数与-raii-机制-)
  - [第三章：STL 容器 ① `std::vector` ](#第三章stl-容器--stdvector-)
    - [1. 迭代器与安全删除 ](#1-迭代器与安全删除-)
  - [第四章：STL 容器 ② `std::unordered_map` ](#第四章stl-容器--stdunordered_map-)
    - [1. 键值对存储与原地构造 ](#1-键值对存储与原地构造-)
    - [2. 极速查找 ](#2-极速查找-)
    - [3. 按键删除 ](#3-按键删除-)
  - [第五章：现代内存管理（智能指针） ](#第五章现代内存管理智能指针-)
    - [1. 核心理解：为什么需要智能指针？ ](#1-核心理解为什么需要智能指针-)
    - [2. 语法操作 ](#2-语法操作-)
  - [第六章：底层 I/O 与异常处理 ](#第六章底层-io-与异常处理-)
    - [1. 文件读写与 RAII ](#1-文件读写与-raii-)
    - [2. 字符串流切分 ](#2-字符串流切分-)
    - [3. 异常捕获 ](#3-异常捕获-)

---

## 第一章：基础语法与现代特性 <a id="ch1"></a>

### 1. 自动类型推导 (`auto`) <a id="ch1-1"></a>

*   **项目代码**：
    ```cpp
    auto it = m_tasks.find(id);
    ```
*   **语法解析**：让编译器在编译期推断类型。对于 `unordered_map` 这种底层结构极其复杂的迭代器类型，用 `auto` 既省字又不会写错。

### 2. 范围 `for` 循环与常量引用 <a id="ch1-2"></a>

*   **项目代码**：
    ```cpp
    for (const auto& pair : m_tasks) { ... }
    ```
*   **语法解析**：
    *   `auto&`：使用引用，避免拷贝整个 `pair` 对象（深拷贝极慢）。
    *   `const`：承诺在循环内部绝对不修改字典里的数据（只读）。

### 3. 输入缓冲区清理 <a id="ch1-3"></a>

*   **项目代码**：
    ```cpp
    std::cin >> choice;
    std::cin.ignore(); 
    std::getline(std::cin, desc);
    ```
*   **语法解析**：`std::cin >>` 读取数字后，会把回车符（`\n`）留在缓冲区。如果不用 `ignore()` 吃掉它，紧接着的 `getline` 会读到一个空行，导致程序"跳过"用户的输入。

---

## 第二章：面向对象 (OOP) <a id="ch2"></a>

### 1. 封装与命名规范 <a id="ch2-1"></a>

*   **项目代码**：
    ```cpp
    class Task {
    private:
        int m_id;
        std::string m_desc;
        bool m_completed = false; 
    public:
        int getId() const;
    };
    ```
*   **语法解析**：
    *   `private` 隐藏数据，`public` 暴露接口。
    *   `m_` 前缀是工程规范，代表 Member（成员变量），一眼区分出它是类内部的数据，而不是函数传进来的参数。
    *   `= false`：C++11 允许在类内直接给成员变量默认值。

### 2. 构造函数与初始化列表 <a id="ch2-2"></a>

*   **项目代码**：
    ```cpp
    Task::Task(int id, const std::string& desc, bool completed) 
        : m_id(id), m_desc(desc), m_completed(completed) {}
    ```
*   **语法解析**：
    *   冒号 `:` 后面是初始化列表。它的执行时机在对象分配内存的瞬间，比在大括号 `{}` 内赋值效率更高。

### 3. 析构函数与 RAII 机制 <a id="ch2-3"></a>

*   **项目代码**：
    ```cpp
    Manager::~Manager() {
        try {
            save(); 
        } catch (const std::exception& e) { ... }
    }
    ```
*   **语法解析**：
    *   析构函数在对象生命周期结束时自动调用。你在这里实现了**自动存盘**。
    *   **工程铁律**：析构函数中绝对不能抛出异常（会导致程序直接崩溃）。所以必须用 `try-catch` 把存盘操作包起来。

---

## 第三章：STL 容器 ① `std::vector` <a id="ch3"></a>

### 1. 迭代器与安全删除 <a id="ch3-1"></a>

*   **项目代码**（早期版本）：
    ```cpp
    for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it) {
        if (it->getId() == id) {
            m_tasks.erase(it);
            return; 
        }
    }
    ```
*   **语法解析**：在 `vector` 中删除元素，**绝对不能用范围 for**（会导致迭代器失效崩溃）。必须用迭代器，且 `erase` 删除后必须立刻 `return` 或 `break`。

---

## 第四章：STL 容器 ② `std::unordered_map` <a id="ch4"></a>

### 1. 键值对存储与原地构造 <a id="ch4-1"></a>

*   **项目代码**：
    ```cpp
    std::unordered_map<int, std::unique_ptr<Task>> m_tasks;
    m_tasks.emplace(m_next_id, std::make_unique<Task>(m_next_id, desc));
    ```
*   **语法解析**：
    *   底层是哈希表，存储结构是 `std::pair<const Key, Value>`。
    *   `emplace`：直接在哈希表内部原地构造键值对，避免了临时对象创建和拷贝，性能极高。

### 2. 极速查找 <a id="ch4-2"></a>

*   **项目代码**：
    ```cpp
    auto it = m_tasks.find(id);
    if (it != m_tasks.end()) {
        it->second->markCompleted();
    }
    ```
*   **语法解析**：
    *   `find(id)` 的查找时间复杂度是 \(O(1)\)（极快）。
    *   `it->first` 是键（id），`it->second` 是值（Task 的智能指针）。

### 3. 按键删除 <a id="ch4-3"></a>

*   **项目代码**：
    ```cpp
    int count = m_tasks.erase(id);
    if (count == 0) { ... }
    ```
*   **语法解析**：`erase` 直接传键（Key）即可删除，返回值代表删除的元素个数（0 或 1）。

---

## 第五章：现代内存管理（智能指针） <a id="ch5"></a>

### 1. 核心理解：为什么需要智能指针？ <a id="ch5-1"></a>

*   **实体对象（返回 `Task`）**：就像车行直接把一辆巨大的实体车开进你家的小院子（**栈**）。院子小停不下不说，你想让朋友看看，还得重新克隆一辆一模一样的（**深拷贝极慢**）。
*   **裸指针（返回 `Task*`）**：车行把车停在无限大的公共停车场（**堆**），只给你一张写着车位号的**纸条**。但车行不管你以后怎么停。你要是把纸条弄丢了，这辆车就永远占着车位，变成僵尸车（**内存泄漏**）。
*   **智能指针（返回 `std::unique_ptr<Task>`）**：车行把车停在公共停车场，然后给了你一个**绑定了车位的智能钥匙**。你只要拿着钥匙，车就是你的；你把钥匙一扔，车场系统立刻自动把车拉去报废，腾出车位（**RAII 自动清理，绝不泄漏**）。

### 2. 语法操作 <a id="ch5-2"></a>

*   **项目代码**：
    ```cpp
    // 创建：在堆上构造，返回智能指针
    auto task_ptr = std::make_unique<Task>(id, desc);
    
    // 访问：因为它是指针，必须用 ->
    task_ptr->markCompleted();
    ```
*   **语法解析**：
    *   `unique_ptr` 是独占指针，严禁拷贝（如 `auto p2 = p1;` 会报错）。
    *   如果一定要转移所有权，必须用 `std::move(ptr)`。

---

## 第六章：底层 I/O 与异常处理 <a id="ch6"></a>

### 1. 文件读写与 RAII <a id="ch6-1"></a>

*   **项目代码**：
    ```cpp
    std::ofstream file(m_file_path);
    if (!file) throw std::runtime_error("无法打开文件进行保存");
    ```
*   **语法解析**：文件流对象离开作用域时会自动调用 `close()` 关闭文件。这就是 RAII 机制。

### 2. 字符串流切分 <a id="ch6-2"></a>

*   **项目代码**：
    ```cpp
    std::stringstream ss(line);
    std::getline(ss, item, '|');
    int id = std::stoi(item);
    ```
*   **语法解析**：
    *   `stringstream` 把字符串包装成流。
    *   `getline` 遇到 `|` 就停下，把前面的内容存入 `item`。
    *   `stoi` 把字符串转为整数。

### 3. 异常捕获 <a id="ch6-3"></a>

*   **项目代码**：
    ```cpp
    catch (const std::exception& e) {
        std::cerr << "警告: " << e.what() << std::endl;
    }
    ```
*   **语法解析**：
    *   必须用 `const std::exception&`（常量引用）捕获，防止对象切片（保留多态性）。
    *   `e.what()` 用于提取抛出的错误信息。
>>>>>>> 40fe5f3 (refactor: 引入智能指针管理Task，并添加README文档)
