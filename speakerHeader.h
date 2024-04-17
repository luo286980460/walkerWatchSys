#ifndef SPEAKERHEADER_H
#define SPEAKERHEADER_H

/*
 * 播放MP3文件 (播放类型， 次数/时间)
 *  %1 文件名
 *      注意：歌曲名必须命名为“数字.mp3”，数字大小范围1-100不能使用中文
 *
 *  %2 播放类型：
 *      1-次数播放，播放参数为要播放的次数，单位次；
 *      2-时长播放，播放参数为要播放的时长，单位秒；
 *
 *  %3 播放类型对应的参数：AA	04	01	06	播放内容16进制	BB	EF
 *      单位次；
 *      单位秒；
*/
#define CMD_PLAY_MP3_NAME_TYPE  "AA 06 01 10 %1.mp3 %2 %3 BB EF"

/*
 * TTS (播放类型， 次数/时间)
 *  %1 播放的内容转16进制 (UTF-8)
 *
 *  %2 播放类型：
 *      1-次数播放，播放参数为要播放的次数，单位次；
 *      2-时长播放，播放参数为要播放的时长，单位秒；
 *
 *  %3 播放类型对应的参数：AA	04	01	06	播放内容16进制	BB	EF
 *      单位次；
 *      单位秒；
*/
#define CMD_PLAY_TTS_NAME_TYPE  "AA 06 01 11 %1 %2 %3 BB EF"

/* 停止播放 */
#define CMD_STOP_PLAY  "AA 03 01 02 BB EF"

/* 调节音量（1~7） */
#define CMD_SET_VOLUME  "AA 04 01 03 %1 BB EF"

/*
 * 闪灯开关
 *
 *  %1 （0/1）
 *      0-表示关闭，即闪灯一直不亮
 *      1-表示打开，即播放中闪灯亮，不播放闪灯不亮
*/
#define CMD_SET_LIGHT_SWITCH  "AA 04 01 06 %1 BB EF"


#endif // SPEAKERHEADER_H
