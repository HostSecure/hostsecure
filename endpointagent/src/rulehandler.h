#pragma once
#include <QObject>

//!
//! \brief The RuleHandler class (Halted)
//! Purpose is for managing and distributing service rulesets,
//! Upon downstream rule or setting updates from cloud,
//! rulehandler will update applicable local security services
//!
class RuleHandler : public QObject
{
   Q_OBJECT
public:
   RuleHandler();
};

