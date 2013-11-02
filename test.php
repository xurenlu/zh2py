<?php
/**
 * php.ini:
 * [zh2py]
 * zh2py.pinyin_dict = "/etc/zh2py/gbk.dict"
 * zh2py.polymorphic_dict ="/etc/zh2py/polymorphic.dic"
 * #zh2py.automatic_load_dicts = 1
 * */
$chinese = iconv("UTF-8","GBK","我们都是中国人 \nOne world One dream \n!@#$%^^&*()");
if(zh2py_table_loaded()){
    echo "table loaded:\n";
   print zh2py_get_pinyin($chinese)."\n";   
}else{
    echo "table not loaded\n";
}
zh2py_unload_table();
if($tmp = zh2py_load_table("./gbk.dict")){
    print "load table:".intval($tmp)."\n";
    print zh2py_get_pinyin($chinese)."\n";
    var_dump(zh2py_free_last_result());
    print zh2py_get_pinyin($chinese)."\n";

}else{
    throw new Exception("Can't load dict:./gbk.dict");
}
exit();
/**
 * not finished;
 * */
$z2p = new Zh2Py();
if( $z2p->table_loaded()){
    print $z2p->get_pinyin($chinese)."\n";
}
$z2p->unload_table();
if($z2p->load_table("./gbk.dict")){
    print $z2p->get_pinyin($chinese)."\n";
}else{
    throw new Exception("Can't load dict:./gbk.dict");
}
