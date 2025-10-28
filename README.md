### FpsMonitor
基于Android Graphics显示系统APIs开发的,监测刷新率FPS的演示小程序

#### 详细说明
参见文章：关注公众号 "Android元宇宙"，图形图像专栏获取更多内容


#### 分支
main ：默认分支，基于Android 14平台编译&开发     
android-14 ：基于Android 14平台编译&开发    
android-15 ：基于Android 15平台编译&开发    
android-16 ：基于Android 15平台编译&开发    


#### 使用方法
1. 下载代码放到android源码目录下
2. 执行mm编译获得可执行档 FpsMonitor
3. adb push FpsMonitor /data/local/tmp/
4. 执行 adb shell am stack list 获取要监测的应用的 taskId
5. adb shell /data/local/tmp/FpsMonitor -t taskId 运行程序
6. 输入‘q’ 或者 Ctrl+C 退出监测

![image](https://github.com/yrzroger/FpsMonitor/assets/18068017/3f43092e-5672-49d6-a2f4-09f911f15545)


#### 结果展示
在console log中打印出实时的FPS
![image](https://github.com/yrzroger/FpsMonitor/assets/18068017/916c013c-e739-4b40-8ec2-789d64a927af)


在屏幕左上角展示当前的屏幕的刷新率和图层渲染送显的帧率
![image](https://github.com/yrzroger/FpsMonitor/assets/18068017/acdd16bf-ac93-4e60-b584-bb9bf5dd6d99)
