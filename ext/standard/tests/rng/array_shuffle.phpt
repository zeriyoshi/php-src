--TEST--
Test arrayShuffle() method.
--FILE--
<?php

const SEED = 1234;

foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_random_classes.inc') as $class) {
    $array = range(0, 999);
    $obj = new $class(SEED);
    $obj->arrayShuffle($array);
    foreach (range(0, 999) as $i) {
        foreach ($array as $key => $value) {
            if ($key !== $i) {
                break 2;
            }
        }
        die("NG, Array not shuffled. class: ${class}");
    }
}
die('OK, All array shuffled.');
?>
--EXPECT--
OK, All array shuffled.
