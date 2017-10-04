#ifndef SUBSYSTEM_TEMPLATE_H
#define SUBSYSTEM_TEMPLATE_H



template <class T>
void LoopingSubsystem<T>::clear(){
    m_iterators.clear();
}

template <class T>
bool LoopingSubsystem<T>::loop(const QStringList& values, std::function<bool (IInfo&) > looper){
    MIN_ARG(1)
    if(m_iterators.isEmpty() || m_iterators.top().ipos != m_engine.lineId()) {
        bool ok = true;
        const auto v = values.length() > 1 ? getValue(values[0]) : m_engine.popc(&ok);
        if(!ok) {
            return false;
        }
        const auto it = getValue(values[values.length() > 1 ? 1 : 0], &ok, QutsEngine::CALLPARAM);
        if(!ok) {
            return false;
        }

        Closure c;
        if(it.type() == QVariant::String) {
            c.first = Quts::makeGlobalName(it.toString(), m_engine.current());
        } else {
            if(!it.template canConvert<Closure>()) {
                return syntaxError(INVALID_ARGUMENT);
            }
            c = it.template value<Closure>();
        }
        if(!v.template canConvert<T>()) {
            return syntaxError(INVALID_ARGUMENT);
        }
        m_iterators.push({m_engine.lineId(), 0, c, looper, v.template value<T>()});
    }
    return true;
}

template <class T>
bool LoopingSubsystem<T>::run(){
    auto& top = m_iterators.top();
    return top.lambda(top);
}

template <class T>
bool IterableSubsystem<T>::iterate(const QStringList& values){
    auto ok = true;
    const QVariantList freeParams = map<QVariant, QString>(values.mid(2), [this, &ok](const QString& s){
        return ok ? this->getValue(s, &ok) : QVariant();});
    if(!ok)
        return this->syntaxError(INVALID_ARGUMENT);
    ok = this->loop(values, [this, freeParams](typename LoopingSubsystem<T>::IInfo& top){
        if(this->m_engine.lineId() == top.ipos) {
            if(top.index < top.data.length()) {
                QVariantList params({top.data.at(top.index)});
                params.append(freeParams);
                if(!this->m_engine.callLambda(top.closure,
                params,
                top.index < top.data.length() - 1,
                [this](const bool stepValue) {
                    if(!stepValue) {
                        this->m_iterators.pop();
                    }
                })) {
                    this->m_iterators.pop();
                    return false;
                }
                ++top.index;
            }
        }
        return true;
    });
    return ok && this->run();
}


#endif // SUBSYSTEM_TEMPLATE_H
