--TEST--
Test Random::getBytes()
--FILE--
<?php

foreach (Random::getAlgos() as $algo) {
    $random = new Random($algo);
    $bytes = $random->getBytes(255);

    if (! is_string($bytes) || ! strlen($bytes) === 255) {
        die("failue native");
    }
}

$bytes = (new Random(new class() implements RandomNumberGenerator {
    public function generate(): int
    {
        return random_int(1, 1000);
    }
}))->getBytes(255);

if (! is_string($bytes) || ! strlen($bytes) === 255) {
    die("failue user");
}

die("success");

?>
--EXPECT--
success
