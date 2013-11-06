#-*- coding:UTF-8 -*-

from distutils.core import setup, Extension

m_zh2py = Extension(
	'zh2py',
	include_dirs = ["src"],
	sources = [
		 'src/Zh2py.c',
	]
)

setup(
	name = "zh2py",
	version = '0.1',
	author = "renlu.xu",
	author_email = "xurenlu@gmail.com",
	url = "http://github.com/xurenlu/zh2py",
	description = " chinese words => pinyin",
	license = "NEW BSD",
	ext_modules = [m_zh2py],
)
