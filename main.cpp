#include <iostream>
#include <string.h>
#include <vector>

struct file_header
{
    int filesize;
    char filename[256];
};

#define ARRAY_NUM(arr) (sizeof(arr)/sizeof(arr[0]))

void pack(std::vector<char*>& file_arr, const char* packname){

    // int len = sizeof(file_header);

    // printf("struct len : %d", len);

    //create new file
    FILE* fp = fopen(packname, "wb");
    if (fp == NULL)
    {
        printf("open new file failed !!!\n");
        throw std::logic_error("open new file failed !!!");
    }

    for (int i = 0; i < file_arr.size(); ++i)
    {
        const char* filename = file_arr[i];

        //open file
        FILE* tfp = fopen(filename, "rb");
        if (NULL == tfp)
        {
            printf("open file: %s failed !!!\n", filename);
            continue;
        }

        //calculate file size
        fseek(tfp, 0, SEEK_END);
        long filesize = ftell(tfp);
        fseek(tfp, 0, SEEK_SET);

        //write the file_header to 1.pack
        struct file_header hdr = { 0 };
        hdr.filesize = filesize;
        strcpy(hdr.filename, filename);
        fwrite(&hdr, 1, sizeof(hdr), fp);

        //write the file_body to 1.pack
        char buffer[4096] = { 0 };
        while (1)
        {
            int ret = fread(buffer, 1, 4096, tfp);
            if (ret <= 0)
            {
                break;
            }
            fwrite(buffer, 1, ret, fp);
        }

        //close tfp
        fclose(tfp);
        printf("file: %s pack success.\n", filename);

        
    }
    //close fp
    fclose(fp);
}

void unpack(const char* unpackname){
    //create new file
    FILE* fp = fopen(unpackname, "rb");
    if (fp == NULL)
    {
        printf("open new file failed !!!\n");
        throw std::logic_error("open new file failed !!!");
    }
    
    struct file_header hdr;
    while (1)
    {
        int ret = fread(&hdr, 1, sizeof(hdr), fp);
        if (ret <= 0) 
        {
            break;
        }

        FILE* tfp = fopen(hdr.filename, "wb");
        if (tfp == NULL)
        {
            fseek(fp, hdr.filesize, SEEK_CUR);
            continue;
        }
        
        int size = hdr.filesize;
        char buffer[4096] = { 0 };
        while (size > 0)
        {
            ret = fread(buffer, 1, size < 4096 ? size : 4096, fp);
            size -= ret;
            fwrite(buffer, 1, ret, tfp);
        }

        //close tfp
        fclose(tfp);
    }
    
    //close fp
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
    ; 
    
    return 0;
}
