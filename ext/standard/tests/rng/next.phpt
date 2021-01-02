--TEST--
Test next() results.
--FILE--
<?php
foreach (include(__DIR__ . DIRECTORY_SEPARATOR . '_rng_classes.inc') as $class) {
    $obj = new $class(random_int(PHP_INT_MIN, PHP_INT_MAX));
    for ($i = 1; $i <= 10000; $i++) {
        $next = $obj->next();
        if (0 > $next) {
            die("NG, invalid result: ${next}. (loop: ${i})");
        }
    }
}

die('OK, result is corrected.');
?>
--EXPECT--
OK, result is corrected.
