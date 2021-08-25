--TEST--
Check if random is loaded
--EXTENSIONS--
random
--FILE--
<?php
echo 'The extension "random" is available';
?>
--EXPECT--
The extension "random" is available
