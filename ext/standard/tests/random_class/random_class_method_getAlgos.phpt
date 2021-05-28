--TEST--
Test Random::getAlgos()
--FILE--
<?php

if (is_array(Random::getAlgos())) {
    die("success");
}

?>
--EXPECT--
success
