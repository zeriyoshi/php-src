--TEST--
Test Random::nextInt()
--INI--
random.ignore_generated_size_exceeded=1
--FILE--
<?php

foreach (Random::getAlgos() as $algo) {
    $random = new Random($algo);

    if (! is_int($random->nextInt())) {
        die("failure native");
    }
}

if (! is_int((new Random(new class() implements RandomNumberGenerator {
    public function generate(): int
    {
        return random_int(1, 1000);
    }
}))->nextInt())) {
    die("failue user");
}

die("success");

?>
--EXPECT--
success
