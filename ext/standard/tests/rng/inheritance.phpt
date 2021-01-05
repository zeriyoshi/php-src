--TEST--
Test inheritance classes.
--FILE--
<?php
const SEED = 50;

foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_rng_classes.inc') as $class) {
    $obj = new $class(SEED);
    if (! $obj instanceof \RNG\RNGInterface) {
        die("NG, ${class} has not implemented \\RNG\\RNGInterface.");
    }
}

foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_random_classes.inc') as $class) {
    $obj = new $class(SEED);
    if (! $obj instanceof \RNG\RandomInterface) {
        die("NG, ${class} has not implemented \\RNG\\RandomInterface.");
    }
}

die('OK, inheritance is corrected.');
?>
--EXPECT--
OK, inheritance is corrected.
