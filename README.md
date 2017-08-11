> WKCategory
常用的类别

如果你想要将这个工具导入你的项目
请使用

`pod 'WKCategory', :git=> 'https://github.com/Blanbok/WKCategory'`

 如果你想判断一串字符串是否符合某个规范，请`#import "NSString+WKApplicabilityJudgment.h"`
 然后调用您想要判断的类型进行判断
 ```objective-c 
 typedef NS_ENUM(NSInteger,WKApplicabilityJudgmentType){
    ///数字
    WKApplicabilityJudgmentTypeNumberCharacters             = 1,
    ///英文
    WKApplicabilityJudgmentTypeOrdinaryEnglishCharacters    = 1 << 1,
    ///中文
    WKApplicabilityJudgmentTypeChineseCharacters            = 1 << 2,
    ///电话
    WKApplicabilityJudgmentTypeMobile               = 1 << 3,
    ///身份证
    WKApplicabilityJudgmentTypeIDCard               = 1 << 4,
    ///银行卡
    WKApplicabilityJudgmentTypeBankCard             = 1 << 5,
    ///邮箱
    WKApplicabilityJudgmentTypeEmail             = 1 << 6,

};
```



```
/**
 符合选定标准
 
 @param type 类型
 */
- (BOOL)WK_conformToApplicabilityJudgmentType:(WKApplicabilityJudgmentType)type;

/**
 保留小数位数几位

 @param digit 位数
 @return 是否符合标准
 */
- (BOOL)WK_belongsToDecimalDigits:(int)digit;

 ```
 
补充内容
[不常应用的方法](./METHOD.md)
