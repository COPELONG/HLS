//
// Created by bxc on 2023/1/7.
//

#ifndef BXC_MEDIASERVER_UTILS_H
#define BXC_MEDIASERVER_UTILS_H
#include <string>
#include <chrono>
#include <vector>

static int64_t getCurTime()// 获取当前系统启动以来的毫秒数
{
#ifndef WIN32
    // Linux系统
    struct timespec now;// tv_sec (s) tv_nsec (ns-纳秒)
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec * 1000 + now.tv_nsec / 1000000);
#else
    long long now = std::chrono::steady_clock::now().time_since_epoch().count();
    return now / 1000000;
#endif // !WIN32

}
static int64_t getCurTimestamp()// 获取毫秒级时间戳（13位）
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch()).
        count();

}

static std::string getCurFormatTime(const char *format = "%Y-%m-%d %H:%M:%S") { //

    time_t t = time(nullptr);
    char tc[64];
    strftime(tc, sizeof(tc), format, localtime(&t));

    std::string tcs = tc;
    return tcs;
}


static int genRandomInt() {
    std::string numStr;

    numStr.append(std::to_string(rand()%9 + 1));
    numStr.append(std::to_string(rand() % 10));
    numStr.append(std::to_string(rand() % 10));
    numStr.append(std::to_string(rand() % 10));
    numStr.append(std::to_string(rand() % 10));
    numStr.append(std::to_string(rand() % 10));
    numStr.append(std::to_string(rand() % 10));
    numStr.append(std::to_string(rand() % 10));
    int num = stoi(numStr);

    return num;
}

static std::vector<std::string> split(std::string file,std::string pattern) {

    std::string::size_type pos;
    std::vector<std::string> result;
    file += pattern;//扩展字符串以方便操作
    int size = file.size();
    for (int i = 0; i < size; i++) {
        pos = file.find(pattern, i);
        if (pos < size) {
            std::string s = file.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

#endif // BXC_MEDIASERVER_UTILS_H
