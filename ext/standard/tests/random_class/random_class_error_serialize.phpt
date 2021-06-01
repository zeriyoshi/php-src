--TEST--
Test Random class serialize error case
--FILE--
<?php

try {
    serialize(new Random(RANDOM_SECURE));
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

serialize(new class() extends Random {
    protected function next(): int
    {
        return random_int(0, 1000);
    }
});

?>
--EXPECTF--
Algorithm does not support serialization

Fatal error: Class Random@anonymous may not inherit from final class (Random) in %s on line %d
