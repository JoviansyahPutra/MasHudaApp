<?php

namespace App\Http\Controllers\API;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use App\Models\User;
use App\Models\AccessLog;

class FingerprintController extends Controller
{
    public function verify(Request $request)
       {
           $fingerprintId = $request->input('fingerprint_id');
           $user = User::where('fingerprint_id', $fingerprintId)->first();

           if ($user) {
               AccessLog::create([
                   'user_id' => $user->ID,
                   'access_granted' => true,
                   'timestamp' => now(),
               ]);

               return response()->json([
                   'approved' => true,
                   'user_level' => $user->user_level,
               ]);
           } else {
               AccessLog::create([
                   'user_id' => null,
                   'access_granted' => false,
                   'timestamp' => now(),
               ]);

               return response()->json([
                   'approved' => false,
               ]);
           }
       }
}
