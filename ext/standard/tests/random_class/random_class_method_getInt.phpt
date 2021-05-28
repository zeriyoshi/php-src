--TEST--
Test Random::getInt()
--FILE--
<?php

foreach (Random::getAlgos() as $algo) {
    $result = (new Random($algo))->getInt(50, 100);

    if (50 > $result || $result > 100) {
        die("failure algo: ${algo} result: ${result}");
    }
}

$result = (new class(RANDOM_USER) extends Random {
    protected function next(): int
    {
        return random_int(0, 1000);
    }
})->getInt(50, 100);

if (50 > $result || $result > 100) {
    die("failure algo: user result: ${result}");
}

die("success");

?>
--EXPECT--
success
