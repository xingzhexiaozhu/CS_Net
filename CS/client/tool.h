#ifndef TOOL_H_
#define TOOL_H_

#include <string>

// 判断字符串是否为OD_*开始
inline bool is_start_with_od_(const std::string& str, const std::string reg="OD_") 
{
    return str.find(reg) == 0?1:0;
}

#endif