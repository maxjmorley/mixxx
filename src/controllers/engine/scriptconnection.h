#pragma once

#include <QJSValue>
#include <QUuid>

#include "preferences/configobject.h"

class ControllerEngine;
class ControllerEngineJSProxy;

/// ScriptConnection is a connection between a ControlObject and a
/// script callback function that gets executed when the value
/// of the ControlObject changes.
class ScriptConnection {
  public:
    ConfigKey key;
    QUuid id;
    QJSValue callback;
    ControllerEngineJSProxy* engineJSProxy;
    ControllerEngine* controllerEngine;

    void executeCallback(double value) const;

    // Required for various QList methods and iteration to work.
    inline bool operator==(const ScriptConnection& other) const {
        return id == other.id;
    }
    inline bool operator!=(const ScriptConnection& other) const {
        return !(*this == other);
    }
};
