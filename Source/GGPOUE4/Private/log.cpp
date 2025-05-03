/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#include "log.h"
#include "types.h"
#include "GGPOUE4_Settings.h"

static char logbuf[4 * 1024 * 1024];

void Log(const char *fmt, ...)
{
   va_list args;
   va_start(args, fmt);
   Logv(fmt, args);
   va_end(args);
}

void Logv(const char *fmt, va_list args)
{
#if WITH_EDITOR
   if (GIsEditor)
   {
       // Get the settings object
       // Return if logging is not enabled
      UGGPOUE4_Settings* Settings = GetMutableDefault<UGGPOUE4_Settings>();
      if (!Settings->LoggingEnabled) {
         return;
      }

      // Apply the string format
      vsprintf_s(logbuf, ARRAY_SIZE(logbuf), fmt, args);
      FString Message = FString(strlen(logbuf), logbuf);

      // If this is an instance playing in the editor, include its Id
      if (GPlayInEditorID >= 0)
      {
          Message.InsertAt(0, FString::Printf(TEXT("PIE %d :: "), GPlayInEditorID));
      }

      UE_LOG(LogNet, Display, TEXT("%s"), *Message);
   }
#endif
}

