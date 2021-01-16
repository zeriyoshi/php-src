<?php

/** @generate-function-entries */

namespace RNG;

interface RNG64Interface extends RNGInterface
{
    public function next64(): int;
}
