--TEST--
Test Random::nextInt() result XorShift128+ (32-bit) error case
--SKIPIF--
<?php
if (PHP_INT_SIZE >= 8) {
    echo 'skip this test only available on 32bit enviroment.';
}
?>
--FILE--
<?php

ini_set('random.ignore_generated_size_exceeded', true);
(new Random(RANDOM_XORSHIFT128PLUS, 12345))->nextInt();

ini_set('random.ignore_generated_size_exceeded', false);
try {
    (new Random(RANDOM_XORSHIFT128PLUS, 12345))->nextInt();
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
Generated random number size exceeded
