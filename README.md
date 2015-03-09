反向weixinxxx.apk
------------------

***请不要用于商业用途，版权归腾讯所有！***


## 依赖 for ArchLinux

```
yaourt -S apktool
```

apktool是反向apk的工具。


```
wget android-ndk32-r10-linux-x86_64.tar.bz2
```

android-ndk其中的工具链，至少需要***arm-linux-androideabi-objdump***


## 反向apk

```
apktool d weixin610android540.apk
```

可以通过修改smali/com/tencent/mm/ui/来just for fun ;-)

而重要的逻辑层全部被封装成库lib/armeabi

```
arm-linux-androideabi-objdump -d lib/armeabi/libwechatnetwork.so > libwechatnetwork.txt
```

可以发现例如***Java_com_tencent_mm_network_Java2C_getNetworkServerIp***函数入口
借助了java2c把Java项目“翻译”成了C/C++生成动态链接库，以防止反向工程？


## 将 arm 二进制转换成 x86 架构

[Intel 二进制翻译] (http://www.anandtech.com/show/5770/lava-xolo-x900-review-the-first-intel-medfield-phone/3)
