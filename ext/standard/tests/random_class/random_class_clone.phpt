--TEST--
Test Random class clone
--INI--
random.ignore_generated_size_exceeded=1
--FILE--
<?php

$user_defined = new class() extends Random {
    private int $count = 0;
    protected int $foo = 1;
    public string $bar = 'bar';

    public function __construct()
    {
        parent::__construct(RANDOM_USER);
    }

    protected function next(): int
    {
        ++$this->count;
        $this->bar = $this->bar . $this->count;
        return $this->count;
    }
};

$user_defined->nextInt();
$user_defined_clone = clone $user_defined;

if ($user_defined->nextInt() !== $user_defined_clone->nextInt() ||
    $user_defined->bar !== $user_defined_clone->bar) {
    die('failure user');
}


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

die("success");

?>
--EXPECT--
success
