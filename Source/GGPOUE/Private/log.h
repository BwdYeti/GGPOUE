/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#pragma once

UENUM(BlueprintType)
enum class EGGPOLogVerbosity : uint8
{
    Info = 0     UMETA(DisplayName = "Info"),
    Verbose = 1     UMETA(DisplayName = "Verbose"),
    VeryVerbose = 2     UMETA(DisplayName = "Very Verbose"),
};

extern void Log(const char *fmt, ...);
extern void Log(EGGPOLogVerbosity Verbosity, const char *fmt, ...);
extern void Logv(EGGPOLogVerbosity Verbosity, const char *fmt, va_list list);

