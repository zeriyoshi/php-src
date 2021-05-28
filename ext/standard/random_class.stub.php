<?php

/** @generate-function-entries */

class Random
{
    public static function getAlgos(): array;
    public static function getAlgoInfo(string $algo): ?array;
    public function __construct(string $algo = RANDOM_XORSHIFT128PLUS, ?int $seed = null) {}
    public function nextInt(): int {}
    public function getInt(int $min, int $max): int {}
    public function getBytes(int $length): string {}
    public function shuffleArray(array $array): array {}
    public function shuffleString(string $string): string {}
    public function __serialize(): array {}
    public function __unserialize(array $data): void {}
    protected function next(): int {}
}
