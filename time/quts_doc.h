#ifndef QUTS_DOC_H
#define QUTS_DOC_H
#include<QHash>
#include<QMap>
#include<QVariant>



/**Generated file, please do not edit**/



namespace Quts{
const QHash<QString, QMap<QString, QVariant>> qutsDoc = {
	{"TIME/SLEEP", {
		{"definition", QVariant("number")},
		{"parameters", QVariant(QStringList({"milliseconds to sleep"}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Sleep milliseconds")}
	}},
	{"TIME/GET", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"TIME/DELTA", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"TIME/REPEATER", {
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
