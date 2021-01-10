<?php

/** @generate-function-entries */

namespace RNG;

interface RNGInterface
{
    public function next(): int;
    /** @throws ValueError */
    public function next64(): int;
}
