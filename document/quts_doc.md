# Quts API  
<!--Generated file, please do not edit!-->  
Quts core functions  
[AUDIO.Beep][audiobeep]  
[AUDIO.Wait][audiowait]  
[Assert][coreassert]  
[Breakpoints][corebreakpoints]  
[Call][corecall]  
[Commandspeed][corecommandspeed]  
[Debug][coredebug]  
[Else][coreelse]  
[Error][coreerror]  
[Eval][coreeval]  
[For][corefor]  
[Help][corehelp]  
[If][coreif]  
[Import][coreimport]  
[Inc][coreinc]  
[Let][corelet]  
[Load][coreload]  
[Maxint][coremaxint]  
[Minint][coreminint]  
[Not][corenot]  
[Pop][corepop]  
[Printprefix][coreprintprefix]  
[Puke][corepuke]  
[Push][corepush]  
[Return][corereturn]  
[Swap][coreswap]  
[Test][coretest]  
[Use][coreuse]  
[Var][corevar]  
[Void][corevoid]  
[FLOAT.Acos][floatacos]  
[FLOAT.Asin][floatasin]  
[FLOAT.Assert][floatassert]  
[FLOAT.Atan][floatatan]  
[FLOAT.Atan2][floatatan2]  
[FLOAT.Ceil][floatceil]  
[FLOAT.Cos][floatcos]  
[FLOAT.Div][floatdiv]  
[FLOAT.Drop][floatdrop]  
[FLOAT.Dup][floatdup]  
[FLOAT.Equal][floatequal]  
[FLOAT.Exp][floatexp]  
[FLOAT.Floor][floatfloor]  
[FLOAT.Lessthan][floatlessthan]  
[FLOAT.Lessthanorequal][floatlessthanorequal]  
[FLOAT.Ln][floatln]  
[FLOAT.Morethan][floatmorethan]  
[FLOAT.Morethanorequal][floatmorethanorequal]  
[FLOAT.Mul][floatmul]  
[FLOAT.Over][floatover]  
[FLOAT.Pop][floatpop]  
[FLOAT.Pow][floatpow]  
[FLOAT.Push][floatpush]  
[FLOAT.Rad][floatrad]  
[FLOAT.Rad][floatrad]  
[FLOAT.Sin][floatsin]  
[FLOAT.Sqrt][floatsqrt]  
[FLOAT.Subt][floatsubt]  
[FLOAT.Subt][floatsubt]  
[FLOAT.Sum][floatsum]  
[FLOAT.Swap][floatswap]  
[FLOAT.Tan][floattan]  
[INT.And][intand]  
[INT.Assert][intassert]  
[INT.Div][intdiv]  
[INT.Drop][intdrop]  
[INT.Dup][intdup]  
[INT.Equal][intequal]  
[INT.Lessthan][intlessthan]  
[INT.Lessthanorequal][intlessthanorequal]  
[INT.Morethan][intmorethan]  
[INT.Morethanorequal][intmorethanorequal]  
[INT.Mul][intmul]  
[INT.Not][intnot]  
[INT.Or][intor]  
[INT.Over][intover]  
[INT.Pop][intpop]  
[INT.Pow][intpow]  
[INT.Push][intpush]  
[INT.Remainder][intremainder]  
[INT.Sum][intsum]  
[INT.Swap][intswap]  
[INT.Xor][intxor]  
[LIST.Add][listadd]  
[LIST.Iterate][listiterate]  
[LIST.Len][listlen]  
[LIST.List][listlist]  
[QT.Child][qtchild]  
[QT.Click][qtclick]  
[QT.Find][qtfind]  
[QT.Item][qtitem]  
[QT.Parent][qtparent]  
[QT.Qutssignal][qtqutssignal]  
[QT.Signal][qtsignal]  
[QT.Slot][qtslot]  
[STORE.Container][storecontainer]  
[STORE.Contains][storecontains]  
[STORE.Read][storeread]  
[STORE.Write][storewrite]  
[STRING.Add][stringadd]  
[STRING.Len][stringlen]  
[STRING.List][stringlist]  
[STRING.Match][stringmatch]  
[STRING.Substr][stringsubstr]  
[TIME.Delta][timedelta]  
[TIME.Get][timeget]  
[TIME.Repeater][timerepeater]  
[TIME.Sleep][timesleep]  
###	Assert  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
###	Breakpoints  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
###	Call  
<code>name</code>  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Call subroutine, subroutines are sections starting with &  

  
###	Commandspeed  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Command calling speed  

  
###	Debug  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Print debug info  

  
###	Else  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
###	Error  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
###	Eval  
<code><value> <param and return value list></code>  
<b>Parameters:</b>	"value is how many lines from stack are consumed as command lines, a list or a string"  
<b>Return:</b>	None  
Executes parameters as commads  

  
###	For  
<code><count> <index></code>  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
###	Help  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
###	If  
<code>condition</code>  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Createas a conditional scope  

  
###	Import  
<code>filename <name></code>  
<b>Parameters:</b>	"filename that can be used to load script.","name that is used to refer, if not given, a basename of filename"  
<b>Return:</b>	None  
Load another script into this script so they share stack and registers. Another script and its subroutines can then be called using CALL  

  
###	Inc  
<code><values></code>  
<b>Parameters:</b>	None  
<b>Return:</b>	result  
increment by values  

  
###	Let  
<code>variableName <value></code>  
<b>Parameters:</b>	"variable name","value, if not set a variable is destroyed"  
<b>Return:</b>	None  
Write value to variables  

  
###	Load  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
as import but not called before the call  

  
###	Maxint  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Maximum value  

  
###	Minint  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Minimimum value  

  
###	Not  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
###	Pop  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
###	Printprefix  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
prefix for each print command  

  
###	Puke  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
print out internals  

  
###	Push  
<code><values></code>  
<b>Parameters:</b>	"if empty current value (if exists) is copied on stack."  
<b>Return:</b>	Pushed values so that first value is on top of stack.  
Copy values on top of stack  

  
###	Return  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
###	Swap  
<b>Parameters:</b>	"nth value, if not given swap second on top"  
<b>Return:</b>	None  
Bring nth value on top stack <num>  

  
###	Test  
<code><valueList></code>  
<b>Parameters:</b>	"value list, for each value one value is popped from stack and compared, if no values are given it if stack is empty"  
<b>Return:</b>	True if all compares match, otherwise false  
Compare values  

  
###	Use  
<code>name</code>  
<b>Parameters:</b>	"name of subsystem"  
<b>Return:</b>	None  
import subsystem  

  
###	Var  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
###	Void  
<code><value></code>  
<b>Parameters:</b>	"value if not available is read from stack set amount of values read from stack."  
<b>Return:</b>	None  
Empty items from stack  

  
[audiobeep]:#audiobeep  
##Audio  
###	Beep  
<code>notes <duration> <octave></code>  
<b>Parameters:</b>	"notes := notes note; note = [CDEFGAB][#b]?\\d* and d 1, 2, 4, 8, or 16 (default 4)","duration of each note in ms","octave where notes are played"  
<b>Return:</b>	None  
Play Notes  

  
[audiowait]:#audiowait  
###	Wait  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Wait play to complete  

  
[coreassert]:#coreassert  
##Core  
[corebreakpoints]:#corebreakpoints  
[corecall]:#corecall  
[corecommandspeed]:#corecommandspeed  
[coredebug]:#coredebug  
[coreelse]:#coreelse  
[coreerror]:#coreerror  
[coreeval]:#coreeval  
[corefor]:#corefor  
[corehelp]:#corehelp  
[coreif]:#coreif  
[coreimport]:#coreimport  
[coreinc]:#coreinc  
[corelet]:#corelet  
[coreload]:#coreload  
[coremaxint]:#coremaxint  
[coreminint]:#coreminint  
[corenot]:#corenot  
[corepop]:#corepop  
[coreprintprefix]:#coreprintprefix  
[corepuke]:#corepuke  
[corepush]:#corepush  
[corereturn]:#corereturn  
[coreswap]:#coreswap  
[coretest]:#coretest  
[coreuse]:#coreuse  
[corevar]:#corevar  
[corevoid]:#corevoid  
[floatacos]:#floatacos  
##Float  
###	Acos  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatasin]:#floatasin  
###	Asin  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatassert]:#floatassert  
###	Assert  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Error if two top values are non equal.  

  
[floatatan]:#floatatan  
###	Atan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatatan2]:#floatatan2  
###	Atan2  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatceil]:#floatceil  
###	Ceil  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatcos]:#floatcos  
###	Cos  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatdiv]:#floatdiv  
###	Div  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatdrop]:#floatdrop  
###	Drop  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
remove topmost value  

  
[floatdup]:#floatdup  
###	Dup  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
duplicate value  

  
[floatequal]:#floatequal  
###	Equal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatexp]:#floatexp  
###	Exp  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatfloor]:#floatfloor  
###	Floor  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatlessthan]:#floatlessthan  
###	Lessthan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatlessthanorequal]:#floatlessthanorequal  
###	Lessthanorequal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatln]:#floatln  
###	Ln  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatmorethan]:#floatmorethan  
###	Morethan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatmorethanorequal]:#floatmorethanorequal  
###	Morethanorequal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatmul]:#floatmul  
###	Mul  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatover]:#floatover  
###	Over  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
duplicate value under top on top  

  
[floatpop]:#floatpop  
###	Pop  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
pop a value from math stack on application stack  

  
[floatpow]:#floatpow  
###	Pow  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatpush]:#floatpush  
###	Push  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
pop a value from application stack into corresponding math stack.  

  
[floatrad]:#floatrad  
###	Rad  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatrad]:#floatrad  
###	Rad  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatsin]:#floatsin  
###	Sin  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatsqrt]:#floatsqrt  
###	Sqrt  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Squareroot  

  
[floatsubt]:#floatsubt  
###	Subt  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Subtract  

  
[floatsubt]:#floatsubt  
###	Subt  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Subtract  

  
[floatsum]:#floatsum  
###	Sum  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floatswap]:#floatswap  
###	Swap  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[floattan]:#floattan  
###	Tan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intand]:#intand  
##Int  
###	And  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Bitwise and  

  
[intassert]:#intassert  
###	Assert  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Error if two top values are non equal.  

  
[intdiv]:#intdiv  
###	Div  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intdrop]:#intdrop  
###	Drop  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
remove topmost value  

  
[intdup]:#intdup  
###	Dup  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
duplicate value  

  
[intequal]:#intequal  
###	Equal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intlessthan]:#intlessthan  
###	Lessthan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intlessthanorequal]:#intlessthanorequal  
###	Lessthanorequal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intmorethan]:#intmorethan  
###	Morethan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intmorethanorequal]:#intmorethanorequal  
###	Morethanorequal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intmul]:#intmul  
###	Mul  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intnot]:#intnot  
###	Not  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intor]:#intor  
###	Or  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Bitwise or  

  
[intover]:#intover  
###	Over  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
duplicate value under top on top  

  
[intpop]:#intpop  
###	Pop  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
pop a value from math stack on application stack  

  
[intpow]:#intpow  
###	Pow  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intpush]:#intpush  
###	Push  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
pop a value from application stack into corresponding math stack.  

  
[intremainder]:#intremainder  
###	Remainder  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intsum]:#intsum  
###	Sum  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intswap]:#intswap  
###	Swap  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[intxor]:#intxor  
###	Xor  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Bitwise xor  

  
[listadd]:#listadd  
##List  
###	Add  
<b>Parameters:</b>	None  
<b>Return:</b>	result  
Add items  

  
[listiterate]:#listiterate  
###	Iterate  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[listlen]:#listlen  
###	Len  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Length of list on stack  

  
[listlist]:#listlist  
###	List  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[qtchild]:#qtchild  
##Qt  
###	Child  
<code><index> <list>, where list := <list item>, item := property regexp</code>  
<b>Parameters:</b>	"index of item in case of multiple matches. Defaults to the first.","property name of item","regexp that match to to value of property."  
<b>Return:</b>	number of items in match.  
Set Focus to current item child or item its property refers to See FOCUS and PARENT  

  
[qtclick]:#qtclick  
###	Click  
<code><x-offset y-offset></code>  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Set mouse event to middle of focused UI item, offset to that point are optional.  

  
[qtfind]:#qtfind  
###	Find  
<code>name <index> <list>, where list := <list item>, item := property regexp</code>  
<b>Parameters:</b>	"index of item in case of multiple matches. Defaults to the first.","property name of item","regexp that match to to value of property."  
<b>Return:</b>	number of items in match.  
change active \'item\', if there are multiple items, use index, defaults to 0. There may be list of property names and regexp values given used for filtering the results. Return amount of hits on top of stack. Does not change UI focus (use CLICK for that). see CHILD, PARENT, SIGNAL and CLICK.  

  
[qtitem]:#qtitem  
###	Item  
<code>property <index> <list>, where list := <list item>, item := property regexp</code>  
<b>Parameters:</b>	"property name","index if property is a list, hash or map","list of property regep pairs, see FOCUS and CHILD"  
<b>Return:</b>	number of matches  
Set Focus to item that is currently selected item property.  

  
[qtparent]:#qtparent  
###	Parent  
<b>Parameters:</b>	None  
<b>Return:</b>	Boolean due found.  
Set Focus to current item parent.  

  
[qtqutssignal]:#qtqutssignal  
###	Qutssignal  
<code>signal_name <parameter_list></code>  
<b>Parameters:</b>	"sign_name is string to idententify this signal in the slot, followed by parameters"  
<b>Return:</b>	None  
Emits a QutsAPI signal \"qutsSignal\"  

  
[qtsignal]:#qtsignal  
###	Signal  
<code>signal_name <parameter_list></code>  
<b>Parameters:</b>	"signal_name (without parameters or brackets)"  
<b>Return:</b>	None  
Emit a signal to focused object  

  
[qtslot]:#qtslot  
###	Slot  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[storecontainer]:#storecontainer  
##Store  
###	Container  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[storecontains]:#storecontains  
###	Contains  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[storeread]:#storeread  
###	Read  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[storewrite]:#storewrite  
###	Write  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[stringadd]:#stringadd  
##String  
###	Add  
<b>Parameters:</b>	None  
<b>Return:</b>	result  
concanate items  

  
[stringlen]:#stringlen  
###	Len  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Length of string on stack  

  
[stringlist]:#stringlist  
###	List  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[stringmatch]:#stringmatch  
###	Match  
<code>regexp <string></code>  
<b>Parameters:</b>	"value to match, if omitted, a value is taken from stack"  
<b>Return:</b>	true or false  
a regular expression  

  
[stringsubstr]:#stringsubstr  
###	Substr  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Get a substring   

  
[timedelta]:#timedelta  
##Time  
###	Delta  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[timeget]:#timeget  
###	Get  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
[timerepeater]:#timerepeater  
###	Repeater  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
TIME  

  
[timesleep]:#timesleep  
###	Sleep  
<code>number</code>  
<b>Parameters:</b>	"milliseconds to sleep"  
<b>Return:</b>	None  
Sleep milliseconds  

  
