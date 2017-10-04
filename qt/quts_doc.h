#ifndef QUTS_DOC_H
#define QUTS_DOC_H
#include<QHash>
#include<QMap>
#include<QVariant>



/**Generated file, please do not edit**/



namespace Quts{
const QHash<QString, QMap<QString, QVariant>> qutsDoc = {
	{"QT/CLICK", {
		{"definition", QVariant("<x-offset y-offset>")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Set mouse event to middle of focused UI item, offset to that point are optional.")}
	}},
	{"QT/SLOT", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("")}
	}},
	{"QT/FIND", {
		{"definition", QVariant("name <index> <list>, where list := <list item>, item := property regexp")},
		{"parameters", QVariant(QStringList({"index of item in case of multiple matches. Defaults to the first.","property name of item","regexp that match to to value of property."}))}, 
		{"returnValue", QVariant("number of items in match.")},
		{"comment", QVariant("change active \'item\', if there are multiple items, use index, defaults to 0. There may be list of property names and regexp values given used for filtering the results. Return amount of hits on top of stack. Does not change UI focus (use CLICK for that). see CHILD, PARENT, SIGNAL and CLICK.")}
	}},
	{"QT/PARENT", {
		{"definition", QVariant("")},
		{"parameters", QVariant(QStringList({}))}, 
		{"returnValue", QVariant("Boolean due found.")},
		{"comment", QVariant("Set Focus to current item parent.")}
	}},
	{"QT/CHILD", {
		{"definition", QVariant("<index> <list>, where list := <list item>, item := property regexp")},
		{"parameters", QVariant(QStringList({"index of item in case of multiple matches. Defaults to the first.","property name of item","regexp that match to to value of property."}))}, 
		{"returnValue", QVariant("number of items in match.")},
		{"comment", QVariant("Set Focus to current item child or item its property refers to See FOCUS and PARENT")}
	}},
	{"QT/ITEM", {
		{"definition", QVariant("property <index> <list>, where list := <list item>, item := property regexp")},
		{"parameters", QVariant(QStringList({"property name","index if property is a list, hash or map","list of property regep pairs, see FOCUS and CHILD"}))}, 
		{"returnValue", QVariant("number of matches")},
		{"comment", QVariant("Set Focus to item that is currently selected item property.")}
	}},
	{"QT/QUTSSIGNAL", {
		{"definition", QVariant("signal_name <parameter_list>")},
		{"parameters", QVariant(QStringList({"sign_name is string to idententify this signal in the slot, followed by parameters"}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Emits a QutsAPI signal \"qutsSignal\"")}
	}},
	{"QT/SIGNAL", {
		{"definition", QVariant("signal_name <parameter_list>")},
		{"parameters", QVariant(QStringList({"signal_name (without parameters or brackets)"}))}, 
		{"returnValue", QVariant("")},
		{"comment", QVariant("Emit a signal to focused object")}
	}}
};
}//namespace Quts



#ifndef QUTSLIB
extern "C" void* QUTS_EXPORT_FUNCTION HELP_FUNCTION() {return new QHash<QString, QMap<QString, QVariant>>(Quts::qutsDoc);}
#endif


#endif
