> WKCategory
不常应用的方法

[TOC]
# 1、禁止手机睡眠
```
 [UIApplication sharedApplication].idleTimerDisabled = YES;
```
# 2、跳进app权限设置
```
// 跳进app设置
            if (UIApplicationOpenSettingsURLString != NULL) {
                UIApplication *application = [UIApplication sharedApplication];
                NSURL *URL = [NSURL URLWithString:UIApplicationOpenSettingsURLString];
                if ([application respondsToSelector:@selector(openURL:options:completionHandler:)]) {
                    [application openURL:URL options:@{}
                       completionHandler:nil];
                } else {
                    [application openURL:URL];
                }
            }
```
# 3、给一个view截图
```
UIGraphicsBeginImageContextWithOptions(view.bounds.size, YES, 0.0);
    [view.layer renderInContext:UIGraphicsGetCurrentContext()];
    UIImage *img = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
```
# 4、控制屏幕旋转，在控制器中写
```
/** 是否支持自动转屏 */
- (BOOL)shouldAutorotate {
    return YES;
}

/** 支持哪些屏幕方向 */
- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight;
}

/** 默认的屏幕方向（当前ViewController必须是通过模态出来的UIViewController（模态带导航的无效）方式展现出来的，才会调用这个方法） */
- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation {
    return UIInterfaceOrientationLandscapeLeft | UIInterfaceOrientationLandscapeRight;
}
```
# 5、获取app缓存大小
```
- (CGFloat)getCachSize {

    NSUInteger imageCacheSize = [[SDImageCache sharedImageCache] getSize];
    //获取自定义缓存大小
    //用枚举器遍历 一个文件夹的内容
    //1.获取 文件夹枚举器
    NSString *myCachePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Library/Caches"];
    NSDirectoryEnumerator *enumerator = [[NSFileManager defaultManager] enumeratorAtPath:myCachePath];
    __block NSUInteger count = 0;
    //2.遍历
    for (NSString *fileName in enumerator) {
        NSString *path = [myCachePath stringByAppendingPathComponent:fileName];
        NSDictionary *fileDict = [[NSFileManager defaultManager] attributesOfItemAtPath:path error:nil];
        count += fileDict.fileSize;//自定义所有缓存大小
    }
    // 得到是字节  转化为M
    CGFloat totalSize = ((CGFloat)imageCacheSize+count)/1024/1024;
    return totalSize;
}
```
# 6、清理app缓存
```
- (void)handleClearView {
    //删除两部分
    //1.删除 sd 图片缓存
    //先清除内存中的图片缓存
    [[SDImageCache sharedImageCache] clearMemory];
    //清除磁盘的缓存
    [[SDImageCache sharedImageCache] clearDisk];
    //2.删除自己缓存
    NSString *myCachePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Library/Caches"];
    [[NSFileManager defaultManager] removeItemAtPath:myCachePath error:nil];
}
```

# 7、模型转字典
```
static NSSet *classes;

- (NSMutableDictionary *)getParameterDictionary {

    NSMutableDictionary *dict = [NSMutableDictionary dictionary];

    Class c = self.class;

    while (c) {
        unsigned count;
        objc_property_t *properties = class_copyPropertyList([c class], &count);

        for (int i = 0; i < count; i++) {
            NSString *key = [NSString stringWithUTF8String:property_getName(properties[i])];
            dict[key] = [self valueForKey:key];
        }
        free(properties);

        // 获得父类
        c = class_getSuperclass(c);

        if ([self isClassFromFoundation:c]) break;
    }
    return dict;
}

- (BOOL)isClassFromFoundation:(Class)c
{
    if (c == [NSObject class] || c == [NSManagedObject class]) return YES;

    __block BOOL result = NO;
    [[self foundationClasses] enumerateObjectsUsingBlock:^(Class foundationClass, BOOL *stop) {
        if ([c isSubclassOfClass:foundationClass]) {
            result = YES;
            *stop = YES;
        }
    }];
    return result;
}

- (NSSet *)foundationClasses
{
    if (classes == nil) {
        // 集合中没有NSObject，因为几乎所有的类都是继承自NSObject，具体是不是NSObject需要特殊判断
        classes = [NSSet setWithObjects:
                              [NSURL class],
                              [NSDate class],
                              [NSValue class],
                              [NSData class],
                              [NSError class],
                              [NSArray class],
                              [NSDictionary class],
                              [NSString class],
                              [NSAttributedString class], nil];
    }
    return classes;
}
```

