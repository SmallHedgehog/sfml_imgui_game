# sfml_imgui_game
使用ImGui和sfml实现的一款带匹配和社交的五子棋游戏，目的是熟悉游戏服务器编程以及服务器和客户端之间的通信库，C++开发，服务器端配合Visual studio 2017，客户端配合Visual studio 2015，,项目中包含有相应的VS工程文件。服务器端是使用Windows socket编写， 客户端界面使用ImGui，窗口管理使用sfml。

## 相关介绍
> ImGui：Immediate Mode Graphical User Interafce, it is convenient for games or other frame-by-frame realtime 
> applications，可以看看相关的介绍 [Introduce ImGui](http://sol.gfxile.net/imgui/)，使用https://github.com/ocornut/imgui的实现。

> [SFML](https://www.sfml-dev.org/tutorials/2.4/)：这里使用的是SFML来管理客户端UI窗口，为了使用ImGui，需要将sfml与ImGui绑定，使用的是eliasdaler/imgui-sfml，该库> 介绍了怎样使用ImGui和SFML以及该怎样配置你的项目。

## Server
服务端介绍请看https://github.com/SmallHedgehog/sfml_imgui_game/blob/master/Server/README.md
## Client
客户端介绍请看https://github.com/SmallHedgehog/sfml_imgui_game/blob/master/Client/README.md

## 实现效果
* 匹配效果

![匹配效果](https://github.com/SmallHedgehog/sfml_imgui_game/blob/master/Res/match.png)

* 聊天效果

![聊天效果](https://github.com/SmallHedgehog/sfml_imgui_game/blob/master/Res/chat.png)

* 人机对弈效果

![人机对弈效果](https://github.com/SmallHedgehog/sfml_imgui_game/blob/master/Res/rj.png)

* 人人对弈效果

![人人对弈效果](https://github.com/SmallHedgehog/sfml_imgui_game/blob/master/Res/rr.png)
