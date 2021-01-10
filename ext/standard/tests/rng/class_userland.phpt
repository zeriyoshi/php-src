--TEST--
Test class: userland implemented classes.
--FILE--
<?php

class FixedNumberGenerator implements \RNG\RNGInterface
{
    public function next(): int
    {
        return 1;
    }

    public function next64(): int
    {
        return 2;
    }
}

$rng = new FixedNumberGenerator();
$array = range(1, 100);

$rng->next() . PHP_EOL;
$rng->next64() . PHP_EOL;
array_rand($array, 1, $rng);
// array_rand($array, 2, $rng); Note: Stuck at a fixed number
shuffle($array, $rng);
str_shuffle('foobar', $rng);
rng_range($rng, 1, 1000) . PHP_EOL;
rng_bytes($rng, 100) . PHP_EOL;
die('OK, userland implementation works correctly.');
?>
--EXPECT--
OK, userland implementation works correctly.
