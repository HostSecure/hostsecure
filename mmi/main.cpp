#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "hostsecurecollection.h"
#include "devicemodel.h"

namespace
{
   void registerTypes();
}

int main( int argc, char *argv[] )
{
   qputenv("QML_DISABLE_DISK_CACHE", "1");

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
   QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
   QCoreApplication::setApplicationName( "HostSecure" );
   QCoreApplication::setApplicationVersion( "1.0" );
   QCoreApplication::setOrganizationName( "HostSecure" );
   QCoreApplication::setOrganizationDomain( "usn.no" );

   QGuiApplication app(argc, argv);
   QIcon::setThemeName( "HostSecureIcons" );
   QQmlApplicationEngine engine;

   auto hsc = new HostSecureCollection( &engine );
   engine.rootContext()->setContextProperty( "HSC", hsc );

   registerTypes();
   engine.addImportPath( ":/" );

   const QUrl url( QStringLiteral("qrc:/main.qml") );
   QObject::connect( &engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url]( QObject *obj, const QUrl &objUrl ) {
      if ( !obj && url == objUrl )
         QCoreApplication::exit( -1 );
   }, Qt::QueuedConnection );
   engine.load( url );

   return app.exec();
}

namespace
{
   void registerTypes()
   {
      constexpr auto msg = "Cannot create from QML..";
      constexpr auto uri = "HostSecure";
      qmlRegisterUncreatableType< HostSecureCollection >( "HostSecure", 1, 0, "HostSecureCollection", msg );      
      qmlRegisterUncreatableType< MmiMqttClient        >( uri, 1, 0, "MmiMqttClient", msg );
      qmlRegisterUncreatableType< EdgeModel            >( uri, 1, 0, "EdgeModel", msg );
      qmlRegisterUncreatableType< DeviceModel          >( uri, 1, 0, "DeviceModel", msg );
   }
}






























