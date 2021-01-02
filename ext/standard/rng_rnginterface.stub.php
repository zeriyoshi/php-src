<?php

/** @generate-function-entries */

namespace RNG;

interface RNGInterface
{
    public function next(int $min = UNKNOWN, int $max = UNKNOWN): int;
}
