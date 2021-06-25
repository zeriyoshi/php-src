--TEST--
Test Random: NumberGenerator: serialize
--FILE--
<?php

foreach (include __DIR__ . DIRECTORY_SEPARATOR . 'common.inc' as $klass => $is_clone_or_serializable) {
    if (!$is_clone_or_serializable) {
        continue;
    }

    $instance = new $klass();

    $instance->generate();
    $clone_instance = unserialize(serialize($instance));

    if ($instance->generate() !== $clone_instance->generate()) {
        die('failure: ' . $instance::class);
    }
}

die('success');
?>
--EXPECT--
success
