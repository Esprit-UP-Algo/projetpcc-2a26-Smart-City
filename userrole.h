#ifndef USERROLE_H
#define USERROLE_H

#include <QString>

enum class UserRole {
    ADMIN,           // All pages access (smart city / 0000)
    RESIDENT,        // Only Residents page (resident / 0000)
    FINANCE,         // Only Finance page (finance / 0000)
    INCIDENT,        // Only Incidents page (incident / 0000)
    TRANSPORT,       // Only Transports page (transport / 0000)
    LOCAUX,          // Only Locaux page (locaux / 0000)
    ADMIN_SETTINGS,  // Only Paramètres page (admin / 0000)
    NONE             // No access
};

struct UserInfo {
    QString email;
    QString password;
    UserRole role;
    QString displayName;
};

#endif // USERROLE_H
