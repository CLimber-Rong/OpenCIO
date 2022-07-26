# ***OpenCIO***技术文档
* 仓库地址：https://github.com/CLimber-Rong/OpenCIO
## 什么是OpenCIO
<p id="SGR_CODE">OpenCIO，全称Open Console IO，是一个基于C++的，跨平台的，轻量级的控制台IO库，你可以利用它快速的实现控制台光标移动，输出彩色文本等功能，简化了巨大的工作量  
让我们来举一个简单的案例</p>

```C++
#include"opencio.cpp"
#include"stdio.h"
using namespace std;
int main()
{
    OpenCIO cio;    //实例化对象
    #ifdef __WIN32__
    cio.OpenANSIControl();  //如果是win32平台要开启虚拟终端
    #endif
    cio.SelectGraphicRendition(SGR_UnderLine);  //设置输出格式为下划线
    cio.SelectGraphicRendition(SGR_Blue);    //设置字体色为蓝色
    printf("Hello OpenCIO!");
    return 0;
}
```
以上代码会输出带有下划线的颜色为蓝色的：

<u style="color: blue;">Hello OpenCIO!</u>

***
## OpenCIO的基本结构
OpenCIO的结构可细分为：
* SCM(Select Cursor Move,光标移动指令)
* CLS(Clean Screen,清屏指令)
* SGR(Select Graphic Rendition,字符渲染指令)
* KEL(Keyboard Event Listen,键盘事件侦听 该模块于1.2.2.0新增)
***
## OpenCIO的环境搭建和初始化
### 1.环境搭建
OpenCIO的环境搭建极其简单，有两种方法：
1. 你只需要从仓库下载opencio.cpp这个文件，并把它放到你需要依赖opencio.cpp的源文件同目录即可。注意：以下的示例全部都是基于这种方法之上的
2. 你也可以把他放到标准库路径中，但是我们不建议这么做。  

* 仓库地址：https://github.com/CLimber-Rong/OpenCIO
### 2.初始化
首先，你需要用以下代码初始化一个对象：
```c++
OpenCIO cio;    //这里的cio是对象名，可以更改
```
其次，如果你的系统是Linux/Unix类，那么你无需任何初始化！但是如果你的系统是win32平台，那么你需要在正式使用之前调用OpenANSIControl()方法来开启虚拟终端以便初始化，他的原型是：
```C++
void OpenCIO::OpenANSIControl(void);
```
所以为了实现跨平台，你可以在你的代码中加入以下内容来实现初始化：
```C++
OpenCio cio;
#ifdef __WIN32__
cio.OpenANSIControl();  //如果是win32平台就初始化
#endif
```
但是请注意：该方法只能在win32平台中调用！
***
## OpenCIO接口详解
注意：以下方法都在OpenCIO类中
### 1.SCM方法集

|方法原型|功能|
|:-:|:-:|
|void CursorUp(unsigned row)|向上移动row行|
|void CursorDown(unsigned row)|向下移动row行|
|void CursorForward(unsigned column)|向右移动column列|
|void CursorBack(unsigned column)|向左移动column列|
|void CursorNextLine(unsigned row)|向下移动row行并将光标移至行首|
|void CursorPreciousLine(unsigned row)|向上移动row行并将光标移至行首|
|void CursorHorizontalAbsolute(int column)|将光标移至第column列|
|void CursorPosition(int row,int column)|将光标移至第row行，第column列|
### 2.CLS详解
CLS类一共有两个方法：

|方法原型|功能|
|:-:|:-:|
|void CleanScreen(CleanScreenMode mode)|按mode清除屏幕|
|void CleanLine(CleanLineMode mode)|按mode清除光标所在行|

这里的mode指的是清除模式，清除屏幕有以下模式：

|模式名|功能|
|:-:|:-:|
|CLSMode_FromCursorToEnd|从光标位置到屏幕结尾全部清除|
|CLSMode_FromCursorToTop|从光标位置到屏幕开头全部清除|
|CLSMode_AllScreen|清除整个屏幕|

而清除行有以下模式：

|模式名|功能|
|:-:|:-:|
|CLLMode_FromCursorToEnd|从光标位置到行尾全部清除|
|CLLMode_FromCursorToTop|从光标位置到行首全部清除|
|CLLMode_AllLine|清除一整行|

对于清屏模式的存储，你可以使用```CleanScreenMode```类型，对于清除行模式的存储，你可以使用```CleanLineMode```类型。  
下面将用代码举例来方便理解：  
1. 清屏示例

```C++
/*该代码会清除从整个屏幕*/
#include"opencio.cpp"
#include"stdio.h"

using namespace std;

int main()
{
    OpenCIO cio;        //实例化对象
    #ifdef __WIN32__
    cio.OpenANSIControl();  //如果是win32平台就开启虚拟终端初始化
    #endif
    printf("看好了，按下回车键键后这段内容将变身！\n");
    getchar();  //读取一个键
    CleanScreenMode mode = CLSMode_AllScreen;
    cio.CleanScreen(mode);
    /*
     * 这两行代码等效于cio.CleanScreen(CLSMode_AllScreen);
    */
    printf("变身成功！\n");
    return 0;
}
```

2. 清除行示例

```C++
#include"opencio.cpp"
#include"stdio.h"

using namespace std;

int main()
{
    OpenCIO cio;    //实例化
    #ifdef __WIN32__
    cio.OpenANSIControl();  //如果是win32平台就开启虚拟终端初始化
    #endif
    printf("小王：看我，按回车键我会消失一半!\n");
    getchar();
    cio.CursorPosition(1,10);  //切至第一行中间
    CleanLineMode mode = CLLMode_FromCursorToTop;
    cio.CleanLine(mode);
    return 0;
}
```

