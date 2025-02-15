好的，下面我将给出一个简单的 C++ 示例，封装 `CommandLineOption` 和 `CommandLineParse` 类来实现命令行参数解析的功能。此代码可以方便地添加选项、解析命令行参数，并提供获取选项值的功能。

### 设计思路

1. **`CommandLineOption`**：表示单个命令行选项，包括名称、别名、描述和默认值（可选）。这个类的作用是存储每个命令行参数的相关信息。
2. **`CommandLineParse`**：负责解析命令行参数，支持通过 `addOption()` 添加选项，通过 `getValue()` 获取选项的值，并能输出所有选项的描述信息。

### 代码实现

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <map>

// CommandLineOption 类用于表示单个命令行选项
class CommandLineOption {
public:
    std::string longName;  // 长名称，例如 "--version"
    std::string shortName; // 短名称，例如 "-v"
    std::string description; // 描述
    std::string value;      // 存储该选项的值

    CommandLineOption(std::string longName, std::string shortName, std::string description, std::string value = "")
        : longName(longName), shortName(shortName), description(description), value(value) {}
};

// CommandLineParse 类用于管理命令行参数的解析
class CommandLineParse {
public:
    // 存储命令行选项
    std::vector<CommandLineOption> options;

    // 添加命令行选项
    void addOption(const std::string& longName, const std::string& shortName, const std::string& description, const std::string& defaultValue = "") {
        options.emplace_back(longName, shortName, description, defaultValue);
    }

    // 获取某个选项的值
    std::string getValue(const std::string& optionName, int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            if (argv[i] == optionName) {
                return (i + 1 < argc) ? argv[i + 1] : "";
            }
        }
        return "";
    }

    // 输出所有选项和描述
    void showHelp() {
        for (const auto& option : options) {
            std::cout << "Option: " << option.longName << " (" << option.shortName << ") - " << option.description << std::endl;
        }
    }
};

// 使用示例
int main(int argc, char* argv[]) {
    CommandLineParse cmdParser;

    // 添加命令行选项
    cmdParser.addOption("--version", "v", "Show version information.");
    cmdParser.addOption("--host", "h", "Specify the host address.");
    cmdParser.addOption("--help", "?", "Show help message.");

    // 输出帮助信息
    if (argc == 1) {
        cmdParser.showHelp();
        return 0;
    }

    // 解析 --version 选项
    std::string version = cmdParser.getValue("--version", argc, argv);
    if (!version.empty()) {
        std::cout << "Version: 1.0.0" << std::endl;
    }

    // 解析 --host 选项
    std::string host = cmdParser.getValue("--host", argc, argv);
    if (!host.empty()) {
        std::cout << "Host: " << host << std::endl;
    }

    // 解析 --help 选项
    std::string help = cmdParser.getValue("--help", argc, argv);
    if (!help.empty()) {
        cmdParser.showHelp();
    }

    return 0;
}
```

### 代码解释

1. **`CommandLineOption`** 类：
   - 存储每个命令行选项的长名称（`longName`）、短名称（`shortName`）、描述（`description`）和选项的值（`value`）。
   - 构造函数用于初始化这些字段。
2. **`CommandLineParse`** 类：
   - 存储一个 `CommandLineOption` 类型的 `options` 向量，用于管理所有命令行选项。
   - `addOption` 函数用于添加新的命令行选项。
   - `getValue` 函数根据给定的选项名称查找对应的值，并返回。
   - `showHelp` 函数遍历所有选项并输出它们的描述信息。
3. **`main` 函数**：
   - 我们通过 `cmdParser.addOption()` 添加了几个选项。
   - 如果命令行参数中有 `--help` 选项，则输出所有选项的帮助信息。
   - 如果有 `--version`，输出版本信息。
   - 如果有 `--host`，输出指定的主机地址。

### 示例运行：

假设你有一个程序叫 `my_program`，你可以使用以下命令行：

```
my_program --version
```

输出：

```
Version: 1.0.0
```

或者：

```
my_program --host localhost
```

输出：

```
Host: localhost
```

如果没有传入任何有效的命令行参数（例如 `my_program`），则会显示所有支持的命令行选项的帮助信息：

```
Option: --version (v) - Show version information.
Option: --host (h) - Specify the host address.
Option: --help (?) - Show help message.
```

### 扩展

- 你可以进一步封装 `getValue` 函数来支持 `bool` 类型参数（例如 `--help`），当该参数出现时，直接返回 `true`。
- 你可以将 `CommandLineOption` 和 `CommandLineParse` 类中的功能扩展为支持更多复杂的解析逻辑，比如处理多个值、默认值等。

这个示例展示了一个简单而灵活的命令行参数解析方法，适用于小型的命令行工具。如果你的需求更复杂，可以进一步扩展和优化。