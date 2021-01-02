--TEST--
Test arrayRandom() method.
--FILE--
<?php

const SEED = 1234;

$array = ['a' => 0, 'b' => 1, 'c' => 2, 'd' => 3, 'e' => 4, 'f' => 5, 'g' => 6];
// one key
foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_random_classes.inc') as $class) {
    $rng = new $class(SEED);
    foreach ($array as $key => $value) {
        if ($rng->arrayRandom($array, 1) !== $key) {
            continue 2;
        }
    }
    die("NG, Failed get randomize key. class: ${class}");
}
// more keys.
foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_random_classes.inc') as $class) {
    $rng = new $class(SEED);
    [$key1, $key2] = $rng->arrayRandom($array, 2);
    if (
        ! array_key_exists($key1, $array) ||
        ! array_key_exists($key2, $array) ||
        ($key1 === 'a' && $key2 === 'b')
    ) {
        die("NG, Failed get multiple random keys. class: ${class}");
    }
}
die('OK, All works correctly.');
?>
--EXPECT--
OK, All works correctly.