# 8、交换两个方法实现
```
Class aClass = [self class]; 

        SEL originalSelector = @selector(viewWillAppear:); 
        SEL swizzledSelector = @selector(xxx_viewWillAppear:); 

        Method originalMethod = class_getInstanceMethod(aClass, originalSelector); 
        Method swizzledMethod = class_getInstanceMethod(aClass, swizzledSelector); 

        BOOL didAddMethod = 
            class_addMethod(aClass, 
                originalSelector, 
                method_getImplementation(swizzledMethod), 
                method_getTypeEncoding(swizzledMethod)); 

        if (didAddMethod) { 
            class_replaceMethod(aClass, 
                swizzledSelector, 
                method_getImplementation(originalMethod), 
                method_getTypeEncoding(originalMethod)); 
        } else { 
            method_exchangeImplementations(originalMethod, swizzledMethod); 
        }
```

# 9、几个常用权限判断
```
if ([CLLocationManager authorizationStatus] ==kCLAuthorizationStatusDenied) {
        NSLog(@"没有定位权限");
    }
    AVAuthorizationStatus statusVideo = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    if (statusVideo == AVAuthorizationStatusDenied) {
        NSLog(@"没有摄像头权限");
    }
    //是否有麦克风权限
    AVAuthorizationStatus statusAudio = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio];
    if (statusAudio == AVAuthorizationStatusDenied) {
        NSLog(@"没有录音权限");
    }
    [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
        if (status == PHAuthorizationStatusDenied) {
            NSLog(@"没有相册权限");
        }
    }];
```

# 10、长按复制功能
```
- (void)viewDidLoad
{
    [self.view addGestureRecognizer:[[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(pasteBoard:)]];
}
- (void)pasteBoard:(UILongPressGestureRecognizer *)longPress {
    if (longPress.state == UIGestureRecognizerStateBegan) {
        UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
        pasteboard.string = @"需要复制的文本";
    }
}
```

# 11、获取手机和app信息
```
NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];  
 CFShow(infoDictionary);  
// app名称  
 NSString *app_Name = [infoDictionary objectForKey:@"CFBundleDisplayName"];  
 // app版本  
 NSString *app_Version = [infoDictionary objectForKey:@"CFBundleShortVersionString"];  
 // app build版本  
 NSString *app_build = [infoDictionary objectForKey:@"CFBundleVersion"];  



    //手机序列号  
    NSString* identifierNumber = [[UIDevice currentDevice] uniqueIdentifier];  
    NSLog(@"手机序列号: %@",identifierNumber);  
    //手机别名： 用户定义的名称  
    NSString* userPhoneName = [[UIDevice currentDevice] name];  
    NSLog(@"手机别名: %@", userPhoneName);  
    //设备名称  
    NSString* deviceName = [[UIDevice currentDevice] systemName];  
    NSLog(@"设备名称: %@",deviceName );  
    //手机系统版本  
    NSString* phoneVersion = [[UIDevice currentDevice] systemVersion];  
    NSLog(@"手机系统版本: %@", phoneVersion);  
    //手机型号  
    NSString* phoneModel = [[UIDevice currentDevice] model];  
    NSLog(@"手机型号: %@",phoneModel );  
    //地方型号  （国际化区域名称）  
    NSString* localPhoneModel = [[UIDevice currentDevice] localizedModel];  
    NSLog(@"国际化区域名称: %@",localPhoneModel );  

    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];  
    // 当前应用名称  
    NSString *appCurName = [infoDictionary objectForKey:@"CFBundleDisplayName"];  
    NSLog(@"当前应用名称：%@",appCurName);  
    // 当前应用软件版本  比如：1.0.1  
    NSString *appCurVersion = [infoDictionary objectForKey:@"CFBundleShortVersionString"];  
    NSLog(@"当前应用软件版本:%@",appCurVersion);  
    // 当前应用版本号码   int类型  
    NSString *appCurVersionNum = [infoDictionary objectForKey:@"CFBundleVersion"];  
    NSLog(@"当前应用版本号码：%@",appCurVersionNum);
```

# 12、获取一个类的所有属性
```
id LenderClass = objc_getClass("Lender");
unsigned int outCount, i;
objc_property_t *properties = class_copyPropertyList(LenderClass, &outCount);
for (i = 0; i < outCount; i++) {
    objc_property_t property = properties[i];
    fprintf(stdout, "%s %s\n", property_getName(property), property_getAttributes(property));
}
```

# 13、删除NSUserDefaults所有记录

