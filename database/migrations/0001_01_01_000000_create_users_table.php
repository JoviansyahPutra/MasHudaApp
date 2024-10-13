<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateUsersTable extends Migration
{
    public function up()
    {
        Schema::create('users', function (Blueprint $table) {
            $table->id('ID');
            $table->string('nama');
            $table->string('email')->unique();
            $table->string('password');
            $table->integer('fingerprint_id')->unique()->nullable();
            $table->enum('user_level', ['admin', 'user'])->default('user');
            $table->timestamp('waktu_masuk')->nullable();
            $table->timestamps();
        });
    }

    public function down()
    {
        Schema::dropIfExists('users');
    }
}