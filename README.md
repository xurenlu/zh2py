Zh2py   -- 汉字转拼音方案

zh2py是一个简单而高效的汉字转拼音方案,提供php和python扩展,

实现方式:将汉字转换成数字序号,然后去词表数组中查询拼音.词表已经事先生成好,
存储在dict/gbk.dict中,由mmap加载.

示例:
```php
<?php
if(zh2py_load_table("../dict/gbk.dict")){
 $chinese = "测试数据";
 print zh2py_get_pinyin($chinese)."\n";
}
```

```python
#!/usr/bin/env python
import zh2py
zy = zh2py.Zh2Py()
zy.load_table("../dict/gbk.dict")
data = open("./testdata/gbkall").read()
print zy.get_pinyin(data)
print zy.get_pinyin(data)
```

本项目还整理了一个多音字的词库,存入在dict/polymorphic.dict;示例文件是php-example/test_poly.php:
```php
<?php
include "./Zh2pyPoly.php";
$z2p = new Zh2pyPoly("../dict/polymorphic.dict","../dict/gbk.dict");
print_r($z2p->getPinyin("我们都是中国人.我有一匹黄骠马.我屏息屏气.一页扁舟来."));
```

注意,如果运行php的时候拼音结果出不来,请注意查看php文件编码是否是GBK;