```
//方法一
  NSString *appDomain = [[NSBundle mainBundle] bundleIdentifier];
 [[NSUserDefaults standardUserDefaults] removePersistentDomainForName:appDomain];   
 //方法二  
- (void)resetDefaults {   
  NSUserDefaults * defs = [NSUserDefaults standardUserDefaults];
     NSDictionary * dict = [defs dictionaryRepresentation];
     for (id key in dict) {
          [defs removeObjectForKey:key];
     }
      [defs synchronize];
 }
// 方法三
[[NSUserDefaults standardUserDefaults] setPersistentDomain:[NSDictionary dictionary] forName:[[NSBundle mainBundle] bundleIdentifier]];
```

# 14、禁用系统滑动返回功能
```
- (void)viewDidAppear:(BOOL)animated
{
     [super viewDidAppear:animated];
if ([self.navigationController respondsToSelector:@selector(interactivePopGestureRecognizer)]) {self.navigationController.interactivePopGestureRecognizer.delegate = self;
    }
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    if ([self.navigationController respondsToSelector:@selector(interactivePopGestureRecognizer)]) {self.navigationController.interactivePopGestureRecognizer.delegate = nil;
    }
}
- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer
{
     return NO;
}
```

# 15、UILabel设置文字描边
```
子类化UILabel，重写drawTextInRect方法
- (void)drawTextInRect:(CGRect)rect
{
    CGContextRef c = UIGraphicsGetCurrentContext();
    // 设置描边宽度
    CGContextSetLineWidth(c, 1);
    CGContextSetLineJoin(c, kCGLineJoinRound);
    CGContextSetTextDrawingMode(c, kCGTextStroke);
    // 描边颜色
    self.textColor = [UIColor redColor];
    [super drawTextInRect:rect];
    // 文本颜色
    self.textColor = [UIColor yellowColor];
    CGContextSetTextDrawingMode(c, kCGTextFill);
    [super drawTextInRect:rect];
}
```

# 16、让手机震动一下
```
倒入框架
#import <AudioToolbox/AudioToolbox.h>
AudioServicesPlayAlertSound(kSystemSoundID_Vibrate);
或者
AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
```

# 17、摇一摇功能
```
1、打开摇一摇功能
    [UIApplication sharedApplication].applicationSupportsShakeToEdit = YES;
2、让需要摇动的控制器成为第一响应者
[self becomeFirstResponder];
3、实现以下方法

// 开始摇动
- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event
// 取消摇动
- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event
// 摇动结束
- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event
```

# 18、设置屏幕方向
```
NSNumber *orientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
[[UIDevice currentDevice] setValue:orientationTarget forKey:@"orientation"];
[UIViewController attemptRotationToDeviceOrientation];
```

# 19、tableViewCell分割线顶到头
```
- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath {
    [cell setSeparatorInset:UIEdgeInsetsZero];
    [cell setLayoutMargins:UIEdgeInsetsZero];
    cell.preservesSuperviewLayoutMargins = NO;
}

- (void)viewDidLayoutSubviews {
    [self.tableView setSeparatorInset:UIEdgeInsetsZero];
    [self.tableView setLayoutMargins:UIEdgeInsetsZero];
}
```

# 20、在状态栏增加网络请求的菊花，类似safari加载网页的时候状态栏菊花
```
[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
```

# 21、获取屏幕方向
```
UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;

if(orientation == 0) //Default orientation 
    //默认
else if(orientation == UIInterfaceOrientationPortrait)
    //竖屏
else if(orientation == UIInterfaceOrientationLandscapeLeft)
    // 左横屏
else if(orientation == UIInterfaceOrientationLandscapeRight)
    //右横屏
```

# 22、地图上两个点之间的实际距离
```
// 需要导入#import <CoreLocation/CoreLocation.h>
CLLocation *locA = [[CLLocation alloc] initWithLatitude:34 longitude:113];
    CLLocation *locB = [[CLLocation alloc] initWithLatitude:31.05 longitude:121.76];
// CLLocationDistance求出的单位为米
    CLLocationDistance distance = [locA distanceFromLocation:locB];
```

