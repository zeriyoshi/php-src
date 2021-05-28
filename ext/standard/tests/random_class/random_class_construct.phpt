--TEST--
Test Random::__construct()
--FILE--
<?php

new Random();
new Random(seed: 1234);
new Random(algo: RANDOM_XORSHIFT128PLUS);
new Random(RANDOM_XORSHIFT128PLUS, 1234);
new Random(algo: RANDOM_MT19937);
new Random(algo: RANDOM_SECURE);

die("success");

?>
--EXPECT--
success
