#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "hostsecurecollection.h"

/* MessageHandler */
#include "MqttInterface.h"
#include "EdgeHandler.h"
#include "DatabaseHandler.h"

namespace
{
   void registerTypes();
}

int main( int argc, char *argv[] )
{
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

   /* Configure broker (server) */
   QString broker_addr = "127.0.0.1";
   quint16 broker_port = 1883;

   /* Initialize a unique pointer to DatabaseHandler */
   QScopedPointer<MessageHandler::Gateway::DatabaseHandler> database_handler = QScopedPointer<MessageHandler::Gateway::DatabaseHandler>(
       new MessageHandler::Gateway::DatabaseHandler(broker_addr, broker_port)
   );

   return app.exec();
}

namespace
{
   void registerTypes()
   {
      constexpr auto msg = "Cannot create from QML..";
      constexpr auto uri = "HostSecure";
      qmlRegisterUncreatableType< HostSecureCollection >( "HostSecure", 1, 0, "HostSecureCollection", msg );
      qmlRegisterUncreatableType< EdgeModel            >( uri, 1, 0, "EdgeModel", msg );
      qmlRegisterUncreatableType< UsbDeviceModel       >( uri, 1, 0, "UsbDeviceModel", msg );
   }
}






