# 23、在应用中打开设置的某个界面
```
// 打开设置->通用
[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"prefs:root=General"]];

// 以下是设置其他界面
prefs:root=General&path=About
prefs:root=General&path=ACCESSIBILITY
prefs:root=AIRPLANE_MODE
prefs:root=General&path=AUTOLOCK
prefs:root=General&path=USAGE/CELLULAR_USAGE
prefs:root=Brightness
prefs:root=Bluetooth
prefs:root=General&path=DATE_AND_TIME
prefs:root=FACETIME
prefs:root=General
prefs:root=General&path=Keyboard
prefs:root=CASTLE
prefs:root=CASTLE&path=STORAGE_AND_BACKUP
prefs:root=General&path=INTERNATIONAL
prefs:root=LOCATION_SERVICES
prefs:root=ACCOUNT_SETTINGS
prefs:root=MUSIC
prefs:root=MUSIC&path=EQ
prefs:root=MUSIC&path=VolumeLimit
prefs:root=General&path=Network
prefs:root=NIKE_PLUS_IPOD
prefs:root=NOTES
prefs:root=NOTIFICATIONS_ID
prefs:root=Phone
prefs:root=Photos
prefs:root=General&path=ManagedConfigurationList
prefs:root=General&path=Reset
prefs:root=Sounds&path=Ringtone
prefs:root=Safari
prefs:root=General&path=Assistant
prefs:root=Sounds
prefs:root=General&path=SOFTWARE_UPDATE_LINK
prefs:root=STORE
prefs:root=TWITTER
prefs:root=FACEBOOK
prefs:root=General&path=USAGE prefs:root=VIDEO
prefs:root=General&path=Network/VPN
prefs:root=Wallpaper
prefs:root=WIFI
prefs:root=INTERNET_TETHERING
prefs:root=Phone&path=Blocked
prefs:root=DO_NOT_DISTURB
```

# 24、通知监听APP生命周期
```
UIApplicationDidEnterBackgroundNotification 应用程序进入后台
UIApplicationWillEnterForegroundNotification 应用程序将要进入前台
UIApplicationDidFinishLaunchingNotification 应用程序完成启动
UIApplicationDidFinishLaunchingNotification 应用程序由挂起变的活跃
UIApplicationWillResignActiveNotification 应用程序挂起(有电话进来或者锁屏)
UIApplicationDidReceiveMemoryWarningNotification 应用程序收到内存警告
UIApplicationDidReceiveMemoryWarningNotification 应用程序终止(后台杀死、手机关机等)
UIApplicationSignificantTimeChangeNotification 当有重大时间改变(凌晨0点，设备时间被修改，时区改变等)
UIApplicationWillChangeStatusBarOrientationNotification 设备方向将要改变
UIApplicationDidChangeStatusBarOrientationNotification 设备方向改变
UIApplicationWillChangeStatusBarFrameNotification 设备状态栏frame将要改变
UIApplicationDidChangeStatusBarFrameNotification 设备状态栏frame改变
UIApplicationBackgroundRefreshStatusDidChangeNotification 应用程序在后台下载内容的状态发生变化
UIApplicationProtectedDataWillBecomeUnavailable 本地受保护的文件被锁定,无法访问
UIApplicationProtectedDataWillBecomeUnavailable 本地受保护的文件可用了
```

# 25、触摸事件类型
```
UIControlEventTouchCancel 取消控件当前触发的事件
UIControlEventTouchDown 点按下去的事件
UIControlEventTouchDownRepeat 重复的触动事件
UIControlEventTouchDragEnter 手指被拖动到控件的边界的事件
UIControlEventTouchDragExit 一个手指从控件内拖到外界的事件
UIControlEventTouchDragInside 手指在控件的边界内拖动的事件
UIControlEventTouchDragOutside 手指在控件边界之外被拖动的事件
UIControlEventTouchUpInside 手指处于控制范围内的触摸事件
UIControlEventTouchUpOutside 手指超出控制范围的控制中的触摸事件
```

# 26、根据经纬度获取城市等信息
```
// 创建经纬度
    CLLocation *location = [[CLLocation alloc] initWithLatitude:latitude longitude:longitude];
    //创建一个译码器
    CLGeocoder *cLGeocoder = [[CLGeocoder alloc] init];
    [cLGeocoder reverseGeocodeLocation:userLocation completionHandler:^(NSArray *placemarks, NSError *error) {
        CLPlacemark *place = [placemarks objectAtIndex:0];
        // 位置名
    　　NSLog(@"name,%@",place.name);
    　　// 街道
    　　NSLog(@"thoroughfare,%@",place.thoroughfare);
    　　// 子街道
    　　NSLog(@"subThoroughfare,%@",place.subThoroughfare);
    　　// 市
    　　NSLog(@"locality,%@",place.locality);
    　　// 区
    　　NSLog(@"subLocality,%@",place.subLocality); 
    　　// 国家
    　　NSLog(@"country,%@",place.country);
        }
    }];

/*  CLPlacemark中属性含义
name                    地名

thoroughfare            街道

subThoroughfare        街道相关信息，例如门牌等

locality                城市

subLocality            城市相关信息，例如标志性建筑

administrativeArea      直辖市

subAdministrativeArea  其他行政区域信息（自治区等）

postalCode              邮编

ISOcountryCode          国家编码

country                国家

inlandWater            水源，湖泊

ocean                  海洋

areasOfInterest        关联的或利益相关的地标
*/
```

