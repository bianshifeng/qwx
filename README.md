qwx
----

WeChat Qt frontend based on [webwx protocol] (https://github.com/xiangzhai/qwx/blob/master/doc/protocol.md) 
微信Qt前端，使用[网页微信协议] (https://github.com/xiangzhai/qwx/blob/master/doc/protocol.md)

***images and protocol Copyright (C) by [Tencent] (http://weixin.qq.com/)*** 
***图片、协议版权归[腾讯] (http://weixin.qq.com/) 所有！***

## Rock for wayland 

[QtWayland 5.4 with QtCompositor] (https://github.com/papyros/papyros-shell#dependencies)


## Build && Install 编译、运行

### Build for Ubuntu 15.04+

```
sudo apt-get install qtdeclarative5-dev qt5-default qt5-qmake qml-module-qtquick-controls qtdeclarative5-controls-plugin

mkdir build
cd build
qtchooser -run-tool=qmake -qt=qt5 ..
make
./qwx

```

## i18n

```
lupdate src/*.cpp qml/*.qml -ts translations/qwx_zh_CN.ts
lrelease translations/qwx_zh_CN.ts
```
