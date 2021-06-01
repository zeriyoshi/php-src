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

$result = (new Random(new class() implements RandomNumberGenerator {
    public function generate(): int
    {
        return random_int(1, 1000);
    }
}))->getInt(50, 100);

if (50 > $result || $result > 100) {
    die("failure algo: user result: ${result}");
}

die("success");

?>
--EXPECT--
success