# 27、某个界面多个事件同时响应引起的问题(比如，两个button同时按push到新界面，两个都会响应，可能导致push重叠)
```
// UIView有个属性叫做exclusiveTouch，设置为YES后，其响应事件会和其他view互斥(有其他view事件响应的时候点击它不起作用)
view.exclusiveTouch = YES;

// 一个一个设置太麻烦了，可以全局设置
[[UIView appearance] setExclusiveTouch:YES];

// 或者只设置button
[[UIButton appearance] setExclusiveTouch:YES];
```

# 28、启动页显示延时
```
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
//  延时10s
    sleep(10);
    return YES;
}
```

# 29、获取手机RAM容量
```
// 需要导入#import <mach/mach.h>
mach_port_t host_port;
    mach_msg_type_number_t host_size;
    vm_size_t pagesize;

    host_port = mach_host_self();
    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    host_page_size(host_port, &pagesize);

    vm_statistics_data_t vm_stat;

    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS) {
        NSLog(@"Failed to fetch vm statistics");
    }

    /* Stats in bytes */
    natural_t mem_used = (vm_stat.active_count +
                          vm_stat.inactive_count +
                          vm_stat.wire_count) * pagesize;
    natural_t mem_free = vm_stat.free_count * pagesize;
    natural_t mem_total = mem_used + mem_free;
    NSLog(@"已用: %u 可用: %u 总共: %u", mem_used, mem_free, mem_total);
```

# 30、获取当前应用CUP用量
```
float cpu_usage()
{
    kern_return_t kr;
    task_info_data_t tinfo;
    mach_msg_type_number_t task_info_count;

    task_info_count = TASK_INFO_MAX;
    kr = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)tinfo, &task_info_count);
    if (kr != KERN_SUCCESS) {
        return -1;
    }

    task_basic_info_t      basic_info;
    thread_array_t         thread_list;
    mach_msg_type_number_t thread_count;

    thread_info_data_t     thinfo;
    mach_msg_type_number_t thread_info_count;

    thread_basic_info_t basic_info_th;
    uint32_t stat_thread = 0; // Mach threads

    basic_info = (task_basic_info_t)tinfo;

    // get threads in the task
    kr = task_threads(mach_task_self(), &thread_list, &thread_count);
    if (kr != KERN_SUCCESS) {
        return -1;
    }
    if (thread_count > 0)
        stat_thread += thread_count;

    long tot_sec = 0;
    long tot_usec = 0;
    float tot_cpu = 0;
    int j;

    for (j = 0; j < (int)thread_count; j++)
    {
        thread_info_count = THREAD_INFO_MAX;
        kr = thread_info(thread_list[j], THREAD_BASIC_INFO,
                         (thread_info_t)thinfo, &thread_info_count);
        if (kr != KERN_SUCCESS) {
            return -1;
        }

        basic_info_th = (thread_basic_info_t)thinfo;

        if (!(basic_info_th->flags & TH_FLAGS_IDLE)) {
            tot_sec = tot_sec + basic_info_th->user_time.seconds + basic_info_th->system_time.seconds;
            tot_usec = tot_usec + basic_info_th->user_time.microseconds + basic_info_th->system_time.microseconds;
            tot_cpu = tot_cpu + basic_info_th->cpu_usage / (float)TH_USAGE_SCALE * 100.0;
        }

    } // for each thread

    kr = vm_deallocate(mach_task_self(), (vm_offset_t)thread_list, thread_count * sizeof(thread_t));
    assert(kr == KERN_SUCCESS);

    return tot_cpu;
}
```

# 31、给一个view截图
```
UIGraphicsBeginImageContextWithOptions(view.bounds.size, YES, 0.0);
[view.layer renderInContext:UIGraphicsGetCurrentContext()];
UIImage *img = UIGraphicsGetImageFromCurrentImageContext();
UIGraphicsEndImageContext();
```


