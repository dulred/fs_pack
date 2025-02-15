#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <cstdint>
#include <iostream>

#if defined(_WIN32)
#define fseek _fseeki64
#define ftell _ftelli64
#else
#define fseek fseeko
#define ftell ftello
#endif


struct file_header
{
    int64_t filesize;
    char filename[256];
};

#define ARRAY_NUM(arr) (sizeof(arr)/sizeof(arr[0]))

void pack(std::vector<char*>& file_arr, const char* packname) {
    // 创建新文件
    FILE* fp = fopen(packname, "wb");
    if (fp == NULL) {
        printf("打开打包文件失败 !!!\n");
        throw std::logic_error("打开打包文件失败 !!!");
    }

    for (int i = 0; i < file_arr.size(); ++i) {
        const char* filename = file_arr[i];

        // 打开文件
        FILE* tfp = fopen(filename, "rb");
        if (tfp == NULL) {
            printf("打开文件: %s 失败 !!!\n", filename);
            continue;
        }


        // 计算文件大小
        fseek(tfp, 0, SEEK_END);  // 使用 _fseeki64
        int64_t filesize = ftell(tfp);  // 使用 _ftelli64
        fseek(tfp, 0, SEEK_SET);
        printf("filesize : %lld \n", filesize);  // 使用 %lld 来打印 int64_t 类型
        // 写入文件头
        file_header hdr = { 0 };
        hdr.filesize = filesize;
        strncpy(hdr.filename, filename, sizeof(hdr.filename) - 1);  // 使用 strncpy 防止越界
        fwrite(&hdr, sizeof(hdr), 1, fp);

        // 写入文件内容
        char buffer[4096] = { 0 };
        while (1) {
            size_t ret = fread(buffer, 1, sizeof(buffer), tfp);
            if (ret == 0) break;
            fwrite(buffer, 1, ret, fp);
        }

        // 关闭文件
        fclose(tfp);
        printf("文件: %s 打包成功.\n", filename);
    }

    // 关闭打包文件
    fclose(fp);
}

void unpack(const char* unpackname) {
    // 打开打包文件
    FILE* fp = fopen(unpackname, "rb");
    if (fp == NULL) {
        printf("打开解包文件失败 !!!\n");
        throw std::logic_error("打开解包文件失败 !!!");
    }

    file_header hdr;
    while (1) {
        size_t ret = fread(&hdr, sizeof(hdr), 1, fp);
        if (ret == 0) break;

        FILE* tfp = fopen(hdr.filename, "wb");
        if (tfp == NULL) {
            fseek(fp, hdr.filesize, SEEK_CUR);
            continue;
        }

        int64_t size = hdr.filesize;
        char buffer[4096] = { 0 };
        while (size > 0) {
            ret = fread(buffer, 1, size < sizeof(buffer) ? size : sizeof(buffer), fp);
            fwrite(buffer, 1, ret, tfp);
            size -= ret;
        }

        // 关闭文件
        fclose(tfp);
    }

    // 关闭解包文件
    fclose(fp);
}

int main(int argc, char const *argv[])
{
    //  // 打印传入的参数数量
    // std::cout << "Number of arguments: " << argc << std::endl;

    // // 遍历并打印每个参数
    // for (int i = 0; i < argc; ++i) {
    //     std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    // }

    // 解析具体参数
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            std::cout << "Help: Use -p <name1> <name2> <pack_name> to pack two file" << std::endl;
            std::cout << "Help: Use -up <name> to unpack a file" << std::endl;
        } 
        else if (arg == "-p" && i + 1 < argc) {
             std::string name1 = argv[++i]; 
             std::string name2 = argv[++i]; 
             std::string pack_name = argv[++i];
             
            // 使用 std::vector 来动态管理文件名
            std::vector<char*> file_arr;
            // 动态添加 name1 和 name2
            file_arr.push_back(const_cast<char*>(name1.c_str()));
            file_arr.push_back(const_cast<char*>(name2.c_str()));
            // std::cout << "packname" << packname << std::endl;
            pack(file_arr,pack_name.c_str());

        } else if (arg == "-up" && i + 1 < argc)
        {
            
            std::string name = argv[++i]; 
            unpack(name.c_str());
            // std::cout << "unpackname" << unpackname << std::endl;
        }
        else {
            std::cout << "Unknown argument: " << arg << std::endl;
        }
    }
    return 0;
}
