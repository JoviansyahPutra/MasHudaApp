<?php

namespace App\Models;

use Illuminate\Foundation\Auth\User as Authenticatable;
use Illuminate\Database\Eloquent\Factories\HasFactory;

class User extends Authenticatable
{
    use HasFactory;

    protected $fillable = [
        'nama',
        'email',
        'password',
        'fingerprint_id',
        'user_level',
        'waktu_masuk',
    ];

    protected $casts = [
        'waktu_masuk' => 'datetime',
    ];
}