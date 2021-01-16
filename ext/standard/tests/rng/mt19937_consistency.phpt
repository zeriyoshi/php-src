--TEST--
Test class: MT19937: consistent for mt_srand() / mt_rand()
--FILE--
<?php

for ($i = 0; $i < 100; $i++) {
    $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
    $rng = new \RNG\MT19937($seed);
    \mt_srand($seed);
    for ($j = 0; $j < 1000; $j++) {
        $rng_next = ($rng->next() >> 1 & PHP_INT_MAX); // virtually logical shift
        $func_next = \mt_rand();

        if ($rng_next !== $func_next) {
            die("NG, Inconsistent result MT19937: ${rng_next} mt_rand: ${func_next} i: ${i} j: ${j}");
        }
    }
}
die('OK, Result is consistent.');
?>
--EXPECT--
OK, Result is consistent.
