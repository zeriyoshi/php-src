--TEST--
Test Random class serialize error case
--FILE--
<?php

try {
    serialize(new Random(RANDOM_SECURE));
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

try {
    serialize(new class() extends Random {
        protected function next(): int
        {
            return random_int(0, 1000);
        }
    });
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
Algorithm does not support serialization
Anonymous class serialization is not allowed
