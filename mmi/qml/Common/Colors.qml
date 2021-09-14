pragma Singleton

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

QtObject {
   id: root

   function setColorAlpha( color, alpha ) {
       return Qt.hsla( color.hslHue, color.hslSaturation, color.hslLightness, alpha )
   }

   property color paneBackgroundColor: "#90202020"
   property color paneBorderColor: "#20FFFFFF"
   property color topBarBackgroundColor: Qt.darker( paneBackgroundColor, 1.1 )
   property color setupBackgroundColor: Qt.darker( paneBackgroundColor, 1.1 )
}
