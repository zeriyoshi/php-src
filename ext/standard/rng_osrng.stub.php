<?php

/** @generate-function-entries */

namespace RNG;

class OSRNG implements RNGInterface
{
    public function next(): int {}
    /** @throws ValueError */
    public function next64(): int {}
}