### 3.SGR详解
与SGR有关的只有一个方法：

|方法原型|功能|
|:-:|:-:|
|void SelectGraphicRendition(SGR_Code code)|做渲染|

其中SGR_Code是用来存储渲染类型，举个简单的例子：
```C++
#include"opencio.cpp"
#include"stdio.h"

using namespace std;

int main()
{
    OpenCIO cio;        //实例化对象
    #ifdef __WIN32__
    cio.OpenANSIControl();  //如果是win32平台就开启虚拟终端初始化
    #endif
    SGR_Code code = SGR_Blue;
    cio.SelectGraphicRendition(code);   //进行渲染
    printf("这段字是蓝色的");
    return 0;
}
```
当然，如果你想将背景色设为蓝色也完全没有问题，只需要调用```void SetBackgroundColor(SGR_Code* code);```方法即可，所以通过上面的代码可以修改为：
```C++
#include"opencio.cpp"
#include"stdio.h"

using namespace std;

int main()
{
    OpenCIO cio;        //实例化对象
    #ifdef __WIN32__
    cio.OpenANSIControl();  //如果是win32平台就开启虚拟终端初始化
    #endif
    SGR_Code code = SGR_Blue;
    cio.SetBackgroundColor(&code);      //从字体色改为背景色
    cio.SelectGraphicRendition(code);   //进行渲染
    printf("这段字的背景是蓝色的");
    return 0;
}
```

事实上，开头的那段代码也是一个很好的示例，点击[这里](#SGR_CODE)可以跳回开头  
那么接下来通过源代码来清楚的展示渲染模式分别有那些：
```C++
enum SelectGraphicRenditionMode{
	SGR_Strong=1,
	SGR_Italic=3,
	SGR_UnderLine=4,
	SGR_Flashing=5,
	SGR_Inverse=7,
	SGR_DeleteLine=9,
	SGR_DefaultFont=10,
	SGR_CloseItalic=23,
	SGR_CloseUnderLine,
	SGR_CloseFlashing,
	SGR_CloseInverse,
	SGR_CloseDeleteLine=29,
	SGR_NormalFont=22,
	SGR_Black=30,
	SGR_Red,
	SGR_Green,
	SGR_Yellow,
	SGR_Blue,
	SGR_Magenta,
	SGR_Cyan,
	SGR_White,
	SGR_Gray,
	SGR_DefalutForeColor=39,
	SGR_DefaultBackgroundColoe=49,
	SGR_UpLine=53,
	SGR_CloseUpLine=55,
	SGR_LightRed=90,
	SGR_LightGreen,
	SGR_LightYellow,
	SGR_LightBlue,
	SGR_LightMagenta,
	SGR_LightCyan,
	SGR_LightWhite
};
```
***
## ***OpenCIO 1.2.2.0*** 版本更新特性
### OpenCIO基本结构新增KEL模块
#### 1. 什么是KEL？
KEL，全称Keyboard Event Listen，键盘事件侦听，是1.2.2.0版本新增的模块，OpenCIO解决了键盘事件的不兼容性，通过整合实现了跨平台，让我们来举个例子：

```C++
#ifdef __WIN32__
#include<windows.h>
#else
#include<unistd.h>
#endif
#include"opencio.cpp"
#include"stdio.h"

using namespace std;

int main()
{
    /*此代码示例将每秒检测一次按键并输出*/
    OpenCIO cio;    //实例化对象
    #ifdef __WIN32__
    cio.OpenANSIControl();      //如果是win32平台就开启虚拟终端实现初始化
    #endif
    while(true){
        bool flag = cio.isKeyEvent();    //获取键盘事件
        if(flag){
            //flag=true说明键盘被按下
            char c = cio.GetKeyEvent(); //获取按下的键盘ASCII码
            if(c==0x1B) return 0;   //如果按下ESC键就推出，0x1B是ESC键的ASCII码
            cout<<"一秒过去了，你按下了键盘，按下的键是"<<c<<endl;
        }else{
            cout<<"一秒过去了，你没有按下键盘"<<endl;
            //flag=false说明键盘没有被按下
        }
        //接着进入休眠，但是休眠期间按下的按键依旧会被记录
        #ifdef __WIN32__
        Sleep(1000);
        #else
        sleep(1);
        #endif
    }
    return 0;
}
```

在此示例中，我们看到了本次更新的两个新接口，让我们来看一看：

|方法原型|功能|
|:-:|:-:|
|bool isKeyEvent(void)|判断缓冲区内是否有键按下，非阻塞|
|char GetKeyEvent(void)|向缓冲区读取一个按键，阻塞|

其中，```isKeyEvent()```方法是非阻塞的，即不等待用户操作，立即返回结果，而```GetKeyEvent()```方法是阻塞的，即如果用户还没有按下键，就会一直等待直到用户按下任意键后返回ASCII码。  
所以我们经常配合这两个接口使用，即用```isKeyEvent```检测是否有按键按下，如果有，就用```GetKeyEvent```读取这个按键，否则就之间略过，去做下一件事情。
***
### 以上即为OpenCIO技术文档的全部，如有问题，欢迎发邮件至woshiquxiangrong@outlook.com，谢谢！
***
# ***<center>OpenCIO</center>***
