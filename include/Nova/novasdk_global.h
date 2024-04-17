#pragma once

//宏定义导出
#if defined NOVA_TRAFFIC_DLL//如果没有定义DLLH 就定义 DLLH __declspec(dllexport)
#define NOVA_TRAFFIC_DLL __declspec(dllexport)//导出
#else
#define NOVA_TRAFFIC_DLL __declspec(dllimport)//导入
#endif
