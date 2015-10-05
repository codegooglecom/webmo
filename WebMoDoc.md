# 0. 文档目录 #


# 1. 项目简介 #

---

## 1.1 缘起 ##
本项目源于研一选修的[XML课程](http://cs.sysu.edu.cn/~lcw/)的大作业。本来是想运用所学的Dom编程知识做一个股票网页关键内容抽取的应用，于是我用python+Dom很快实现了想要的功能。在实现的过程中发现，其实类似的应用很多人都在做，譬如抽取网上的天气信息、抽取网上的优惠票价信息、抽取最新的网络视频连接等等。

然而，网络的信息是海量的，每个人感兴趣的内容也不尽相同。虽然一名伟大的程序员可以根据自己的需要去写程序抓取网上的内容，但普通的用户要获取自己想要的信息，只能靠运气找别人实现的程序。譬如，我实现了一个股票信息抽取器，可以查看个股的当前股价，假如某个用户不满足这些，还想查看个股的各种更详细的指数，他是没办法修改程序达到目的的——程序从编译那一刻就决定了用户能获取哪些内容。这种硬编码的方式显然满足不了多样的用户需求。

---

## 1.2 思考 ##
观察发现，各种网页内容抽取的程序虽然实现方法不一（有的用html+javascript，有的用C#+.NET XML，有的用VC+MSXML，有的用java+JAXP），但本质上都是 **定时解析某个网页并提取其中的某些包含动态内容的Element** 。

既然这类应用都有这种共性，何不深入做下去，实现一个更为 **通用的** 程序呢？我希望我的程序不只一个股票查看器，而是能实时获取天气、票价、新闻、博客、通知、成绩等任何网络内容的通用程序。我希望我的程序不只是按我的意愿去抽取哪部分的内容，而是可以让用户去决定——用户想抽取什么，他就能获得什么样的抽取器。

另一方面，为了照顾更多的用户，使得程序的使用门槛尽可能低，我还希望我的程序允许用户方便简单地定义他们想要的内容——总不能让一个普通的用户自己去修改程序的xml配置文档。因此，有必要在这个“通用的”程序上加入直观简单的交互操作，让用户点几下鼠标，就能得到一个为自己量身订做的网络信息抽取器。

---

## 1.3 目标 ##
总结上述思考，我想实现的是一个能 **通过简单操作为用户提供量身定制的网页内容抽取器** 的程序。该程序不再是一个简单的抽取器，而是一个能 **生成抽取器** 的程序。具体来说，这个程序应具备以下的新特性：
  * **通用性**——只要是网页上的内容，无论内容是什么类型，无论是静态的还是动态的，都能提取
  * **可配置性**——用户可以自定义需要提取的信息，而不是传统的硬编码方式
  * **交互简单**——用户容易上手，只需要直观的鼠标操作，即可达到目的
  * **可扩展性**——具备一定技能的用户可
  * **适合广泛的用户**——程序可在各种操作系统平台下运行

---

## 1.4 用到的XML课程知识 ##
  * **HTML/XHTML** ——HTML为本程序的交互对象与处理解析对象，生成的抽取器为XHTML文档
  * **层叠样单CSS** ——用于描述抽取器的外观
  * **Javascript** ——生成的抽取器主要由Javascript实现
  * **Ajax** ——生成的抽取器使用Ajax实现无刷新数据定时抽取
  * **DOM编程** ——用于实现监测显示用户选取的网页元素、解析选取元素的路径以及所生成的javascript抽取功能的实现
  * **XPath** ——在运用jQuery时，使用了类XPath的选择器。

---

# 2 设计思想 #

---

## 2.1 流程设计 ##
程序执行时，首先由用户前往某个网页，并对该网页做交互操作从而选取目标内容，然后由程序解析这些内容，最终生成一个满足用户需要的网页内容实时抽取器。下图示出程序的主要流程：

![https://webmo.googlecode.com/svn/wiki/5.jpg](https://webmo.googlecode.com/svn/wiki/5.jpg)

### 2.1.1 为什么设计为交互式选取？ ###
因为本程序面向广泛的用户，其中可能大部分不懂得怎么去写配置文档；而网络内容千变万化，结构也比较复杂，难以用简单的配置对话框去获取用户的配置。因此，采用交互的方式更为方便、直观。具体的用户交互设计思想将在2.2给出。
### 2.1.2 为什么要生成抽取器，而不直接提取数据？ ###
在用户定制内容之前，我们无从知道需要抽取什么信息，因此无法用硬编码的方式去实现这么一个抽取器。因此本程序本身不提供面向用户的抽取信息功能，而是动态生成一个抽取器去满足用户的需求。具体的抽取器设计思想将在2.3给出。

---

## 2.2 交互设计 ##
### 2.2.1 基本的网页浏览 ###
为了最大限度地增强用户体验，让用户更方便地自定义自己感兴趣的内容，本项目将提供一个类似浏览器的界面：

![https://webmo.googlecode.com/svn/wiki/1.jpg](https://webmo.googlecode.com/svn/wiki/1.jpg)

用户可自行输入网址，浏览目标网页。程序提供网页浏览的基本功能——后退、前进、刷新和停止加载。

### 2.2.2 网页元素选取 ###
基于网页浏览的方式，用户可直观地在网页上对感兴趣的区域右击鼠标进行选取。

![https://webmo.googlecode.com/svn/wiki/3.jpg](https://webmo.googlecode.com/svn/wiki/3.jpg)

如上图所示，当用户右击鼠标后，页面上相应的区域就会斜纹高亮显示。如果需要对当前的区域进行调整，还可按 **F6** 扩大选取的区域：

![https://webmo.googlecode.com/svn/wiki/4.jpg](https://webmo.googlecode.com/svn/wiki/4.jpg)

或按 **F7** 缩小选取的区域：

![https://webmo.googlecode.com/svn/wiki/2.jpg](https://webmo.googlecode.com/svn/wiki/2.jpg)


---

## 2.3 抽取器设计 ##
抽取器是本程序的输出结果。这是一个类似于传统网页信息提取软件的程序，不同点是此抽取器的抽取内容完全由用户定制。
### 2.3.1 抽取器的可选实现方式 ###
目前抽取器有非常多的实现方式，如前面提到的html+javascript、C#+.NET XML、VC+MSXML、java+JAXP等等。但基于以下原因，本程序生成的抽取器只考虑使用 **脚本语言** ：
  1. 网页抽取器对算法效率并不敏感，使用脚本语言所牺牲的效率可以忽略
  1. 脚本代码不需要编译即可执行，因此本程序只需要输出脚本代码，免于另外实现繁琐的编译器
  1. 脚本语言程序修改方便，容易扩展
  1. 脚本语言程序通常与系统平台无关，便于移植

那么，采用哪种脚本语言呢？下面对目前流行的脚步语言的优缺点总结：
|语言|优点|缺点|
|:-|:-|:-|
|Perl|历史悠久，具有高级语言的强大能力和灵活性|代码可读性差，难以模块化设计，多数个人电脑并不具备运行环境|
|PHP|网页程序设计的主流语言|语法不够严谨，难以模块化设计，容易写出具安全隐患的代码，多数个人电脑并不具备运行环境|
|Lua|轻量级，优先考虑速度|资源不多，许多东西要自行实现，多数个人电脑并不具备Lua运行环境|
|Python|代码简洁优雅，可读性强，良好的OO支持，便于模块化编程，资源丰富，非常适合实现抽取器|多数个人电脑并不具备运行环境|
|Javascript|几乎所有的个人电脑都具备运行环境，使用广泛，安全|各浏览器对javascript的支持程度不同，脚本需要照顾浏览器的兼容性|

### 2.3.2 抽取器的实现方案 ###
考虑到平台的可移植性非常重要，用户并不希望生成的代码需要自行安装一个插件平台才可执行，同时也兼顾语言其他方面的优缺点，最终决定使用Javascript实现。得益于Javascript对AJAX的原生支持，定时无刷新提取数据的实现非常方便。具体的实现细节可参考下一章的内容。

---

# 3 实现细节 #

---

## 3.1 开发环境 ##
为保证能够在各种操作系统平台下编译，本项目100%使用纯C++的代码，并利用Qt Creator生成Makefile编译配置文件。本项目的开发系统环如下：

|操作系统|Windows 7|
|:---|:--------|
|IDE |Qt Creator 1.3.1|
|GUI |Qt 4.6.2 (32 bit)|


---

## 3.2 实现技术 ##
### 3.2.1 网页浏览的实现技术 ###
本程序提供基本的网页浏览功能，其基于开源的浏览器引擎WebKit，该引起高效稳定，兼容性好，并且源代码结构清晰，易于维护，目前流行的浏览器如Safari、Google浏览器、遨游3以及搜狗浏览器2都是采用这一引擎。程序利用WebKit提供的类实现网页的渲染、前进、后退、刷新以及停止加载等功能。
### 3.2.2 交互模块的实现技术 ###
GUI方面，本项目采用Qt框架，使得项目脱离系统平台的依赖。

为了避免交互操作上的冲突，本程序在派生WebKit的view类时重写了一些基类的消息函数，并拦截了一些消息。譬如本程序需要用鼠标右键选定网页内容，于是在派生类鼠标右键响应函数中拦截鼠标事件消息，避免在基类作用下弹出右键菜单：

```
void WebView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        enclosingElement = page()->mainFrame()->hitTestContent(event->pos()).enclosingBlockElement();
    }
    else
    {
        enclosingElement = QWebElement();
        QWebView::mouseReleaseEvent(event);
    }
    this->update();
}
```

为了实时反馈显示用户选取的内容，在用户右击鼠标后，由WebKit返回单击内容hitTestContent，并找到最近的Element。这个Element将暂存在View类中，除了渲染网页之外，重绘函数还将用斜纹高亮的方式在所选Element区域上方绘制出一个同样大小的矩形，以反馈给用户。

若用户需要调整Element的级别，则通过Dom树找到当前Element的父节点或第一个子结点，并把找到的节点设为当前Element。
### 3.2.3 解析处理模块的实现技术 ###
#### 3.2.3.1 选取结点的预处理 ####
在用户确定选取的Element后，程序首先对选取结点做预处理。我们知道，有些元素并不能单独存在，或者单独存在没有意义，譬如脱离table的tbody元素，或者脱离table/tbody的tr元素。如下面的代码所示，本程序的实现考虑了这一情况，更多的元素过滤将在今后完善。
```
while((element->tagName().toLower()=="table"||element->tagName().toLower()=="tbody") && !element->parent().isNull())
    element = &element->parent();
```
#### 3.2.3.2 遍历祖先结点 ####
过滤结点后，需要遍历这个Element的所有祖先结点（如下面的代码所示）。由于具内容意义的最高级标签为body，因此遍历到body元素为止。为了防止在某些不含body的不规范网页上遍历时进入死循环，加入了循环次数的限制（这里设为100次）。
```
for(int i=0;
    i<100 && element->tagName().toLower()!="body" && !element->tagName().isEmpty();
    i++, element=&element->parent())
{
    listAncestor.append(element->tagName().toLower());
    listIndex.append(getIndex(element));
}
```
在遍历祖先结点的同时，用两个list分别记录祖先结点的标签名以及他们是父结点的第几个子结点。关于如何求父结点的子结点位置，将在下面说明。
#### 3.2.3.3 求结点在父结点的位置 ####
Qt的Dom API只提供求邻居结点的函数，使用不断迭代邻居结点的方法将得出错误的结果。

![https://webmo.googlecode.com/svn/wiki/6.jpg](https://webmo.googlecode.com/svn/wiki/6.jpg)

如上图所示的情况，求C在B的儿子位置时，迭代邻居结点将会迭代到F，从而得出错误的位置3。因此需要用如下的方法如下实现：
```
int getIndex(QWebElement *element)
{
    int index = 0;
    if(element->isNull() || element->parent().isNull())
        return -1;
    QWebElement *brother = &element->parent().firstChild();
    //qDebug()<<element->toOuterXml();
    while(!brother->isNull())
    {
        qDebug()<<brother->tagName()<<":"<<brother->classes()<<"      "<<element->tagName();
        if(brother->toOuterXml()==element->toOuterXml())
            return index;
        if(brother->tagName()==element->tagName())
            index++;
        brother=&brother->nextSibling();
    }
    return -1;
}
```
即先找到父节点，然后找到父节点的第一个子节点，判断是否与给定结点一致，如果不一致，就检查当前结点的下个兄弟结点，直到找到位置。该循环总能保证结束。

---

## 3.3 抽取器的实现技术 ##
### 3.3.1 转换XPath路径表达 ###
由3.2得到Element的祖先元素列表以及相应的位置信息后，我们就可以得到Dom树中指向目标Element的路径。这条路径可用XPath描述，譬如要选择某个id为res1135li的ul的第2个li的第1个链接，用XPath描述就是
```
//ul[@id='res1135li']/li[1]/li[0]
```
转换为jQuery的表达形式就是
```
ul[id='res1135li']>li:eq(1)>a:eq(0)
```
或者
```
ul#res1135li>li:eq(1)>a:eq(0)
```
### 3.3.2 内容提取的AJAX框架 ###
本程序生成的抽取器在AJAX的框架下，以get的方式无刷新定时加载页面，并设定返回形式为html。当成功返回时，进入解析处理的回调函数，使用3.3.1所生成的路径，借助jQuery的选择器提取相应的Element，并将其内容写入到输出页面中id为result的div。默认情况下，刷新的时间间隔为5秒，用户可自行配置。
```
$(function getmessage() {
$.ajax({
    url:"http://www.sina.com.cn/",
    dataType:"html",
    cache: false,
    success:function(response){
        $("#result").html($("ul#res1135li>li:eq(1)>a:eq(0)", jQuery(response)).html());
    },
    beforeSend: function() {
        document.getElementById("loadimage").style.display = "block";
    },
    complete: function() {
        $("#loadimage").hide();
        setTimeout("getmessage()", 5000);
    },
    error: function() {
        $("#result").html("error");
    }
})
});
```
### 3.3.3 简化路径的搜索 ###
有时Element在Dom树中的深度很大，可考虑以下的简化搜索路径的方法：

在搜索中加入id属性的约束，我们重写3.2.3.2中的遍历代码，加入id属性的检测，假如某个元素包含了id属性，即停止遍历。以后搜索时，就可以根据id直接定位到该结点，然后再沿着后面的路径定位到目标Element。新的遍历代码如下所示：
```
for(int i=0; 
    i<100 && element->tagName().toLower()!="body" && !element->tagName().isEmpty(); 
    i++, element=&element->parent()) 
{ 
    if(element->hasAttribute("id")) 
    { 
        idTag = element->tagName().toLower(); 
        idVal = element->attribute("id", ""); 
        return; 
    } 
    listAncestor.append(element->tagName().toLower()); 
    listIndex.append(getIndex(element)); 
}
```
注意，我们假定html标签的id属性值唯一。事实上，极少数不规范的网页并不遵循id属性的语义规定，即多个元素使用同一个id。在这种情况下，如果要选择的Element正好是首个使用该id的元素，那么结果仍然是正确的，否则就会定位到错误的Element，但这种概率非常非常低。

---

# 4 应用 #

---

## 4.1 使用环境说明 ##
本程序为绿色软件，无需安装，不依赖额外的运行库，在Windows下通过测试。同时，亦可直接编译Linux、Mac OS等操作系统平台上运行的版本。
## 4.2 应用例子1——获得京东的最新特价商品 ##

**步骤1：前往京东首页**

在地址栏输入http://www.360buy.com/ 并回车，程序窗口显示如下页面：

![https://webmo.googlecode.com/svn/wiki/7.jpg](https://webmo.googlecode.com/svn/wiki/7.jpg)


**步骤2：选取特价信息的区域**

找到页面中的特价信息，并在其上单击鼠标右键，这时该区域被斜纹矩形高亮突出，如下图所示：

![https://webmo.googlecode.com/svn/wiki/8.jpg](https://webmo.googlecode.com/svn/wiki/8.jpg)

但显然这只是特价信息的某个条目，并不是我们想要的全部内容，因此需要扩大区域，以取得所有的特价信息。按一下F6键，则变成：

![https://webmo.googlecode.com/svn/wiki/9.jpg](https://webmo.googlecode.com/svn/wiki/9.jpg)

这样就选中了我们感兴趣的区域，然后按F8确定，将在程序目录下生成output.htm文件，这就是我们需要的抽取器：

![https://webmo.googlecode.com/svn/wiki/10.jpg](https://webmo.googlecode.com/svn/wiki/10.jpg)

**步骤3：查看抽取信息**

用任意的浏览器打开刚刚生成的output.htm文件，即可得到生成的结果：

![https://webmo.googlecode.com/svn/wiki/11.jpg](https://webmo.googlecode.com/svn/wiki/11.jpg)

当然，这里的结果略显凌乱，这是因为内容丢失了CSS的信息，但稍懂CSS的用户都可以轻易控制这些外观，从而使得显示的结果更加美观。

---

## 4.3 应用例子2——获取北京气象信息 ##

**步骤1：前往能够获取北京气象信息的网页**

在地址栏输入http://www.timeanddate.com/worldclock/city.html?n=33 并回车，程序窗口显示如下页面：

![https://webmo.googlecode.com/svn/wiki/12.jpg](https://webmo.googlecode.com/svn/wiki/12.jpg)


**步骤2：选取气象信息的区域**

找到页面中的特价信息，并在其上单击鼠标右键，这时该区域被斜纹矩形高亮突出，如下图所示：

![https://webmo.googlecode.com/svn/wiki/13.jpg](https://webmo.googlecode.com/svn/wiki/13.jpg)

但显然这只是气象信息的某个条目，并不是我们想要的全部内容，因此需要扩大区域，以取得所有的气象信息。按一下F6键，则变成：

![https://webmo.googlecode.com/svn/wiki/17.jpg](https://webmo.googlecode.com/svn/wiki/17.jpg)

很遗憾，这仍然不是我们想要的，继续按F6扩大区域，直到如下图所示：

![https://webmo.googlecode.com/svn/wiki/14.jpg](https://webmo.googlecode.com/svn/wiki/14.jpg)

这样就选中了我们感兴趣的区域，然后按F8确定。

**步骤3：查看抽取信息**

用任意的浏览器打开刚刚生成的output.htm文件，即可得到生成的结果：

![https://webmo.googlecode.com/svn/wiki/15.jpg](https://webmo.googlecode.com/svn/wiki/15.jpg)