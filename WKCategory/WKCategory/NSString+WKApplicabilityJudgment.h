//
//  NSString+WKApplicabilityJudgment.h
//  WKCategory
//
//  Created by admin on 2017/7/14.
//  Copyright © 2017年 juyuanGroup. All rights reserved.
//
/*
 功能描述：字符串适用性判断
 --修改人：
 修改时间：
 修改描述：
 */
;
#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger,WKApplicabilityJudgmentType){
    WKApplicabilityJudgmentTypeNumberCharacters             = 1,
    WKApplicabilityJudgmentTypeOrdinaryEnglishCharacters    = 1 << 1,
    WKApplicabilityJudgmentTypeChineseCharacters            = 1 << 2,
    WKApplicabilityJudgmentTypeMobile               = 1 << 3,
    WKApplicabilityJudgmentTypeIDCard               = 1 << 4,
    WKApplicabilityJudgmentTypeBankCard             = 1 << 5,

};
@interface NSString (WKApplicabilityJudgment)

/**
 是否符合正则表达式
 @param predStr 正则公式
 */
- (BOOL)conformToPredicateString:(NSString*)predStr;


- (BOOL)conformToApplicabilityJudgmentType:(NSInteger)type;

@end
