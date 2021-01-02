--TEST--
Test stringShuffle() method.
--FILE--
<?php

const SEED = 1234;

foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_random_classes.inc') as $class) {
    $string = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.';
    $obj = new $class(SEED);
    $shuffled_string = $obj->stringShuffle($string);
    for ($i = 0; $i < strlen($string); $i++) {
        if ($string[$i] !== $shuffled_string[$i]) {
            continue 2;
        }
    }
    die("NG, String not shuffled. class: ${class} string: ${shuffled_string}");
}
die('OK, All string shuffled.');
?>
--EXPECT--
OK, All string shuffled.
