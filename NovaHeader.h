#ifndef NOVAHEADER_H
#define NOVAHEADER_H

/*
 * 节目1 默认节目
 * 页面数量：2
 * 第一页内容：默认节目图片
 * 第二页内容：默认节目文字
*/
//#define PROGRAM1 "\
//[all]\r\n\
//items=2\r\n\
//[item1]\r\n\
//param=50,1,1,1,0,5,1\r\n\
//img1=0,0,Program1Item1Pic.jpg,0,240,360\r\n\
//imgparam1=50\r\n\
//[item2]\r\nparam=50,1,1,1,0,5,1\r\n\
//txtext1=0,0,240,360,黑体,70,0,2,2,0,30,ff0000,00000000,0,1,50,1,行穿人越请车勿道,0,0,0,0,0,0\r\n\
//info=240,360";
#define PROGRAM1 "\
[all]\r\n\
items=1\r\n\
[item1]\r\nparam=50,1,1,1,0,5,1\r\n\
txtext1=0,0,240,360,黑体,70,0,2,2,0,30,ff0000,00000000,0,1,50,1,行穿人越请车勿道,0,0,0,0,0,0\r\n\
info=240,360";
#define PROGRAM1_ITEM1_PIC "Program1Item1Pic.jpg"

/*
 * 节目2 默认违法节目（临时使用）
 * 页面数量：1
 * 第一页内容：默认违法节目图片
*/
//#define PROGRAM2 "\
//[all]\r\n\
//items=1\r\n\
//[item1]\r\n\
//param=100,1,1,1,0,5,1\r\n\
//txtext1=0,0,240,360,黑体,70,0,2,2,0,30,ff0000,00000000,1,4,100,1,横立穿即危返险回,0,0,0,0,0,0\r\n\
//info=240,360";

#define PROGRAM2 "\
[all]\r\n\
items=1\r\n\
[item1]\r\n\
param=50,1,1,1,0,5,1\r\n\
img1=0,0,Program2Item1Pic.jpg,0,240,360\r\n\
imgparam1=50";

#define PROGRAM2_ITEM1_PIC "Program2Item1Pic.jpg"

/*
 * 节目3 违法节目
 * 页面数量：1
 * 第一页内容：违法照片
 * 第一页内容：默认违法节目图片
*/
#define PROGRAM3 "\
[all]\r\n\
items=2\r\n\
[item1]\r\n\
param=50,1,1,1,0,5,1\r\n\
img1=0,0,Program3Item1Pic.jpg,0,240,360\r\n\
imgparam1=50\r\n\
[item2]\r\n\
param=100,1,1,1,0,5,1\r\n\
txtext1=0,0,240,360,黑体,70,0,2,2,0,30,ff0000,00000000,1,4,100,1,横立穿即危返险回,0,0,0,0,0,0\r\n\
info=240,360";

#define PROGRAM3_ITEM1_PIC "Program3Item1Pic.jpg"
#define PROGRAM3_ITEM2_PIC "Program3Item2Pic.jpg"


#endif // NOVAHEADER_H
