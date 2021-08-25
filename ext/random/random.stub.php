<?php

/** @generate-class-entries */

function mt_srand(int $seed = 0, int $mode = MT_RAND_MT19937): void {}
/** @alias mt_srand */
function srand(int $seed = 0, int $mode = MT_RAND_MT19937): void {}
function rand(int $min = UNKNOWN, int $max = UNKNOWN): int {}
function mt_rand(int $min = UNKNOWN, int $max = UNKNOWN): int {}
function mt_getrandmax(): int {}
/** @alias mt_getrandmax */
function getrandmax(): int {}


function random_bytes(int $length): string {}
function random_int(int $min, int $max): int {}
