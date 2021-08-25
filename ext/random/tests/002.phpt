--TEST--
test1() Basic test
--EXTENSIONS--
random
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension random is loaded and working!
NULL
