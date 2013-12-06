<?php
class Zh2pyPoly {
    /**
     * @var string 多音字词表的路径
     */
    public $polyDict;
    /**
     * @var string 拼音词表文件的位置;
     */
    public $pinyinDict;

    /**
     * @var resource acfilter的句柄
     */
    public $ac=null;

    /**
     * @var array
     */
    public $poly_dict = array();
    /**
     * @param mixed $pinyinDict
     */
    public
    function setPinyinDict($pinyinDict)
    {
        $this->pinyinDict = $pinyinDict;
    }

    /**
     * @return mixed
     */
    public
    function getPinyinDict()
    {
        return $this->pinyinDict;
    }

    /**
     * @param mixed $polyDict
     */
    public
    function setPolyDict($polyDict)
    {
        $this->polyDict = $polyDict;
    }

    /**
     * @return mixed
     */
    public
    function getPolyDict()
    {
        return $this->polyDict;
    }

    /**
     * @param $poly_dict
     * @param $py_dict
     */
    public function __construct($polyDict,$pinyinDict)
	{
        $this->polyDict = $polyDict;
        $this->pinyinDict = $pinyinDict;
        if(!zh2py_table_loaded()){
            zh2py_load_table($pinyinDict);
        }
        $this->ac =acfilter_new();
        $lines = file($polyDict);
        foreach($lines as $line){
            $dict = explode(":",trim($line));
            $dict[0]= trim($dict[0]);
            $dict[1] = trim($dict[1]);
            if(empty($dict[0])) continue;
            if(sizeof($dict)==2){
                $this->poly_dict[$dict[0]] = $dict[1];
            }
            acfilter_add_word($this->ac,$dict[0]);
        }

        acfilter_prepare($this->ac);

        # code...
	}

    public function getPinyin($str){
        $polyHits = trim(acfilter_check_text($this->ac,$str,1),"|");

        $polyHits = explode("|",$polyHits);

        foreach($polyHits as $word){
            $this->log("handle  polymorphic word:".$word);
            $str = str_replace($word,$this->poly_dict[trim($word)]." ",$str);
        }
        return zh2py_get_pinyin($str);
    }
    public function log($msg){
        error_log($msg);
    }
    public function __destruct(){
        acfilter_close($this->ac);
    }
    //public function
}
//""
