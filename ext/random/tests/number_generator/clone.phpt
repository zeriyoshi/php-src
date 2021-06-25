--TEST--
Test Random: NumberGenerator: clone
--FILE--
<?php

foreach (include __DIR__ . DIRECTORY_SEPARATOR . 'common.inc' as $klass => $is_clone_or_serializable) {
    if (!$is_clone_or_serializable) {
        continue;
    }

    $instance = new $klass();

    $instance->generate();
    $clone_instance = clone $instance;

    if ($instance->generate() !== $clone_instance->generate()) {
        die('failure: ' . $instance::class);
    }
}

die('success');
?>
--EXPECT--
success
