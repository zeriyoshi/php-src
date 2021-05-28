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

$array = (new class(RANDOM_USER) extends Random {
    protected function next(): int
    {
        return random_int(0, 1000);
    }
})->shuffleArray(TEST_ARR);

if (! is_array($array) || ! count($array) === count(TEST_ARR)) {
    die("failue user");
}

die("success");

?>
--EXPECT--
success
