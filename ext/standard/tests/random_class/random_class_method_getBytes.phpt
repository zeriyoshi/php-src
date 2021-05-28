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

$bytes = (new class(RANDOM_USER) extends Random {
    protected $current = 0;

    protected function next(): int
    {
        return ++$this->current;
    }
})->getBytes(255);

if (! is_string($bytes) || ! strlen($bytes) === 255) {
    die("failue user");
}

die("success");

?>
--EXPECT--
success
