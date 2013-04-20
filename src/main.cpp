#include <bb/cascades/Application>

#include <QtCore/QLocale>
#include <QtCore/QTranslator>

#include <Qt/qdeclarativedebug.h>

#include "permtest.hpp"

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString("PermTest_%1").arg( locale_string );
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    new PermTest(&app);

    return Application::exec();
}
