# Quts API
###	Assert
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Breakpoints
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Call
<code>"name"</code>
Parameters:	
Return:	""
Comments:	"Call subroutine, subroutines are sections starting with &"


###	Commandspeed
<code>""</code>
Parameters:	
Return:	""
Comments:	"Command calling speed"


###	Debug
<code>""</code>
Parameters:	
Return:	""
Comments:	"Print debug info"


###	Else
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Error
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Eval
<code>"<value> <param and return value list>"</code>
Parameters:	"value is how many lines from stack are consumed as command lines, a list or a string"
Return:	""
Comments:	"Executes parameters as commads"


###	For
<code>"<count> <index>"</code>
Parameters:	
Return:	""
Comments:	""


###	Help
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	If
<code>"condition"</code>
Parameters:	
Return:	""
Comments:	"Createas a conditional scope"


###	Import
<code>"filename <name>"</code>
Parameters:	"filename that can be used to load script.","name that is used to refer, if not given, a basename of filename"
Return:	""
Comments:	"Load another script into this script so they share stack and registers. Another script and its subroutines can then be called using CALL"


###	Inc
<code>"<values>"</code>
Parameters:	
Return:	"result"
Comments:	"increment by values"


###	Let
<code>"variableName <value>"</code>
Parameters:	"variable name","value, if not set a variable is destroyed"
Return:	""
Comments:	"Write value to variables"


###	Load
<code>""</code>
Parameters:	
Return:	""
Comments:	"as import but not called before the call"


###	Maxint
<code>""</code>
Parameters:	
Return:	""
Comments:	"Maximum value"


###	Minint
<code>""</code>
Parameters:	
Return:	""
Comments:	"Minimimum value"


###	Not
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Pop
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Printprefix
<code>""</code>
Parameters:	
Return:	""
Comments:	"prefix for each print command"


###	Puke
<code>""</code>
Parameters:	
Return:	""
Comments:	"print out internals"


###	Push
<code>"<values>"</code>
Parameters:	"if empty current value (if exists) is copied on stack."
Return:	"Pushed values so that first value is on top of stack."
Comments:	"Copy values on top of stack"


###	Return
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Swap
<code>""</code>
Parameters:	"nth value, if not given swap second on top"
Return:	""
Comments:	"Bring nth value on top stack <num>"


###	Test
<code>"<valueList>"</code>
Parameters:	"value list, for each value one value is popped from stack and compared, if no values are given it if stack is empty"
Return:	"True if all compares match, otherwise false"
Comments:	"Compare values"


###	Use
<code>"name"</code>
Parameters:	"name of subsystem"
Return:	""
Comments:	"import subsystem"


###	Var
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Void
<code>"<value>"</code>
Parameters:	"value if not available is read from stack set amount of values read from stack."
Return:	""
Comments:	"Empty items from stack"


##Audio
###	Beep
<code>"notes <duration> <octave>"</code>
Parameters:	"notes := notes note; note = [CDEFGAB][#b]?\\d* and d 1, 2, 4, 8, or 16 (default 4)","duration of each note in ms","octave where notes are played"
Return:	""
Comments:	"Play Notes"


###	Wait
<code>""</code>
Parameters:	
Return:	""
Comments:	"Wait play to complete"


##Core
##Float
###	Acos
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Asin
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Assert
<code>""</code>
Parameters:	
Return:	""
Comments:	"Error if two top values are non equal."


###	Atan
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Atan2
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Ceil
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Cos
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Div
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Drop
<code>""</code>
Parameters:	
Return:	""
Comments:	"remove topmost value"


###	Dup
<code>""</code>
Parameters:	
Return:	""
Comments:	"duplicate value"


###	Equal
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Exp
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Floor
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Lessthan
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Lessthanorequal
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Ln
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Morethan
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Morethanorequal
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Mul
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Over
<code>""</code>
Parameters:	
Return:	""
Comments:	"duplicate value under top on top"


###	Pop
<code>""</code>
Parameters:	
Return:	""
Comments:	"pop a value from math stack on application stack"


###	Pow
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Push
<code>""</code>
Parameters:	
Return:	""
Comments:	"pop a value from application stack into corresponding math stack."


###	Rad
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Rad
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Sin
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Sqrt
<code>""</code>
Parameters:	
Return:	""
Comments:	"Squareroot"


###	Sum
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Swap
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Tan
<code>""</code>
Parameters:	
Return:	""
Comments:	""


