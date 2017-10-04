#ifndef QUTS_DOC_H
#define QUTS_DOC_H
#include<QHash>
#include<QMap>
#include<QVariant>



/**Generated file, please do not edit**/



namespace Quts{
const QHash<QString, QMap<QString, QVariant>> qutsDoc = {
	{"CORE/MAXINT", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Maximum value")}
	}},
	{"CORE/MININT", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Minimimum value")}
	}},
	{"CORE/DEBUG", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Print debug info")}
	}},
	{"CORE/BREAKPOINTS", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"CORE/PRINTPREFIX", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("prefix for each print command")}
	}},
	{"CORE/COMMANDSPEED", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Command calling speed")}
	}},
	{"CORE/USE", {
		{"definition", QVariant("name")},
		{"parameters", QVariant(QStringList({"name of subsystem"}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("import subsystem")}
	}},
	{"CORE/LOAD", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("as import but not called before the call")}
	}},
	{"CORE/IMPORT", {
		{"definition", QVariant("filename <name>")},
		{"parameters", QVariant(QStringList({"filename that can be used to load script.","name that is used to refer, if not given, a basename of filename"}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Load another script into this script so they share stack and registers. Another script and its subroutines can then be called using CALL")}
	}},
	{"CORE/CALL", {
		{"definition", QVariant("name")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Call subroutine, subroutines are sections starting with &")}
	}},
	{"CORE/PUKE", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("print out internals")}
	}},
	{"CORE/LET", {
		{"definition", QVariant("variableName <value>")},
		{"parameters", QVariant(QStringList({"variable name","value, if not set a variable is destroyed"}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Write value to variables")}
	}},
	{"CORE/PUSH", {
		{"definition", QVariant("<values>")},
		{"parameters", QVariant(QStringList({"if empty current value (if exists) is copied on stack."}))}, 
		{"returnValue", QVariant("Pushed values so that first value is on top of stack.")},
		{"comment", QVariant("Copy values on top of stack")}
	}},
	{"CORE/POP", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"CORE/SWAP", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({"nth value, if not given swap second on top"}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Bring nth value on top stack <num>")}
	}},
	{"CORE/IF", {
		{"definition", QVariant("condition")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Createas a conditional scope")}
	}},
	{"CORE/RETURN", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"CORE/ELSE", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"CORE/EVAL", {
		{"definition", QVariant("<value> <param and return value list>")},
		{"parameters", QVariant(QStringList({"value is how many lines from stack are consumed as command lines, a list or a string"}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Executes parameters as commads")}
	}},
	{"CORE/NOT", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"CORE/TEST", {
		{"definition", QVariant("<valueList>")},
		{"parameters", QVariant(QStringList({"value list, for each value one value is popped from stack and compared, if no values are given it if stack is empty"}))}, 
		{"returnValue", QVariant("True if all compares match, otherwise false")},
		{"comment", QVariant("Compare values")}
	}},
	{"CORE/VOID", {
		{"definition", QVariant("<value>")},
		{"parameters", QVariant(QStringList({"value if not available is read from stack set amount of values read from stack."}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Empty items from stack")}
	}},
	{"CORE/ERROR", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"CORE/ASSERT", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"CORE/FOR", {
		{"definition", QVariant("<count> <index>")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"CORE/HELP", {
		{"definition", QVariant("<name>")},
		{"parameters", QVariant(QStringList({"name of topic spesific information is requested."}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Print information, without a parameter prints prints available topics.")}
	}},
	{"CORE/", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"CORE/VAR", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"CORE/INC", {
		{"definition", QVariant("<values>")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("result")},
		{"comment", QVariant("increment by values")}
	}}
};
}//namespace Quts



#ifndef QUTSLIB
extern "C" void* QUTS_EXPORT_FUNCTION HELP_FUNCTION() {return new QHash<QString, QMap<QString, QVariant>>(Quts::qutsDoc);}
#endif


#endif
