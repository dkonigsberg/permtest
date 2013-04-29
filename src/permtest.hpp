#ifndef PERMTEST_HPP
#define PERMTEST_HPP

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <bb/PpsOpenMode>

namespace bb { namespace cascades { class Application; }}

class PermTest : public QObject
{
    Q_OBJECT
public:
    PermTest(bb::cascades::Application *app);
    virtual ~PermTest() {}
private:
    QVariantMap testDeviceIdentifyingInformation();
    QVariantMap testReadCellularData();
    QVariantMap testSharedFiles();
    QVariantMap testLocation();
    QVariantMap testContacts();
    QVariantMap testTextMessages();
    QVariantMap testRecordAudio();
#if 0
    QVariantMap testPhone();
    QVariantMap testEmailMessages();
    QVariantMap testCamera();
    QVariantMap testCalendar();
    QVariantMap testBlackBerryMessenger();
#endif

    bool testPpsObject(const QString &path, bb::PpsOpenMode::Types mode = bb::PpsOpenMode::PublishSubscribe);
};


#endif // PERMTEST_HPP
