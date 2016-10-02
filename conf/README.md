##conf
>###1、需求分析
>>1）解决哪些问题？
>>>a、读取单个配置文件，获取里面的配置信息
<br>b、检查选项是否符合
<br>c、若没有配置，使用默认配置

>>2）具体的问题需求
>>>a、读取单个配置文件，获取里面的配置信息
>>>>I.指定完整的配置文件路径
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;设定默认配置文件为：./conf/default.conf
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;设定用户配置文件为：./conf/user.conf

>>>>II.规定配置文件的格式
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;每一项单独占一行，空行或者只有空格的行将被忽略。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;以“#”开头的一行为注释行，注释行的首字符必须是“#”，其之前不能有空格等其它字符。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;注释行必须为单独行，不能在选项末尾使用“#”进行注释。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;每一项的选项名必须从开头开始写，选项名之前不允许有空格等。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;每一项包含三个字段([...]表示一个字段内容)，语法为：
>>>>>                      [选项名][空格][选项值]
>>>>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;默认配置文件的选项名、选项值字段不可以为空。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;配置文件的选项名、选项值字段可以为空，为空表示使用默认值。

>>>>III.构建默认字典
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;检查默认配置文件是否存在。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 默认选项将使用载入默认配置文件的方式。默认配置文件包含所有的选项和其默认值。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;读取默认配置文件，忽略注释行。将每一项分割按照分隔符分割为选项名和选项值两个字段。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;将选项名作为字典的key，选项值为字典的value。


>>>b-c、检查选项是否符合
>>>>I.构建程序使用的配置选项
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;读取配置文件，忽略注释行。将每一项分割按照分隔符分割为选项名和选项值两个字段。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;默认字典使用has_key方法判定选项名是否存在，不存在即不合法。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;由于不同选项值的具体要求不同，比如有些值是yes、no而有些是0、1等，不对选项值合法性检查。
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;判定选项值是否为空，不为空，用该选项值覆盖默认字典中的值。为空则保留默认字典中的值。

>###2、系统设计
>>1）模块
>>>a、初始化默认配置模块（简称init模块）
>>>>I.模块功能：
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;构建默认选项名的选项值的字典。

>>>b、应用配置文件配置选项值模块（简称app模块）
>>>>I.模块功能：
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;将配置文件中配置的选项名对应的选项值应用到默认字典，得到的字典将被作为程序运行时的配置。

>>2）模块之间的关系
>>>应用配置文件配置选项值模块  依赖  初始化默认配置模块

>>3）数据结构设计
>>>数据结构：
>>>>dict

>>>范围：
>>>>由于app模块依赖于init模块，并且该数据结构将会被程序运行时使用，因此其范围是该类中的全局范围。
