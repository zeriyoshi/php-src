--TEST--
Test Random class unserialize error case
--FILE--
<?php

try {
    unserialize('O:6:"Random":2:{i:0;a:0:{}i:1;s:12:"invalid_algo";}');
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

try {
    unserialize('O:6:"Random":2:{i:0;a:0:{}i:1;s:15:"xorshift128plus";}');
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

try {
    unserialize('O:6:"Random":2:{i:0;a:0:{}i:1;s:7:"mt19937";}');
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

try {
    unserialize('O:6:"Random":2:{i:0;a:0:{}i:1;s:6:"secure";}');
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
Algorithm does not registered
Unserialize failed
Unserialize failed
Algorithm does not support serialization
