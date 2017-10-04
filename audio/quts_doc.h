#ifndef QUTS_DOC_H
#define QUTS_DOC_H
#include<QHash>
#include<QMap>
#include<QVariant>



/**Generated file, please do not edit**/



namespace Quts{
const QHash<QString, QMap<QString, QVariant>> qutsDoc = {
	{"AUDIO/BEEP", {
		{"definition", QVariant("notes <duration> <octave>")},
		{"parameters", QVariant(QStringList({"notes := notes note; note = [CDEFGAB][#b]?\\d* and d 1, 2, 4, 8, or 16 (default 4)","duration of each note in ms","octave where notes are played"}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Play Notes")}
	}},
	{"AUDIO/WAIT", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Wait play to complete")}
	}}
};
}//namespace Quts



#ifndef QUTSLIB
extern "C" void* QUTS_EXPORT_FUNCTION HELP_FUNCTION() {return new QHash<QString, QMap<QString, QVariant>>(Quts::qutsDoc);}
#endif


#endif
