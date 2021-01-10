--TEST--
Test class: inheritance classes.
--FILE--
<?php
const SEED = 50;

foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_rng_classes.inc') as $class) {
    $rng = new $class(SEED);
    if (! $rng instanceof \RNG\RNGInterface) {
        die("NG, ${class} has not implemented \\RNG\\RNGInterface.");
    }
}

die('OK, inheritance is corrected.');
?>
--EXPECT--
OK, inheritance is corrected.
