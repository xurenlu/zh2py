all:
	#rm ./a.out
	gcc -o zh2py ./main.c -I./
	./zh2py ./gbk.dict ./chn.dict