##Int
###	And
<code>""</code>
Parameters:	
Return:	""
Comments:	"Bitwise and"


###	Assert
<code>""</code>
Parameters:	
Return:	""
Comments:	"Error if two top values are non equal."


###	Div
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Drop
<code>""</code>
Parameters:	
Return:	""
Comments:	"remove topmost value"


###	Dup
<code>""</code>
Parameters:	
Return:	""
Comments:	"duplicate value"


###	Equal
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Lessthan
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Lessthanorequal
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Morethan
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Morethanorequal
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Mul
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Not
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Or
<code>""</code>
Parameters:	
Return:	""
Comments:	"Bitwise or"


###	Over
<code>""</code>
Parameters:	
Return:	""
Comments:	"duplicate value under top on top"


###	Pop
<code>""</code>
Parameters:	
Return:	""
Comments:	"pop a value from math stack on application stack"


###	Pow
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Push
<code>""</code>
Parameters:	
Return:	""
Comments:	"pop a value from application stack into corresponding math stack."


###	Remainder
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Sum
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Swap
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Xor
<code>""</code>
Parameters:	
Return:	""
Comments:	"Bitwise xor"


##Int, float
###	Subt
<code>""</code>
Parameters:	
Return:	""
Comments:	""


##List
###	Add
<code>""</code>
Parameters:	
Return:	"result"
Comments:	"Add items"


###	Iterate
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Len
<code>""</code>
Parameters:	
Return:	""
Comments:	"Length of list on stack"


###	List
<code>""</code>
Parameters:	
Return:	""
Comments:	""


##Qt
###	Child
<code>"<index> <list>, where list := <list item>, item := property regexp"</code>
Parameters:	"index of item in case of multiple matches. Defaults to the first.","property name of item","regexp that match to to value of property."
Return:	"number of items in match."
Comments:	"Set Focus to current item child or item its property refers to See FOCUS and PARENT"


###	Click
<code>"<x-offset y-offset>"</code>
Parameters:	
Return:	""
Comments:	"Set mouse event to middle of focused UI item, offset to that point are optional."


###	Find
<code>"name <index> <list>, where list := <list item>, item := property regexp"</code>
Parameters:	"index of item in case of multiple matches. Defaults to the first.","property name of item","regexp that match to to value of property."
Return:	"number of items in match."
Comments:	"change active \'item\', if there are multiple items, use index, defaults to 0. There may be list of property names and regexp values given used for filtering the results. Return amount of hits on top of stack. Does not change UI focus (use CLICK for that). see CHILD, PARENT, SIGNAL and CLICK."


###	Item
<code>"property <index> <list>, where list := <list item>, item := property regexp"</code>
Parameters:	"property name","index if property is a list, hash or map","list of property regep pairs, see FOCUS and CHILD"
Return:	"number of matches"
Comments:	"Set Focus to item that is currently selected item property."


###	Parent
<code>""</code>
Parameters:	
Return:	"Boolean due found."
Comments:	"Set Focus to current item parent."


###	Qutssignal
<code>"signal_name <parameter_list>"</code>
Parameters:	"sign_name is string to idententify this signal in the slot, followed by parameters"
Return:	""
Comments:	"Emits a QutsAPI signal \"qutsSignal\""


###	Signal
<code>"signal_name <parameter_list>"</code>
Parameters:	"signal_name (without parameters or brackets)"
Return:	""
Comments:	"Emit a signal to focused object"


###	Slot
<code>""</code>
Parameters:	
Return:	""
Comments:	""


##Store
###	Container
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Contains
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Read
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Write
<code>""</code>
Parameters:	
Return:	""
Comments:	""


##String
###	Add
<code>""</code>
Parameters:	
Return:	"result"
Comments:	"concanate items"


###	Len
<code>""</code>
Parameters:	
Return:	""
Comments:	"Length of string on stack"


###	List
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Match
<code>"regexp <string>"</code>
Parameters:	"value to match, if omitted, a value is taken from stack"
Return:	"true or false"
Comments:	"a regular expression"


###	Substr
<code>""</code>
Parameters:	
Return:	""
Comments:	"Get a substring "


##Time
###	Delta
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Get
<code>""</code>
Parameters:	
Return:	""
Comments:	""


###	Repeater
<code>""</code>
Parameters:	
Return:	""
Comments:	"TIME"


###	Sleep
<code>"number"</code>
Parameters:	"milliseconds to sleep"
Return:	""
Comments:	"Sleep milliseconds"


