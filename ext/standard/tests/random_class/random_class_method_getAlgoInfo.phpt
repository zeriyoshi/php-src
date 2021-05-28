--TEST--
Test Random::getAlgoInfo()
--FILE--
<?php

foreach (Random::getAlgos() as $algo) {
    var_dump(Random::getAlgoInfo($algo));
}

?>
--EXPECT--
array(3) {
  ["algo"]=>
  string(15) "xorshift128plus"
  ["seedable"]=>
  bool(true)
  ["serializable"]=>
  bool(true)
}
array(3) {
  ["algo"]=>
  string(7) "mt19937"
  ["seedable"]=>
  bool(true)
  ["serializable"]=>
  bool(true)
}
array(3) {
  ["algo"]=>
  string(6) "secure"
  ["seedable"]=>
  bool(false)
  ["serializable"]=>
  bool(false)
}
