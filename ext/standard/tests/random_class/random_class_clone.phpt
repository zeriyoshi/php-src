--TEST--
Test Random class clone
--INI--
random.ignore_generated_size_exceeded=1
--FILE--
<?php

// "secure" doesn't same result nextInt()
$instances = array_map(
    function (string $algo): Random {
        return new Random($algo);
    }, 
    array_filter(Random::getAlgos(), function (string $algo): bool {
        return $algo !== 'secure';
    }
));

foreach ($instances as $instance) {
    $instance->nextInt();
    $cloned_instance = clone $instance;

    if ($cloned_instance->nextInt() !== $instance->nextInt()) {
        die('failure native');
    }
}

$secure_instance = new Random(RANDOM_SECURE);
$secure_instance_clone = clone $secure_instance;

$secure_instance->nextInt();
$secure_instance_clone->nextInt();

die("success");

?>
--EXPECT--
success
