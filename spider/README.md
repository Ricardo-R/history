###程序设计
>####功能
>>获取bing搜索引擎的搜索xls类型文件的结果页面，然后提取页面中的每一个xls文件的URL

>####SERsult类

>>#####def \__init(self)__:
>>>配置参数和URL，以及设置提取url的正则表达式，载入相应的配置文件。

>>#####def openUrl(self):
>>>构建请求的完整url——请求的搜索关键字和搜索结果的页面数
<br>获取搜索结果页面

>>#####def extractUrl(self):
>>>提取搜索结果页面html文件中的xls文件的url
<br>将提取的url缓存

>>#####def save(self):
>>>将提取的url和当前搜索到的页面数保存到磁盘文件
