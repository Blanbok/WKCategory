//
//  WKGeneralHeader.h
//  WKCategory
//
//  Created by admin on 2017/7/16.
//  Copyright © 2017年 juyuanGroup. All rights reserved.
//

#ifndef WKGeneralHeader_h
#define WKGeneralHeader_h
#ifdef DEBUG
#define NSLog(fmt, ...) NSLog((@"%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define NSLog(...)
#endif


///主窗口宽
#define WK_SCREEN_Width [UIScreen mainScreen].bounds.size.width

///主窗口高
#define WK_SCREEN_Height [UIScreen mainScreen].bounds.size.height

///随机小数
#define WK_ARC4RANDOM_float    ((double)arc4random() / 0x100000000)

///copy 视图
#define WK_VIEWCOPY(view) [NSKeyedUnarchiver unarchiveObjectWithData:[NSKeyedArchiver archivedDataWithRootObject:view]];
//------------------color----------------

///rgb颜色
#define WK_Color_RGB(r,g,b,a) [UIColor colorWithRed:(r)/255.0 green:(g)/255.0 blue:(b)/255.0 alpha:a]

///16进制颜色转rgb颜色
#define WK_Color_HEX(rgbValue) \
[UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16)) / 255.0 \
green:((float)((rgbValue & 0xFF00) >> 8)) / 255.0 \
blue:((float)(rgbValue & 0xFF)) / 255.0 alpha:1.0]

///随机颜色
#define WK_Color_RANDOM [UIColor colorWithRed:(arc4random_uniform(256))/255.0 green:(arc4random_uniform(256))/255.0 blue:(arc4random_uniform(256))/255.0 alpha:1]

//------------------typeof----------------
///弱引用
#define WK_TYPEWEAK(type)  __weak typeof(type) weak##type = type;

///强引用
#define WK_TYPESTRONG(type)  __strong typeof(type) type = weak##type;


//------------------path----------------
///拼接Document路径
#define WK_PATHDocument [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject]

#define WK_PATHCache [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) firstObject]

//------------------Thread----------------
///GCD代码只执行一次
#define WK_DISPATCH_ONCE(onceBlock) static dispatch_once_t onceToken; dispatch_once(&onceToken, onceBlock);

///在主线程上运行
#define WK_DISPATCH_MAIN(mainQueueBlock) dispatch_async(dispatch_get_main_queue(), mainQueueBlock);

///开启异步线程
#define WK_DISPATCH_GLOBAL_QUEUE_DEFAULT(globalQueueBlock) dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), globalQueueBlocl);

#endif /* WKGeneralHeader_h */
