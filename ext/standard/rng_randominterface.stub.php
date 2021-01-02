<?php

/** @generate-function-entries */

namespace RNG;

interface RandomInterface extends RNGInterface
{
    public function arrayShuffle(array &$array): bool;
    public function stringShuffle(string $string): string;
    public function arrayRandom(array $array, int $num = 1): int|string|array;
}
