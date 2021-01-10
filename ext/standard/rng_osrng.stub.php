<?php

/** @generate-function-entries */

namespace RNG;

final class OSRNG implements RNGInterface
{
    public function next(): int {}
    /** @throws ValueError */
    public function next64(): int {}
}
