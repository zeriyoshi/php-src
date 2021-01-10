--TEST--
Test class: serialize in supported RNG classes.
--FILE--
<?php
foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_serializable_rng_classes.inc') as $class => $is_seed) {
    $t = $is_seed ? new $class(random_int(PHP_INT_MIN, PHP_INT_MAX)) : new $class();
    $t->next();

    $s = serialize($t);
    $t_next = $t->next();
    $ut = unserialize($s);
    $ut_next = $ut->next();
    if ($ut_next !== $t_next) {
        die("NG, broken detected. class: ${class} method: next() correct: ${t_next} result: ${ut_next}");
    }
}
die('OK, serialize / unserialize works correctly.');
?>
--EXPECT--
OK, serialize / unserialize works correctly.
