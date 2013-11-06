#!/usr/bin/env python
import zh2py
zy = zh2py.Zh2Py()
zy.load_table("./gbk.dict")
data = open("./gbkall").read()
print zy.get_pinyin(data)
print zy.get_pinyin(data)
