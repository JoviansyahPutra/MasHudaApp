<?php

namespace App\Filament\Resources\AccessLogResource\Pages;

use App\Filament\Resources\AccessLogResource;
use Filament\Actions;
use Filament\Resources\Pages\CreateRecord;

class CreateAccessLog extends CreateRecord
{
    protected static string $resource = AccessLogResource::class;
}
