#ifndef QUTSDLL_H
#define QUTSDLL_H

#ifdef QUTS_EXPORT
# define QUTS_EXPORT_API Q_DECL_EXPORT
#else
# define QUTS_EXPORT_API Q_DECL_IMPORT
#endif

#endif // QUTSDLL_H
