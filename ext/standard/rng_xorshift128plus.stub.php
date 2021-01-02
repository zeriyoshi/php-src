<?php

/** @generate-function-entries */

namespace RNG;

class XorShift128Plus implements RandomInterface
{
    public function __construct(int $seed) {}
    public function next(int $min = UNKNOWN, int $max = UNKNOWN): int {}
    public function arrayShuffle(array &$array): bool {}
    public function stringShuffle(string $string): string {}
    public function arrayRandom(array $array, int $num = 1): int|string|array {}
    public function __serialize(): array {}
    public function __unserialize(array $data): void {}
}
