#ifndef USERROLE_H
#define USERROLE_H

#include <QString>

// NEW TEACHER VERSION ROLES — SAFE REPLACEMENT
enum class UserRole {
    GENERAL_ADMIN,       // smart city / 0000 → access to everything
    SERVICE_CLIENTS,     // serviceClients / 0000 → Residents + Incidents + settings
    SERVICE_FINANCIER,   // serviceFinancier / 0000 → Finances + settings
    SERVICE_LOGISTIQUE,  // serviceLogistique / 0000 → Transport + Locaux + settings
    SERVICE_INFORMATIQUE,// serviceInformatique / 0000 → Admin DB + settings
    NONE                 // invalid login
};

struct UserInfo {
    QString email;
    QString password;
    UserRole role;
    QString displayName;
};

#endif // USERROLE_H
