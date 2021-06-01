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

$string = (new Random(new class() implements RandomNumberGenerator {
    public function generate(): int
    {
        return random_int(1, 1000);
    }
}))->shuffleString(TEST_STR);

if (! is_string($string) || ! strlen($string) === strlen(TEST_STR)) {
    die("failue user");
}

die("success");

?>
--EXPECT--
success
