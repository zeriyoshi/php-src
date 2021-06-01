--TEST--
Test Random class serialize / unserialize
--FILE--
<?php

// "secure" doesn't support serialize / unserialize.
$instances = array_map(
    function (string $algo): Random {
        return new Random($algo);
    }, 
    array_filter(Random::getAlgos(), function (string $algo): bool {
        return $algo !== 'secure';
    }
));

class UserRNG implements RandomNumberGenerator
{
    protected int $current = 0;

    public function generate(): int
    {
        return ++$this->current;
    }
}

$instances[] = new Random(new UserRNG());

foreach ($instances as $instance) {
    $instance->nextInt();

    if (unserialize(serialize($instance))->nextInt() !== $instance->nextInt()) {
        die('failure');
    }
}

die("success");

?>
--EXPECT--
success
