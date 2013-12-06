#!/usr/bin/env python
import zh2py
zy = zh2py.Zh2Py()
zy.load_table("../dict/gbk.dict")
data = open("./testdata/gbkall").read()
print zy.get_pinyin(data)
print zy.get_pinyin(data)
