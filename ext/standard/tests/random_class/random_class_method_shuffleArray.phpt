--TEST--
Test Random::shuffleArray()
--FILE--
<?php

const TEST_ARR = ['a', 'b', 'c', 'd', 'e'];

foreach (Random::getAlgos() as $algo) {
    $array = (new Random($algo))->shuffleArray(TEST_ARR);

    if (! is_array($array) || ! count($array) === count(TEST_ARR)) {
        die("failue native");
    }
}

$array = (new Random(new class() implements RandomNumberGenerator {
    public function generate(): int
    {
        return random_int(1, 1000);
    }
}))->shuffleArray(TEST_ARR);

if (! is_array($array) || ! count($array) === count(TEST_ARR)) {
    die("failue user");
}

die("success");

?>
--EXPECT--
success
