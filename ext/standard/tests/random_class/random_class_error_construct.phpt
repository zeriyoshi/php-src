--TEST--
Test Random::__construct() error case
--FILE--
<?php

try {
    new Random(RANDOM_SECURE, 12345);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    new Random(new class() implements RandomNumberGenerator {
        public function generate(): int
        {
            return random_int(1, 1000);
        }
    }, 12345);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
Random::__construct(): Argument #2 ($seed) algorithm does not support seed with value
Random::__construct(): Argument #2 ($seed) RandomNumberGenerator does not support seed with value
