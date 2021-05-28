--TEST--
Test Random::shuffleString()
--FILE--
<?php

const TEST_STR = 'foobar';

foreach (Random::getAlgos() as $algo) {
    $string = (new Random($algo))->shuffleString(TEST_STR);

    if (! is_string($string) || ! strlen($string) === strlen(TEST_STR)) {
        die("failue native");
    }
}

$string = (new class(RANDOM_USER) extends Random {
    protected function next(): int
    {
        return random_int(0, 1000);
    }
})->shuffleString(TEST_STR);

if (! is_string($string) || ! strlen($string) === strlen(TEST_STR)) {
    die("failue user");
}

die("success");

?>
--EXPECT--
success
