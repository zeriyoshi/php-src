--TEST--
Test Random: serialize
--FILE--
<?php

foreach (include __DIR__ . DIRECTORY_SEPARATOR . 'number_generator' . DIRECTORY_SEPARATOR . 'common.inc' as $klass => $is_clone_or_serializable) {
    if (!$is_clone_or_serializable) {
        continue;
    }

    $rng = new $klass();
    $instance = new Random($rng);

    $instance->nextInt();
    $clone_instance = unserialize(serialize($instance));

    if ($instance->nextInt() !== $clone_instance->nextInt()) {
        die('failure: ' . $rng::class);
    }
}

die('success');
?>
--EXPECT--
success
