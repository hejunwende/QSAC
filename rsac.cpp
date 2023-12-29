#include "rsac.h"
// 提前声明函数
static void map_hd_out(char* memar, char* buff);
static void map_hd_in(char* memar, char* buff);

float* read_sac(const char* name, SACHEAD* hd){
    std::ifstream file(name, std::ios::binary); // 打开文件，以二进制模式读取
    float* data=nullptr;
    if (file.is_open()) {
        // 头段区分为头段-前和头段后
        const int head_forward_size = 440;
        const int head_after_size = 192;
        char* buffer = new char[head_after_size];

        // 读取头段区-前
        file.read(reinterpret_cast<char*>(hd), head_forward_size);

        if (file.fail()) { // 检查读取是否失败
            std::cout << "Failed to read header forward!" << std::endl;
            delete[] buffer; // 释放内存
            return nullptr;
        }

        if (hd->nvhdr != 6) {
            std::cout << name << " Is not sac format! " << std::endl;
            delete[] buffer;
            return nullptr;
        }
        file.read(buffer, head_after_size);

        if (file.fail()) { // 检查读取是否失败
            std::cout << "Failed to read header after!" << std::endl;
            delete[] buffer; // 释放内存
            return nullptr;
        }

        map_hd_in(reinterpret_cast<char*>(hd) +head_forward_size, buffer);

        // 释放内存
        delete[] buffer;

        // 读取数据区,sizeof(float)=4
        const int dataSize = hd->npts * 4; // 数据区长度
        //std::cout << hd->npts << std::endl;
        data = new float[dataSize];
        file.read(reinterpret_cast<char*>(data), dataSize);

        if (file.fail()) { // 检查读取是否失败
            std::cout << "Failed to read data! And please check the actual data length and delta!" << std::endl;
            delete[] data; // 释放内存
            return nullptr;
        }
        //for (int i = 0; i < 10; i++) {
        //    std::cout << data[i] << std::endl;
        //}
        // 关闭文件
        file.close();
    }
    else 
    {
        std::cout << "Cannot open file!" << std::endl;
    }
    return data;
}

int write_sac(const char* name, SACHEAD* hd, const float* ar)
{
    char* buffer = new char[192];
    // 打开文件，ios::binary 表示以二进制模式打开
    std::ofstream outfile(name, std::ios::binary);

    if (!outfile) {
        std::cerr << "Failed to open file for writing." << std::endl;
        delete[] buffer; // 释放内存
        return 1;
    }

    // 写入头段—前，440个字节
    outfile.write(reinterpret_cast<const char*>(hd), 440);
    // 检查写入操作是否成功
    if (outfile.bad()) {
        std::cerr << name << " Failed to write head-forword to file." << std::endl;
        delete[] buffer; // 释放内存
        return 1;
    }

    // 写入头段-后，192个字节
    map_hd_out(reinterpret_cast<char*>(hd) + 440, buffer);
    outfile.write(buffer, 192);
    // 检查写入操作是否成功
    if (outfile.bad()) {
        std::cerr << name << " Failed to write head-after to file." << std::endl;
        delete[] buffer; // 释放内存
        return 1;
    }

    // 写入浮点数数据
    outfile.write(reinterpret_cast<const char*>(ar), hd->npts * 4);

    // 检查写入操作是否成功
    if (outfile.bad()) {
        std::cerr << name << " Failed to write data to file." << std::endl;
        delete[] buffer; // 释放内存
        return 1;
    }

    // 关闭文件
    outfile.close();
    delete[] buffer; // 释放内存

    std::cout << name <<" SAC file written successfully." << std::endl;
    return 0; // 返回适当的值

}

static bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

MonthDay daysToMonthDay(int year, int days)
{
     int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (isLeapYear(year)) {
        daysInMonth[2] = 29; // February has 29 days in a leap year
    }

    int month = 1;
    while (days > daysInMonth[month]) {
        days -= daysInMonth[month];
        month++;
    }
    //创建-结果结构体，并返回结果
    MonthDay result;
    result.month = month;
    result.day = days;
    return result;
}

Time secondsToDHMS(int totalSeconds) {
    Time result;

    result.days = totalSeconds / 86400; // Calculate days
    totalSeconds %= 86400; // Remaining seconds after calculating days

    result.hours = totalSeconds / 3600; // Calculate hours
    totalSeconds %= 3600; // Remaining seconds after calculating hours

    result.minutes = totalSeconds / 60; // Calculate minutes
    result.seconds = totalSeconds % 60; // Remaining seconds after calculating minutes

    return result;
}

static void map_hd_in(char* memar, char* buff)
{
    char* ptr1 = memar;
    char* ptr2 = buff;

    std::memcpy(ptr1, ptr2, 8);
    *(ptr1 + 8) = '\0';
    ptr1 += 9;
    ptr2 += 8;

    std::memcpy(ptr1, ptr2, 16);
    *(ptr1 + 16) = '\0';
    ptr1 += 18;
    ptr2 += 16;

    for (int i = 0; i < 21; i++)
    {
        std::memcpy(ptr1, ptr2, 8);
        *(ptr1 + 8) = '\0';
        ptr1 += 9;
        ptr2 += 8;
    }
}

static void map_hd_out(char* memar, char* buff) {
    char* ptr1 = memar;
    char* ptr2 = buff;

    memcpy(ptr2, ptr1, 8);
    ptr1 += 9;
    ptr2 += 8;

    memcpy(ptr2, ptr1, 16);
    ptr1 += 18;
    ptr2 += 16;

    for (int i = 0; i < 21; i++) {
        memcpy(ptr2, ptr1, 8);
        ptr1 += 9;
        ptr2 += 8;
    }
}