1.默认参数设计
2.不定长参数解析

3.错误提示处理

4.特殊参数规则的解析 -p3306 

5.c++ 封装两个类: CommandLineOption CommandLineParse
实现命令行的参数的解析操作，方便的输出所有命令
addOption("--version", "v", "Get list of available commands.", NULL);
getValue("--host", argv);



```
#include <iostream>
#include <cstdlib>  // for realloc

int main(int argc, char* argv[]) {
    int initial_size = 2;  // 初始分配2个位置
    char** helpValues = (char**)malloc(initial_size * sizeof(char*));
    if (helpValues == NULL) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return 1;
    }

    int helpCount = 0;  // 当前参数数量

    // 遍历命令行参数（从argv[1]开始）
    for (int i = 1; i < argc; ++i) {
        // 如果当前 helpCount 达到分配的大小，就重新分配内存（每次增加1）
        if (helpCount >= initial_size) {
            // 扩展内存，增加1个元素大小
            helpValues = (char**)realloc(helpValues, (helpCount + 1) * sizeof(char*));
            if (helpValues == NULL) {
                std::cerr << "Memory allocation failed!" << std::endl;
                return 1;
            }
        }

        // 存储当前参数
        helpValues[helpCount] = argv[i];
        ++helpCount;
    }

    // 打印结果
    for (int i = 0; i < helpCount; ++i) {
        std::cout << "Argument " << i + 1 << ": " << helpValues[i] << std::endl;
    }

    // 释放内存
    free(helpValues);

    return 0;
}

```





