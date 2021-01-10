--TEST--
Test function: array_rand() function with RNGs.
--FILE--
<?php

const SEED = 1234;

$array = ['a' => 0, 'b' => 1, 'c' => 2, 'd' => 3, 'e' => 4, 'f' => 5, 'g' => 6];
// one key
foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_rng_classes.inc') as $class) {
    $rng = new $class(SEED);
    foreach ($array as $key => $value) {
        if (array_rand($array, 1, $rng) !== $key) {
            continue 2;
        }
    }
    die("NG, Failed get randomize key. RNG class: ${class}");
}
// more keys.
foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_rng_classes.inc') as $class) {
    $rng = new $class(SEED);
    [$key1, $key2] = array_rand($array, 2, $rng);
    if (
        ! array_key_exists($key1, $array) ||
        ! array_key_exists($key2, $array) ||
        ($key1 === 'a' && $key2 === 'b')
    ) {
        die("NG, Failed get multiple random keys. RNG class: ${class}");
    }
}
die('OK, All works correctly.');
?>
--EXPECT--
OK, All works correctly.
