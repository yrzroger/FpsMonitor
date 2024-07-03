### FpsMonitor
基于Android Graphics显示系统APIs开发的,监测刷新率FPS的演示小程序

#### 详细说明
参见文章：关注公众号 "Android元宇宙"，图形图像专栏获取更多内容
![图片](https://github.com/yrzroger/NativeSFDemo/assets/18068017/e4ddc7ce-cb94-4029-847c-cdabaa5f5dcd)


#### 分支
main ：基于Android 14平台开发 


#### 使用方法
1. 下载代码放到android源码目录下
2. 执行mm编译获得可执行档 FpsMonitor
3. adb push FpsMonitor /data/local/tmp/
4. 执行adb shell am stack list获取要监测的应用的 taskId
5. adb shell /data/local/tmp/FpsMonitor -t taskId 运行程序


#### 结果展示
