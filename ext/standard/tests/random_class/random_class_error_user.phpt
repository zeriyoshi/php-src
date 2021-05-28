--TEST--
Test Random class user defined error case
--FILE--
<?php

try {
    (new class(RANDOM_USER) extends Random{})->nextInt();
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
Must be override the method next(): int
