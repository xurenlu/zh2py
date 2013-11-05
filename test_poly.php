<?php
include "./Zh2pyPoly.php";
$z2p = new Zh2pyPoly("./polymorphic.dict","./gbk.dict");
print_r($z2p->getPinyin("我们都是中国人.我有一匹黄骠马.我屏息屏气.一页扁舟来."));
