/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QSet>

namespace Quts {

constexpr char NAMESPACE_ERROR[] = "Name conflicts";
constexpr char INVALID_ARGUMENT[] = "Invalid argument";
constexpr char INVALID_OBJECT[] = "Invalid object";
constexpr char OBJECT_NOT_FOUND[] = "Object not found";
constexpr char NUMBER_NOT_FOUND[] = "Number not found";
constexpr char TIMEOUT[] = "Timeout";
constexpr char AUDIO_ERROR[] = "Audio play failed";
constexpr char SIGNAL_NOT_FOUND[] = "Signal not found";
constexpr char STACK_UNDERFLOW[] = "Stack underflow";
constexpr char STRAY_SIGNAL[] = "Stray signal";
constexpr char UNKNOWN_COMMAND[] = "Unknown command";
constexpr char INVALID_ERROR[] = "Invalid error";
constexpr char PARSE_ERROR[] = "Invalid source file";
constexpr char NO_SLOTS_SET[] = "Nothing to wait";
constexpr char SLOT_ERROR[] = "Cannot set slot";
constexpr char INVALID_SIGNAL[] = "Invalid signal";
constexpr char DIVISION_BY_ZERO[] = "Division by zero";
constexpr char NOT_READY[] = "Not ready";
constexpr char ASSERTED[] = "Assert";
constexpr char SUBROUTINE_ERROR[] = "Subroutine definition not allowed";
constexpr char SUBROUTINE_NAME[] = "Subroutine has invalid name";
constexpr char UNBALANCED_SCOPE[] = "Unbalanced Scope";
constexpr char NOT_FOUND[] = "Not found";
constexpr char INVALID_INDEX[] = "Invalid Index";
constexpr char MISC_ERROR[] = "Unknown error";
constexpr char ALREADY_EXISTS[] = "Already exists";
constexpr char INVALID_SUBSYSTEM[] = "Subsystem not found";
constexpr char AMBIGUOUS_NAME[] = "Ambigous name";


constexpr char TRUE_[] = "true";
constexpr char FALSE_[] = "false";
constexpr char NOT_[] = "not";
/**DOC
 * N maxInt
 * S CORE
 * Maximum value
**/
constexpr char MAXINT_[] = "maxInt";
/**DOC
 * N minInt
 * Minimimum value
**/
constexpr char MININT_[] = "minInt";

/**DOC
 * N debug
 * Print debug info
**/
constexpr char DEBUG_[] = "debug";

/**DOC
 * N breakpoints
 **/

constexpr char BREAKPOINTS_[] = "breakpoints";

/**DOC
 * N printPrefix
 * prefix for each print command
**/
constexpr char PRINTPREFIX_[] = "printPrefix";

/**DOC
 * N commandSpeed
 * Command calling speed
**/
constexpr char SPEED_[] = "commandSpeed";

constexpr char INVALID_NAME[] = "_*INVALID*_";
constexpr char GLOBAL_SEP[] = ".";
constexpr char END_OF_SCOPE[] = ";;";

static const QSet<QString> SCOPE_WORDS({"FOR", "IF", "FUN", "LAMBDA"});

}


#endif // CONSTANTS_H
