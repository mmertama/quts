[//]: # (Generated file, please do not edit)  
# Quts API  
Quts core functions  
[AUDIO.Beep](#audiobeep)  
[AUDIO.Wait](#audiowait)  
[Assert](#coreassert)  
[Breakpoints](#corebreakpoints)  
[Call](#corecall)  
[Commandspeed](#corecommandspeed)  
[Debug](#coredebug)  
[Else](#coreelse)  
[Error](#coreerror)  
[Eval](#coreeval)  
[For](#corefor)  
[Help](#corehelp)  
[If](#coreif)  
[Import](#coreimport)  
[Inc](#coreinc)  
[Let](#corelet)  
[Load](#coreload)  
[Maxint](#coremaxint)  
[Minint](#coreminint)  
[Not](#corenot)  
[Pop](#corepop)  
[Printprefix](#coreprintprefix)  
[Puke](#corepuke)  
[Push](#corepush)  
[Return](#corereturn)  
[Swap](#coreswap)  
[Test](#coretest)  
[Use](#coreuse)  
[Var](#corevar)  
[Void](#corevoid)  
[FLOAT.Acos](#floatacos)  
[FLOAT.Asin](#floatasin)  
[FLOAT.Assert](#floatassert)  
[FLOAT.Atan](#floatatan)  
[FLOAT.Atan2](#floatatan2)  
[FLOAT.Ceil](#floatceil)  
[FLOAT.Cos](#floatcos)  
[FLOAT.Div](#floatdiv)  
[FLOAT.Drop](#floatdrop)  
[FLOAT.Dup](#floatdup)  
[FLOAT.Equal](#floatequal)  
[FLOAT.Exp](#floatexp)  
[FLOAT.Floor](#floatfloor)  
[FLOAT.Lessthan](#floatlessthan)  
[FLOAT.Lessthanorequal](#floatlessthanorequal)  
[FLOAT.Ln](#floatln)  
[FLOAT.Morethan](#floatmorethan)  
[FLOAT.Morethanorequal](#floatmorethanorequal)  
[FLOAT.Mul](#floatmul)  
[FLOAT.Over](#floatover)  
[FLOAT.Pop](#floatpop)  
[FLOAT.Pow](#floatpow)  
[FLOAT.Push](#floatpush)  
[FLOAT.Rad](#floatrad)  
[FLOAT.Rad](#floatrad)  
[FLOAT.Sin](#floatsin)  
[FLOAT.Sqrt](#floatsqrt)  
[FLOAT.Subt](#floatsubt)  
[FLOAT.Subt](#floatsubt)  
[FLOAT.Sum](#floatsum)  
[FLOAT.Swap](#floatswap)  
[FLOAT.Tan](#floattan)  
[INT.And](#intand)  
[INT.Assert](#intassert)  
[INT.Div](#intdiv)  
[INT.Drop](#intdrop)  
[INT.Dup](#intdup)  
[INT.Equal](#intequal)  
[INT.Lessthan](#intlessthan)  
[INT.Lessthanorequal](#intlessthanorequal)  
[INT.Morethan](#intmorethan)  
[INT.Morethanorequal](#intmorethanorequal)  
[INT.Mul](#intmul)  
[INT.Not](#intnot)  
[INT.Or](#intor)  
[INT.Over](#intover)  
[INT.Pop](#intpop)  
[INT.Pow](#intpow)  
[INT.Push](#intpush)  
[INT.Remainder](#intremainder)  
[INT.Sum](#intsum)  
[INT.Swap](#intswap)  
[INT.Xor](#intxor)  
[LIST.Add](#listadd)  
[LIST.Iterate](#listiterate)  
[LIST.Len](#listlen)  
[LIST.List](#listlist)  
[QT.Child](#qtchild)  
[QT.Click](#qtclick)  
[QT.Find](#qtfind)  
[QT.Item](#qtitem)  
[QT.Parent](#qtparent)  
[QT.Qutssignal](#qtqutssignal)  
[QT.Signal](#qtsignal)  
[QT.Slot](#qtslot)  
[STORE.Container](#storecontainer)  
[STORE.Contains](#storecontains)  
[STORE.Read](#storeread)  
[STORE.Write](#storewrite)  
[STRING.Add](#stringadd)  
[STRING.Len](#stringlen)  
[STRING.List](#stringlist)  
[STRING.Match](#stringmatch)  
[STRING.Substr](#stringsubstr)  
[TIME.Delta](#timedelta)  
[TIME.Get](#timeget)  
[TIME.Repeater](#timerepeater)  
[TIME.Sleep](#timesleep)  
<a name="coreassert"></a>  
### Assert  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="corebreakpoints"></a>  
### Breakpoints  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="corecall"></a>  
### Call  
<code>name</code>  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Call subroutine, subroutines are sections starting with &  

  
<a name="corecommandspeed"></a>  
### Commandspeed  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Command calling speed  

  
<a name="coredebug"></a>  
### Debug  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Print debug info  

  
<a name="coreelse"></a>  
### Else  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="coreerror"></a>  
### Error  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="coreeval"></a>  
### Eval  
<code><value> <param and return value list></code>  
<b>Parameters:</b>	"value is how many lines from stack are consumed as command lines, a list or a string"  
<b>Return:</b>	None  
Executes parameters as commads  

  
<a name="corefor"></a>  
### For  
<code><count> <index></code>  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="corehelp"></a>  
### Help  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="coreif"></a>  
### If  
<code>condition</code>  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Createas a conditional scope  

  
<a name="coreimport"></a>  
### Import  
<code>filename <name></code>  
<b>Parameters:</b>	"filename that can be used to load script.","name that is used to refer, if not given, a basename of filename"  
<b>Return:</b>	None  
Load another script into this script so they share stack and registers. Another script and its subroutines can then be called using CALL  

  
<a name="coreinc"></a>  
### Inc  
<code><values></code>  
<b>Parameters:</b>	None  
<b>Return:</b>	result  
increment by values  

  
<a name="corelet"></a>  
### Let  
<code>variableName <value></code>  
<b>Parameters:</b>	"variable name","value, if not set a variable is destroyed"  
<b>Return:</b>	None  
Write value to variables  

  
<a name="coreload"></a>  
### Load  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
as import but not called before the call  

  
<a name="coremaxint"></a>  
### Maxint  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Maximum value  

  
<a name="coreminint"></a>  
### Minint  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Minimimum value  

  
<a name="corenot"></a>  
### Not  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="corepop"></a>  
### Pop  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="coreprintprefix"></a>  
### Printprefix  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
prefix for each print command  

  
<a name="corepuke"></a>  
### Puke  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
print out internals  

  
<a name="corepush"></a>  
### Push  
<code><values></code>  
<b>Parameters:</b>	"if empty current value (if exists) is copied on stack."  
<b>Return:</b>	Pushed values so that first value is on top of stack.  
Copy values on top of stack  

  
<a name="corereturn"></a>  
### Return  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="coreswap"></a>  
### Swap  
<b>Parameters:</b>	"nth value, if not given swap second on top"  
<b>Return:</b>	None  
Bring nth value on top stack <num>  

  
<a name="coretest"></a>  
### Test  
<code><valueList></code>  
<b>Parameters:</b>	"value list, for each value one value is popped from stack and compared, if no values are given it if stack is empty"  
<b>Return:</b>	True if all compares match, otherwise false  
Compare values  

  
<a name="coreuse"></a>  
### Use  
<code>name</code>  
<b>Parameters:</b>	"name of subsystem"  
<b>Return:</b>	None  
import subsystem  

  
<a name="corevar"></a>  
### Var  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="corevoid"></a>  
### Void  
<code><value></code>  
<b>Parameters:</b>	"value if not available is read from stack set amount of values read from stack."  
<b>Return:</b>	None  
Empty items from stack  

  
## Audio  
<a name="audiobeep"></a>  
### Beep  
<code>notes <duration> <octave></code>  
<b>Parameters:</b>	"notes := notes note; note = [CDEFGAB][#b]?\\d* and d 1, 2, 4, 8, or 16 (default 4)","duration of each note in ms","octave where notes are played"  
<b>Return:</b>	None  
Play Notes  

  
<a name="audiowait"></a>  
### Wait  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Wait play to complete  

  
## Float  
<a name="floatacos"></a>  
### Acos  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatasin"></a>  
### Asin  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatassert"></a>  
### Assert  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Error if two top values are non equal.  

  
<a name="floatatan"></a>  
### Atan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatatan2"></a>  
### Atan2  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatceil"></a>  
### Ceil  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatcos"></a>  
### Cos  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatdiv"></a>  
### Div  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatdrop"></a>  
### Drop  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
remove topmost value  

  
<a name="floatdup"></a>  
### Dup  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
duplicate value  

  
<a name="floatequal"></a>  
### Equal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatexp"></a>  
### Exp  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatfloor"></a>  
### Floor  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatlessthan"></a>  
### Lessthan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatlessthanorequal"></a>  
### Lessthanorequal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatln"></a>  
### Ln  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatmorethan"></a>  
### Morethan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatmorethanorequal"></a>  
### Morethanorequal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatmul"></a>  
### Mul  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatover"></a>  
### Over  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
duplicate value under top on top  

  
<a name="floatpop"></a>  
### Pop  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
pop a value from math stack on application stack  

  
<a name="floatpow"></a>  
### Pow  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatpush"></a>  
### Push  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
pop a value from application stack into corresponding math stack.  

  
<a name="floatrad"></a>  
### Rad  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatrad"></a>  
### Rad  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatsin"></a>  
### Sin  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatsqrt"></a>  
### Sqrt  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Squareroot  

  
<a name="floatsubt"></a>  
### Subt  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Subtract  

  
<a name="floatsubt"></a>  
### Subt  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Subtract  

  
<a name="floatsum"></a>  
### Sum  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floatswap"></a>  
### Swap  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="floattan"></a>  
### Tan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
## Int  
<a name="intand"></a>  
### And  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Bitwise and  

  
<a name="intassert"></a>  
### Assert  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Error if two top values are non equal.  

  
<a name="intdiv"></a>  
### Div  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intdrop"></a>  
### Drop  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
remove topmost value  

  
<a name="intdup"></a>  
### Dup  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
duplicate value  

  
<a name="intequal"></a>  
### Equal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intlessthan"></a>  
### Lessthan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intlessthanorequal"></a>  
### Lessthanorequal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intmorethan"></a>  
### Morethan  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intmorethanorequal"></a>  
### Morethanorequal  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intmul"></a>  
### Mul  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intnot"></a>  
### Not  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intor"></a>  
### Or  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Bitwise or  

  
<a name="intover"></a>  
### Over  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
duplicate value under top on top  

  
<a name="intpop"></a>  
### Pop  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
pop a value from math stack on application stack  

  
<a name="intpow"></a>  
### Pow  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intpush"></a>  
### Push  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
pop a value from application stack into corresponding math stack.  

  
<a name="intremainder"></a>  
### Remainder  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intsum"></a>  
### Sum  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intswap"></a>  
### Swap  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="intxor"></a>  
### Xor  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Bitwise xor  

  
## List  
<a name="listadd"></a>  
### Add  
<b>Parameters:</b>	None  
<b>Return:</b>	result  
Add items  

  
<a name="listiterate"></a>  
### Iterate  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="listlen"></a>  
### Len  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Length of list on stack  

  
<a name="listlist"></a>  
### List  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
## Qt  
<a name="qtchild"></a>  
### Child  
<code><index> <list>, where list := <list item>, item := property regexp</code>  
<b>Parameters:</b>	"index of item in case of multiple matches. Defaults to the first.","property name of item","regexp that match to to value of property."  
<b>Return:</b>	number of items in match.  
Set Focus to current item child or item its property refers to See FOCUS and PARENT  

  
<a name="qtclick"></a>  
### Click  
<code><x-offset y-offset></code>  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Set mouse event to middle of focused UI item, offset to that point are optional.  

  
<a name="qtfind"></a>  
### Find  
<code>name <index> <list>, where list := <list item>, item := property regexp</code>  
<b>Parameters:</b>	"index of item in case of multiple matches. Defaults to the first.","property name of item","regexp that match to to value of property."  
<b>Return:</b>	number of items in match.  
change active \'item\', if there are multiple items, use index, defaults to 0. There may be list of property names and regexp values given used for filtering the results. Return amount of hits on top of stack. Does not change UI focus (use CLICK for that). see CHILD, PARENT, SIGNAL and CLICK.  

  
<a name="qtitem"></a>  
### Item  
<code>property <index> <list>, where list := <list item>, item := property regexp</code>  
<b>Parameters:</b>	"property name","index if property is a list, hash or map","list of property regep pairs, see FOCUS and CHILD"  
<b>Return:</b>	number of matches  
Set Focus to item that is currently selected item property.  

  
<a name="qtparent"></a>  
### Parent  
<b>Parameters:</b>	None  
<b>Return:</b>	Boolean due found.  
Set Focus to current item parent.  

  
<a name="qtqutssignal"></a>  
### Qutssignal  
<code>signal_name <parameter_list></code>  
<b>Parameters:</b>	"sign_name is string to idententify this signal in the slot, followed by parameters"  
<b>Return:</b>	None  
Emits a QutsAPI signal \"qutsSignal\"  

  
<a name="qtsignal"></a>  
### Signal  
<code>signal_name <parameter_list></code>  
<b>Parameters:</b>	"signal_name (without parameters or brackets)"  
<b>Return:</b>	None  
Emit a signal to focused object  

  
<a name="qtslot"></a>  
### Slot  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
## Store  
<a name="storecontainer"></a>  
### Container  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="storecontains"></a>  
### Contains  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="storeread"></a>  
### Read  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="storewrite"></a>  
### Write  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
## String  
<a name="stringadd"></a>  
### Add  
<b>Parameters:</b>	None  
<b>Return:</b>	result  
concanate items  

  
<a name="stringlen"></a>  
### Len  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Length of string on stack  

  
<a name="stringlist"></a>  
### List  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="stringmatch"></a>  
### Match  
<code>regexp <string></code>  
<b>Parameters:</b>	"value to match, if omitted, a value is taken from stack"  
<b>Return:</b>	true or false  
a regular expression  

  
<a name="stringsubstr"></a>  
### Substr  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Get a substring   

  
## Time  
<a name="timedelta"></a>  
### Delta  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="timeget"></a>  
### Get  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="timerepeater"></a>  
### Repeater  
<b>Parameters:</b>	None  
<b>Return:</b>	None  
Documentation missing  

  
<a name="timesleep"></a>  
### Sleep  
<code>number</code>  
<b>Parameters:</b>	"milliseconds to sleep"  
<b>Return:</b>	None  
Sleep milliseconds  

  
