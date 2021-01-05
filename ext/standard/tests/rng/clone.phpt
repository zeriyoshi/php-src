--TEST--
Test clone classes.
--FILE--
<?php

foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_clonable_rng_classes.inc') as $class) {
    $rng1 = new $class(1);
    $rng1->next();

    $rng2 = clone $rng1;

    $rng1_value = $rng1->next();
    $rng2_value = $rng2->next();

    if ($rng1_value !== $rng2_value) {
        die("NG, state is not cloned. class: ${class} RNG1 value: ${rng1_value} RNG2 value: ${rng2_value}");
    }
}
die('OK, clone is corrected.');
?>
--EXPECT--
OK, clone is corrected.
