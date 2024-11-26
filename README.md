# C++ :IfcOpenShell
目的：提取IFC文件中构件信息。
# 示例代码解析
示例代码功能是对IFC2X3版本的IFC文件检索IFCbuildingelement信息，逐行输出在命令行。
 其中IFC2x3版本可以更换，ifcbuildingelement检索信息可以更换。

## main函数输入
在ifcopenshell example模块下的IFCparse项目中，main函数跟平时不同。它有形参，其中argc是参数个数，argv代表为char类型的形参至少一个，使用时需要在vs的工具->命令行->开发人员openshell中打开才能使用：
```cpp
int main(int argc, char** argv)
* **打开文件**：IfcParse::IfcFile file(argv[1])，一行代码打开。good是file打开状态的参数，我记得有四个状态，good是1，如果不是good就是打开不成功，然后输出Unable to parse .ifc file
```cpp
//提示用命令行执行，参数为数组，可以提供多个
int main(int argc, char** argv)
//Logger类中的Setoutput设定处理和log结果输出路径，配置日志输出流
Logger::SetOutput(&std::cout,&std::cout);
//parse分析一行就解决
IfcParse::IfcFile file(argv[1]);
//如果file打开失败，
if (!file.good()) {
                std::cout << "Unable to parse .ifc file" << std::endl;
                return 1;
        }
//file内的实例对象有IfcSchema::IfcBuildingElement::list::ptr类型的elements存储，其中：：list存储了该ifc-schema版本下的xx实例属性，通过std：：vector存储，通过：：ptr分享读取
IfcSchema::IfcBuildingElement::list::ptr elements = file.instances_by_type<IfcSchema::IfcBuildingElement>();
```
## 输出和日志路径
**Logger：：SetOutput函数**接受两个ostream类型的指针，记录输出路径和日志路径，其中如果第二个形参为空，则设置第一个新参数为log日志的参数。
在示例代码中为std：：cout,就是shell黑框打印输出。

```cpp
Logger::SetOutput(std::ostream* l1, std::ostream* l2) {
        wlog1 = wlog2 = 0;
        log1 = l1; 
        log2 = l2; 
        if (!log2) {
                log2 = &log_stream;
        }
}
```
## IfcBuildingElement API
 IfcBuildingElement的直接属性和导出属性由该entity直接记录，反属性由另外的class记录，这里只讨论entity属性。
 关于IfcBuildingElement的声明如下：
```cpp
class IFC_PARSE_API IfcBuildingElement : public  IfcElement {
public:
        virtual const IfcParse::entity& declaration() const;
    static const IfcParse::entity& Class();
    IfcBuildingElement (IfcEntityInstanceData* e);
    IfcBuildingElement (std::string v1_GlobalId, ::Ifc2x3::IfcOwnerHistory* v2_OwnerHistory, boost::optional< std::string > v3_Name, boost::optional< std::string > v4_Description, boost::optional< std::string > v5_ObjectType, ::Ifc2x3::IfcObjectPlacement* v6_ObjectPlacement, ::Ifc2x3::IfcProductRepresentation* v7_Representation, boost::optional< std::string > v8_Tag);
    typedef aggregate_of< IfcBuildingElement > list;
};
```
* **构造函数**：从这个class中，我们可以知道IfcBuildingElement继承于IfcElement，有两个构造函数，第一个形参是IfcEntityInstanceData类型的指针e，第二个是IfcBuildingElement的直接属性和导出属性。
* **实例存储**：IfcBuildingElement如果存在实例，那么所有instance（实例）都会存储在一个IfcBuildingElement的list集合中。
* **读取逻辑**：::list存储了该ifc-schema版本下的IfcBuildingElementsd所有instance实例，通过std：：vector存储，通过：：ptr分享读取。
# 运行结果
1. 打开vs编辑器的开发者命令行工具，切换到example的ifcparseExample.exe文件路径
2. 在命令行输入以下命令：
```cpp
<你的路径>： <ifcparceexample.exe路径> IFC文件路径
```
结果如图：

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/bbcdfa779050fd77b7390821d9e118c1.png)
# 参考博客
[c++main函数知识点](https://blog.csdn.net/xinran0703/article/details/95487843)
[IFC4文档](http://www.vfkjsd.cn/ifc/ifc4/index.htm)
