#ifndef QUTS_DOC_H
#define QUTS_DOC_H
#include<QHash>
#include<QMap>
#include<QVariant>



/**Generated file, please do not edit**/



namespace Quts{
const QHash<QString, QMap<QString, QVariant>> qutsDoc = {
	{"LIST/ADD", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("result")},
		{"comment", QVariant("Add items")}
	}},
	{"STRING/ADD", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("result")},
		{"comment", QVariant("concanate items")}
	}},
	{"STRING/MATCH", {
		{"definition", QVariant("regexp <string>")},
		{"parameters", QVariant(QStringList({"value to match, if omitted, a value is taken from stack"}))}, 
		{"returnValue", QVariant("true or false")},
		{"comment", QVariant("a regular expression")}
	}},
	{"STRING/SUBSTR", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Get a substring ")}
	}},
	{"STRING/LEN", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Length of string on stack")}
	}},
	{"LIST/LEN", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Length of list on stack")}
	}},
	{"LIST/ITERATE", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"LIST/LIST", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"STRING/LIST", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}}
};
}//namespace Quts



#ifndef QUTSLIB
extern "C" void* QUTS_EXPORT_FUNCTION HELP_FUNCTION() {return new QHash<QString, QMap<QString, QVariant>>(Quts::qutsDoc);}
#endif


#endif
