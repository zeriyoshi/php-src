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
    new Random(RANDOM_USER, 12345);
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
Random::__construct(): Argument #2 ($seed) algorithm does not support seed with value
User defined algorithm must be inherited
