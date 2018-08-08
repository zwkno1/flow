#pragma once

#include <map>
#include <string>

#include <flow/slot.h>

#ifdef __cplusplus
extern "C" 
{
#endif

SlotPtr createSlot(const std::map<std::string, std::string> & paras);

#ifdef __cplusplus
}
#endif
