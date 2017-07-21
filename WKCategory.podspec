#
#  Be sure to run `pod spec lint WKCategory.podspec' to ensure this is a
#  valid spec and to remove all comments including this before submitting the spec.
#
#  To learn more about Podspec attributes see http://docs.cocoapods.org/specification.html
#  To see working Podspecs in the CocoaPods repo see https://github.com/CocoaPods/Specs/
#

Pod::Spec.new do |s|

  s.name         = "WKCategory"
  s.version      = "0.0.1"
  s.summary      = "A short description of WKCategory."
  s.homepage     = "https://github.com/Blanbok/WKCategory"
  s.license      = "MIT"
  s.author             = { "Blanbok" => "a1653913692@icloud.com" }
  s.source       = { :git => "https://github.com/Blanbok/WKCategory.git", :tag => "#{s.version}" }
  s.source_files  = "./wkDataManger/WKCategory/*"
  s.requires_arc = true
  s.ios.deployment_target = "8.0"
  s.platform     = :ios

end
