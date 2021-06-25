--TEST--
Test Random: NumberGenerator: inheritance
--FILE--
<?php

foreach (include __DIR__ . DIRECTORY_SEPARATOR . 'common.inc' as $klass => $is_clone_or_serializable) {
    $instance = new $klass();

    if (!$instance instanceof Random\NumberGenerator\RandomNumberGenerator) {
        die('failure: ' . $instance::class);
    }
}

die('success');
?>
--EXPECT--
success
