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

if (! is_int((new class(RANDOM_USER) extends Random {
    protected function next(): int
    {
        return random_int(0, 1000);
    }
})->nextInt())) {
    die("failue user");
}

die("success");

?>
--EXPECT--
success
