#ifndef QUTS_DOC_H
#define QUTS_DOC_H
#include<QHash>
#include<QMap>
#include<QVariant>



/**Generated file, please do not edit**/



namespace Quts{
const QHash<QString, QMap<QString, QVariant>> qutsDoc = {
};
}//namespace Quts



#ifndef QUTSLIB
extern "C" void* QUTS_EXPORT_FUNCTION HELP_FUNCTION() {return new QHash<QString, QMap<QString, QVariant>>(Quts::qutsDoc);}
#endif


#endif
