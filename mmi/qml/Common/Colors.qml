pragma Singleton

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

QtObject {
   id: root

   function setColorAlpha( color, alpha ) {
       return Qt.hsla( color.hslHue, color.hslSaturation, color.hslLightness, alpha )
   }

   property color paneBackgroundColor: setColorAlpha( Material.backgroundColor, 0.6 )
   property color paneBorderColor: "#20FFFFFF"
   property color topBarBackgroundColor: Qt.darker( Material.backgroundColor, 1.1 )
   property color setupBackgroundColor: Qt.darker( Material.backgroundColor, 1.1 )
}
