#include "permtest.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ListView>
#include <bb/cascades/ArrayDataModel>
#include <bb/device/SimCardInfo>
#include <bb/device/SimCardState>
#include <bb/device/HardwareInfo>
#include <bb/PpsObject>
#include <bb/PpsOpenMode>

#include <sys/asoundlib.h>
#include <errno.h>

using namespace bb::cascades;

PermTest::PermTest(bb::cascades::Application *app)
: QObject(app)
{
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();

    ListView *permissionsList = root->findChild<ListView *>("permissionsList");
    ArrayDataModel *dataModel = new ArrayDataModel();
    permissionsList->setDataModel(dataModel);

    dataModel->append(testDeviceIdentifyingInformation());
    dataModel->append(testReadCellularData());
    dataModel->append(testSharedFiles());
    dataModel->append(testLocation());
    dataModel->append(testContacts());
    dataModel->append(testTextMessages());
    dataModel->append(testRecordAudio());
#if 0
    dataModel->append(testPhone());
    dataModel->append(testEmailMessages());
    dataModel->append(testCamera());
    dataModel->append(testCalendar());
    dataModel->append(testBlackBerryMessenger());
#endif

    app->setScene(root);
}
QVariantMap PermTest::testDeviceIdentifyingInformation()
{
    QVariantMap item;
    item["label"] = "Device Identifying Information";
    item["name"] = "read_device_identifying_information";

    bb::device::HardwareInfo hardwareInfo;
    bool identEnabled = !hardwareInfo.pin().isEmpty();
    item["enabled"] = identEnabled;

    return item;
}

QVariantMap PermTest::testReadCellularData()
{
    QVariantMap item;
    item["label"] = "Cellular Data";
    item["name"] = "read_cellular_data";

    bb::device::SimCardInfo simInfo;
    item["enabled"] = simInfo.state() != bb::device::SimCardState::NotDetected;

    return item;
}

QVariantMap PermTest::testSharedFiles()
{
    QVariantMap item;
    item["label"] = "Shared Files";
    item["name"] = "access_shared";

    QDir sharedDir("shared/");
    bool sharedEnabled = sharedDir.exists() && sharedDir.isReadable();
    item["enabled"] = sharedEnabled;

    return item;
}

QVariantMap PermTest::testLocation()
{
    QVariantMap item;
    item["label"] = "Location";
    item["name"] = "access_location_services";
    item["enabled"] = testPpsObject("/pps/services/geolocation/control");
    return item;
}

QVariantMap PermTest::testContacts()
{
    QVariantMap item;
    item["label"] = "Contacts";
    item["name"] = "access_pimdomain_contacts";
    item["enabled"] = testPpsObject("/pps/services/pim/session");
    return item;

    // It seems like having permission to any of the following
    // items will give permission to access "/pps/services/pim/session":
    // - contacts
    // - calendar
    // - notebooks
    // - email and pin messages
}

QVariantMap PermTest::testTextMessages()
{
    QVariantMap item;
    item["label"] = "Text Messages";
    item["name"] = "access_sms_mms";

    // Both of these appear to become readable when we have this permission.
    // To be safe, we will allow either to cause this test to pass.
    bool control_options = testPpsObject("/pps/services/cellular/sms/control_options");
    bool sms_connector = testPpsObject("/pps/services/chat/sms_connector");
    item["enabled"] = control_options || sms_connector;

    return item;
}

QVariantMap PermTest::testRecordAudio()
{
    QVariantMap item;
    item["label"] = "Microphone";
    item["name"] = "record_audio";

    snd_pcm_t *pcm_handle;
    int ret = snd_pcm_open_name(&pcm_handle, (char*)"voice", SND_PCM_OPEN_CAPTURE);
    if(ret == -ENOENT) {
        item["enabled"] = false;
    }
    else {
        item["enabled"] = true;
    }

    snd_pcm_close(pcm_handle);
    return item;
}

#if 0
QVariantMap PermTest::testPhone()
{
    return QVariantMap();
}

QVariantMap PermTest::testEmailMessages()
{
    return QVariantMap();
}

QVariantMap PermTest::testCamera()
{
    return QVariantMap();
}

QVariantMap PermTest::testCalendar()
{
    return QVariantMap();
}

QVariantMap PermTest::testBlackBerryMessenger()
{
    return QVariantMap();
}
#endif

bool PermTest::testPpsObject(const QString &path, bb::PpsOpenMode::Types mode)
{
    bb::PpsObject ppsObject(path);

    if(ppsObject.open(mode)) {
        ppsObject.close();
        return true;
    }
    else {
        return false;
    }
}
